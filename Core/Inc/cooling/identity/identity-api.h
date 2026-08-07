/*!
 * \file identity-api.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-08-07
 * \brief Public API for the module dedicated to providing information about the cooling board.
 */

#ifndef IDENTITY_API_H
#define IDENTITY_API_H

#include "identity.h"

/*!
 * \brief Initialize the identity
 *
 * \retval IDENTITY_RC_OK if the module was successfully initalizated
 */
enum IdentityReturnCode identity_api_init(void);

/*!
 * \brief Send the status of the FSM periodically
 *
 * \param[in] fsm_state The state/status of the FSM
 * \param[in] tick The current time tick
 *
 * \retval IDENTITY_RC_OK if the status was sent successfully
 * \retval IDENTITY_RC_ERROR if there has been an issue trying to send the status
 */
enum IdentityReturnCode identity_api_send_status(uint8_t fsm_state, uint32_t tick);

/*!
 * \brief Send the information about the version of the firmware (and other relevant information) periodically
 *
 * \param[in] tick The current time tick
 *
 * \retval IDENTITY_RC_OK if the information was sent successfully
 * \retval IDENTITY_RC_ERROR if there has been an issue trying to send the information
 */
enum IdentityReturnCode identity_api_send_information(uint32_t tick);

#endif // IDENTITY_API_H
