#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "timebase.h"
#include "watchdogs.h"

#define BOOTLOADER_FLASH_TIMEOUT 500U

typedef void (*reboot_callback)(void);

enum BootloaderReturnCode {
    BOOTLOADER_RC_OK = 0,
    BOOTLOADER_RC_NULL_POINTER,
    BOOTLOADER_RC_ERROR
};

struct BootloaderHandler {
    struct TimebaseHandler *timebase;
    struct WatchdogHandler *watchdog_pool;
    struct Watchdog *watchdog;
    reboot_callback reboot;
    bool flashing;
    bool timedout;
};

#endif // BOOTLOADER_H
