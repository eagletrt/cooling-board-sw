#ifndef BOOTLOADER_API_H
#define BOOTLOADER_API_H

#include "bootloader.h"
#include "can-communication.h"

enum BootloaderReturnCode bootloader_api_init(struct TimebaseHandler *timebase, struct WatchdogHandler *watchdog_pool);

enum BootloaderReturnCode bootloader_api_tick(void);

enum CanCommunicationReturnCode bootloader_api_receive(const struct CanCommunicationFrame *frame);

bool bootloader_is_requested(void);

bool bootloader_is_flashing(void);

#endif // BOOTLOADER_API_H
