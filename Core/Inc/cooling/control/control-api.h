/*!
 * \file control-api.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-07-31
 * \brief Public API for the module dedicated to providing control abstraction over the fans and pumps of the cooling board.
 */

#ifndef CONTROL_API_H
#define CONTROL_API_H

#include "control.h"

/*!
 * \brief Initalize the control module
 *
 * \param[in] pi_configurations The configurations for each PI controller (kp, ki, etc...)
 *
 * \retval CONTROL_RC_OK if the module was successfully initalized
 * \retval CONTROL_RC_ERROR if the module was not successfully initalized
 */
enum ControlReturnCode control_api_init(struct ControlPidConfig pi_configurations[CONTROL_NAME_COUNT]);

/*!
 * \brief Deinitalize the control module
 * \note Since the arena allocator should be free'd even if the program never ends, this does so
 */
void control_api_deinit(void);

/*!
 * \brief Sets the mode of the control module
 *
 * \param[in] mode The mode to set
 */
void control_api_set_mode(enum ControlMode mode);

/*!
 * \brief Updates the internal status of the control module (status variables, config, etc...)
 */
void control_api_update_internal_status(void);

/*!
 * \brief Updates the value of the output dedicated to the control of the left pump
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_left_pump_output();

/*!
 * \brief Updates the value of the output dedicated to the control of the left fan
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_left_fan_output();

/*!
 * \brief Updates the value of the output dedicated to the control of the right pump
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_right_pump_output();

/*!
 * \brief Updates the value of the output dedicated to the control of the right fan
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_right_fan_output();

/*!
 * \brief Returns the value of the specified control output
 *
 * \param[in] control_name The name of the control output
 * \return The value of the control output
 */
float control_api_get_output(enum ControlName control_name);

/*!
 * \brief Periodically send the value of the outputs
 *
 * \param[in] tick The current time tick in ms
 */
enum ControlReturnCode control_api_periodically_send_outputs(uint32_t tick);

#endif // CONTROL_API_H
