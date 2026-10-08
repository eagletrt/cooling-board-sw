#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "timebase.h"
#include "watchdogs.h"

enum BootloaderReturnCode {
    BOOTLOADER_RC_OK = 0,
    BOOTLOADER_RC_NULL_POINTER,
    BOOTLOADER_RC_ERROR
};

struct BootloaderHandler {
    struct TimebaseHandler *timebase;
    struct WatchdogHandler *watchdog_pool;
    struct Watchdog *watchdog;
    bool flashing;
    bool requested;
};

#endif // BOOTLOADER_H
