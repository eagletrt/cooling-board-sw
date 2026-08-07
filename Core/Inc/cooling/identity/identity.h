/*!
 * \file identity.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-08-07
 * \brief Type definitions for the module dedicated to providing information about the cooling board.
 */

#ifndef IDENTITY_H
#define IDENTITY_H

#include <stdint.h>

#include "can-primary.h"

/*!
 * \brief Return codes for the identity module functions
 */
enum IdentityReturnCode {
    IDENTITY_RC_OK,
    IDENTITY_RC_ERROR
};

enum IdentityMessage : uint8_t {
    IDENTITY_MESSAGE_VERSION,
    IDENTITY_MESSAGE_VERSION_INFO,
    IDENTITY_MESSAGE_LIBCAN_VERSION,
    IDENTITY_MESSAGE_LIBCAN_VERSION_INFO,
    IDENTITY_MESSAGE_COUNT
};

/*!
 * \brief The handler of the module which contains the information about the firmware and its dependencies
 */
struct IdentityHandler {
    uint32_t firmware_build_time;
    uint32_t last_send_tick;
};

#endif // IDENTITY_H
