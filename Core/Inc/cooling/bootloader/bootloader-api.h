#ifndef BOOTLOADER_API_H
#define BOOTLOADER_API_H

#include "bootloader.h"

enum BootloaderReturnCode bootloader_api_init(struct TimebaseHandler *timebase, struct WatchdogHandler *watchdog_pool, reboot_callback reboot);

enum BootloaderReturnCode bootloader_api_reboot(void);

enum BootloaderReturnCode bootloader_api_watchdog_reset(void);

enum BootloaderReturnCode bootloader_api_watchdog_pet(void);

bool bootloader_api_is_timedout(void);

bool bootloader_api_is_flashing(void);

enum BootloaderReturnCode bootloader_api_set_flashing(bool flashing);

#endif // BOOTLOADER_API_H
