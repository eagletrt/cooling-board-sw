/*!
 * \file can-communication-router-api.c
 * \date 2026-07-23
 * \authors Alessandro Bridi [ale.bridi15@gmail.com]
 * \authors Thomas Moretti [majorfrost173@gmail.com]
 *
 * \brief Implementation of the hardware-agnostic CAN messages router.
 */

#include "can-communication-router-api.h"
#include "can-primary-api.h"
#include "can-primary.h"
#include "control-api.h"
#include "temperatures-api.h"

EAGLETRT_STATIC void prv_dispatch_rx(uint32_t id, union CanPrimaryMessages message) {
    switch (id) {
        case CAN_PRIMARY_MESSAGE_FRAME_ID_MOTORTEMPERATURE: {
            temperatures_api_set_temperature(TEMPERATURES_NAME_FRONT_LEFT_MOTOR_INTERNAL_TEMPERATURE,
                                             message.motortemperature.temperaturefl);
            temperatures_api_set_temperature(TEMPERATURES_NAME_REAR_LEFT_MOTOR_INTERNAL_TEMPERATURE,
                                             message.motortemperature.temperaturerl);
            temperatures_api_set_temperature(TEMPERATURES_NAME_FRONT_RIGHT_MOTOR_INTERNAL_TEMPERATURE,
                                             message.motortemperature.temperaturefr);
            temperatures_api_set_temperature(TEMPERATURES_NAME_REAR_RIGHT_MOTOR_INTERNAL_TEMPERATURE,
                                             message.motortemperature.temperaturerr);
            break;
        }
        case CAN_PRIMARY_MESSAGE_FRAME_ID_TSACMAINBOARDTEMPERATUREINFO: {
            temperatures_api_set_temperature(TEMPERATURES_NAME_TSAC_TEMPERATURE,
                                             message.tsacmainboardtemperatureinfo.max);
            break;
        }
        case CAN_PRIMARY_MESSAGE_FRAME_ID_INVERTERTEMPERATURE: {
            float inverter_left_temperature_max = EAGLETRT_API_MAX(message.invertertemperature.temperaturefl,
                                                                   message.invertertemperature.temperaturerl);
            float inverter_right_temperature_max = EAGLETRT_API_MAX(message.invertertemperature.temperaturefr,
                                                                    message.invertertemperature.temperaturerr);
            float inverter_temperature_max = EAGLETRT_API_MAX(inverter_left_temperature_max,
                                                              inverter_right_temperature_max);

            temperatures_api_set_temperature(TEMPERATURES_NAME_INVERTER_TEMPERATURE,
                                             inverter_temperature_max);
            break;
        }
        case CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGCONTROLMODE: {
            if (message.coolingcontrolmode.mode == CAN_PRIMARY_COOLINGCONTROLMODE_MODE_AUTO) {
                control_api_set_mode(CONTROL_MODE_AUTOMATIC);
            } else if (message.coolingcontrolmode.mode == CAN_PRIMARY_COOLINGCONTROLMODE_MODE_MODE_0) {
                control_api_set_mode(CONTROL_MODE_0);
            } else if (message.coolingcontrolmode.mode == CAN_PRIMARY_COOLINGCONTROLMODE_MODE_MODE_1) {
                control_api_set_mode(CONTROL_MODE_25);
            } else if (message.coolingcontrolmode.mode == CAN_PRIMARY_COOLINGCONTROLMODE_MODE_MODE_2) {
                control_api_set_mode(CONTROL_MODE_50);
            } else if (message.coolingcontrolmode.mode == CAN_PRIMARY_COOLINGCONTROLMODE_MODE_MODE_3) {
                control_api_set_mode(CONTROL_MODE_75);
            } else if (message.coolingcontrolmode.mode == CAN_PRIMARY_COOLINGCONTROLMODE_MODE_MODE_4) {
                control_api_set_mode(CONTROL_MODE_100);
            }
        }
        default:
            break;
    }
}

enum CanCommunicationReturnCode can_communication_router_api_receive_primary(const struct CanCommunicationFrame *frame) {
    if (frame == NULL) {
        return CAN_COMMUNICATION_RC_NULL_POINTER;
    }

    if (!can_primary_api_id_is_valid(frame->id)) {
        return CAN_COMMUNICATION_RC_INVALID_NETWORK;
    }

    union CanPrimaryMessages message = { 0 };
    if (can_primary_api_deserialize_from_id(frame->id, frame->data, &message) != 0) {
        return CAN_COMMUNICATION_RC_ERROR;
    }

    prv_dispatch_rx(frame->id, message);

    return CAN_COMMUNICATION_RC_OK;
}
