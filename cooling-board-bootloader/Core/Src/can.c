/************************************************************************************/ /**
* \file         Source/ARMCM0_STM32C0/can.c
* \brief        Bootloader CAN communication interface source file for STM32C0.
* \ingroup      Target_ARMCM0_STM32C0
****************************************************************************************/
#include "boot.h"                                                                      /* OpenBLT generic header */
#if (BOOT_COM_CAN_ENABLE > 0)
#include "stm32c0xx_hal.h" /* HAL header for C0 family */

/* Timeout for transmitting a CAN message in milliseconds. */
#define CAN_MSG_TX_TIMEOUT_MS (50u)

/* map the configured CAN channel index to the STM32's FDCAN peripheral */
#if (BOOT_COM_CAN_CHANNEL_INDEX == 0)
#define CAN_CHANNEL FDCAN1
#elif (BOOT_COM_CAN_CHANNEL_INDEX == 1)
#define CAN_CHANNEL FDCAN2
#else
#error "Unsupported BOOT_COM_CAN_CHANNEL_INDEX"
#endif

/* Simple CAN bus timing table (TSEG1/TSEG2 combos with acceptable sample point). */
typedef struct t_can_bus_t {
    blt_int8u tseg1;
    blt_int8u tseg2;
} tCanBusTiming;

static const tCanBusTiming canTiming[] = {
    { 5, 2 }, { 6, 2 }, { 6, 3 }, { 7, 3 }, { 8, 3 }, { 9, 3 }, { 9, 4 }, { 10, 4 }, { 11, 4 }, { 12, 4 }, { 12, 5 }, { 13, 5 }, { 14, 5 }, { 15, 5 }, { 15, 6 }, { 16, 6 }, { 16, 7 }, { 16, 8 }
};

/* CAN handle used by HAL calls */
static FDCAN_HandleTypeDef canHandle;

/************************************************************************************/ /**
** \brief     Find a valid prescaler/TSEG1/TSEG2 for the requested baudrate (kbit/s).
** \param     baud Requested baudrate in kbps (10..1000).
** \param     prescaler Pointer to store computed prescaler.
** \param     tseg1 Pointer to store computed tseg1.
** \param     tseg2 Pointer to store computed tseg2.
** \return    BLT_TRUE on success, BLT_FALSE otherwise.
**
****************************************************************************************/
static blt_bool CanGetSpeedConfig(blt_int16u baud, blt_int16u *prescaler, blt_int8u *tseg1, blt_int8u *tseg2) {
    blt_int32u canClockFreqkHz;
    blt_int8u i;

    /* The driver assumes the CAN peripheral is clocked by the external crystal (XTAL).
   * Ensure BOOT_CPU_XTAL_SPEED_KHZ is configured in blt_conf.h.
   */
    canClockFreqkHz = (blt_int32u)BOOT_CPU_XTAL_SPEED_KHZ;

    for (i = 0; i < (blt_int8u)(sizeof(canTiming) / sizeof(canTiming[0])); ++i) {
        blt_int32u tq_total = (blt_int32u)(canTiming[i].tseg1 + canTiming[i].tseg2 + 1u);
        blt_int32u denom = (blt_int32u)baud * tq_total;
        if (denom == 0)
            continue;
        if ((canClockFreqkHz % denom) == 0) {
            blt_int32u pres = canClockFreqkHz / denom;
            if ((pres > 0) && (pres <= 512)) {
                *prescaler = (blt_int16u)pres;
                *tseg1 = canTiming[i].tseg1;
                *tseg2 = canTiming[i].tseg2;
                return BLT_TRUE;
            }
        }
    }
    return BLT_FALSE;
}

/************************************************************************************/ /**
** \brief     Initializes the CAN controller and synchronizes it to the CAN bus.
** \return    none.
**
****************************************************************************************/
void CanInit(void) {
    blt_int16u prescaler = 0;
    blt_int8u tseg1 = 0, tseg2 = 0;
    blt_int32u rxMsgId = BOOT_COM_CAN_RX_MSG_ID;
    FDCAN_FilterTypeDef filterConfig;

    ASSERT_CT((BOOT_COM_CAN_CHANNEL_INDEX == 0 || BOOT_COM_CAN_CHANNEL_INDEX == 1));

    if (CanGetSpeedConfig((blt_int16u)(BOOT_COM_CAN_BAUDRATE / 1000u),
                          &prescaler,
                          &tseg1,
                          &tseg2) == BLT_FALSE) {
        ASSERT_RT(BLT_FALSE);
    }

    canHandle.Instance = CAN_CHANNEL;
    canHandle.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    canHandle.Init.Mode = FDCAN_MODE_NORMAL;
    canHandle.Init.AutoRetransmission = ENABLE;
    canHandle.Init.TransmitPause = DISABLE;
    canHandle.Init.ProtocolException = DISABLE;
    canHandle.Init.NominalPrescaler = prescaler;
    canHandle.Init.NominalSyncJumpWidth = 1;
    canHandle.Init.NominalTimeSeg1 = tseg1;
    canHandle.Init.NominalTimeSeg2 = tseg2;
    canHandle.Init.DataPrescaler = 1;
    canHandle.Init.DataSyncJumpWidth = 1;
    canHandle.Init.DataTimeSeg1 = 1;
    canHandle.Init.DataTimeSeg2 = 1;

    (void)HAL_FDCAN_Init(&canHandle);

    if ((BOOT_COM_CAN_RX_MSG_ID & 0x80000000U) == 0U) {
        filterConfig.IdType = FDCAN_STANDARD_ID;
    } else {
        filterConfig.IdType = FDCAN_EXTENDED_ID;
        rxMsgId &= ~0x80000000U;
    }
    filterConfig.FilterIndex = 0;
    filterConfig.FilterType = FDCAN_FILTER_MASK;
    filterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filterConfig.FilterID1 = rxMsgId;
    filterConfig.FilterID2 = 0x7FF; /* mask = exact match for std id */
    (void)HAL_FDCAN_ConfigFilter(&canHandle, &filterConfig);

    HAL_FDCAN_ConfigGlobalFilter(&canHandle,
                                 FDCAN_REJECT,
                                 FDCAN_REJECT,
                                 FDCAN_REJECT_REMOTE,
                                 FDCAN_REJECT_REMOTE);

    (void)HAL_FDCAN_Start(&canHandle);
}

/************************************************************************************/ /**
** \brief     Transmits a packet formatted for the communication interface.
** \param     data Pointer to byte array with data that is to be transmitted.
** \param     len  Number of bytes that are to be transmitted.
** \return    none.
**
****************************************************************************************/
void CanTransmitPacket(blt_int8u *data, blt_int8u len) {
    blt_int32u txMsgId = BOOT_COM_CAN_TX_MSG_ID;
    FDCAN_TxHeaderTypeDef txMsgHeader;
    blt_int32u timeout;
    HAL_StatusTypeDef status;

    if ((txMsgId & 0x80000000U) == 0U) {
        txMsgHeader.Identifier = txMsgId;
        txMsgHeader.IdType = FDCAN_STANDARD_ID;
    } else {
        txMsgId &= ~0x80000000U;
        txMsgHeader.Identifier = txMsgId;
        txMsgHeader.IdType = FDCAN_EXTENDED_ID;
    }

    txMsgHeader.TxFrameType = FDCAN_DATA_FRAME;
    txMsgHeader.DataLength = FDCAN_DLC_BYTES_8;
    txMsgHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txMsgHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txMsgHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txMsgHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txMsgHeader.MessageMarker = 0;

    status = HAL_FDCAN_AddMessageToTxFifoQ(&canHandle, &txMsgHeader, data);
    if (status == HAL_OK) {
        timeout = TimerGet() + CAN_MSG_TX_TIMEOUT_MS;
        while (HAL_FDCAN_GetTxFifoFreeLevel(&canHandle) == 0U) {
            CopService();
            if (TimerGet() > timeout) {
                break;
            }
        }
    }
}

/************************************************************************************/ /**
** \brief     Receives a communication interface packet if one is present.
** \param     data Pointer to byte array where the data is to be stored.
** \param     len Pointer where the length of the packet is to be stored.
** \return    BLT_TRUE if a packet was received, BLT_FALSE otherwise.
**
****************************************************************************************/
blt_bool CanReceivePacket(blt_int8u *data, blt_int8u *len) {
    blt_bool result = BLT_FALSE;
    blt_int32u rxMsgId = BOOT_COM_CAN_RX_MSG_ID;
    FDCAN_RxHeaderTypeDef rxMsgHeader;
    HAL_StatusTypeDef rxStatus = HAL_ERROR;

    /* check FIFO0 */
    if (HAL_FDCAN_GetRxFifoFillLevel(&canHandle, FDCAN_RX_FIFO0) > 0) {
        rxStatus = HAL_FDCAN_GetRxMessage(&canHandle, FDCAN_RX_FIFO0, &rxMsgHeader, data);
    }

    if (rxStatus == HAL_OK) {
        if ((BOOT_COM_CAN_RX_MSG_ID & 0x80000000U) == 0U) {
            if ((rxMsgHeader.Identifier == rxMsgId) &&
                (rxMsgHeader.IdType == FDCAN_STANDARD_ID)) {
                result = BLT_TRUE;
            }
        } else {
            rxMsgId &= ~0x80000000U;
            if ((rxMsgHeader.Identifier == rxMsgId) &&
                (rxMsgHeader.IdType == FDCAN_EXTENDED_ID)) {
                result = BLT_TRUE;
            }
        }

        if (result == BLT_TRUE) {
            *len = (blt_int8u)(rxMsgHeader.DataLength >> 16U);
        }
    }

    return result;
}
#endif /* BOOT_COM_CAN_ENABLE > 0 */
/*********************************** end of can.c **************************************/
