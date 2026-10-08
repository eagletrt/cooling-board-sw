#include "bootloader-api.h"

#include "timebase-api.h"
#include "watchdogs-api.h"
#include "eagletrt.h"

EAGLETRT_STATIC struct BootloaderHandler bootloader_handler;

void prv_bootloader_api_flash_timeout(void) {
    bootloader_handler.flashing = false;
}

enum BootloaderReturnCode bootloader_api_init(struct TimebaseHandler *timebase, struct WatchdogHandler *watchdog_pool) {
    memset(&bootloader_handler, 0, sizeof(bootloader_handler));

    if (timebase == nullptr || watchdog_pool == nullptr) {
        return BOOTLOADER_RC_NULL_POINTER;
    }
    bootloader_handler.timebase = timebase;
    bootloader_handler.watchdog_pool = watchdog_pool;

    watchdogs_api_init_watchdog(bootloader_handler.watchdog, 500U, prv_bootloader_api_flash_timeout); // TODO: fix

    bootloader_handler.flashing = false;
    bootloader_handler.requested = false;

    return BOOTLOADER_RC_OK;
}

enum BootloaderReturnCode bootloader_api_tick(void) {
    watchdogs_api_routine(bootloader_handler.watchdog_pool, timebase_get_tick(bootloader_handler.timebase));

    return BOOTLOADER_RC_OK;
}

enum CanCommunicationReturnCode bootloader_api_receive(const struct CanCommunicationFrame *frame) {
    if (frame == nullptr) {
        return CAN_COMMUNICATION_RC_NULL_POINTER;
    }
    if (frame->length > CAN_COMMUNICATION_FRAME_DATA_SIZE) {
        return CAN_COMMUNICATION_RC_INVALID_LENGTH;
    }
    // TODO: check id range

    if (true /* is in flash state */) {
        /*  */
        bootloader_handler.flashing = true;
        watchdogs_api_watchdog_start(
            bootloader_handler.watchdog_pool,
            bootloader_handler.watchdog,
            timebase_get_tick(bootloader_handler.timebase));
    }

    if (true /* requested reboot */) {
        bootloader_handler.requested = true;
    }

    return CAN_COMMUNICATION_RC_OK;
}

bool bootloader_api_is_flashing(void) {
    return bootloader_handler.flashing;
}

bool bootloader_is_requested(void) {
    return bootloader_handler.requested;
}
