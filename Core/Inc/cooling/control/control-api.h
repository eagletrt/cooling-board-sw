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
 * \brief Updates the value of the output dedicated to the control of the left pump
 *
 * \param[in] control_mode The mode used to update the control output
 * \param[in] control_percentage The power percentage at which to run the pump
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_left_pump_output(enum ControlMode control_mode, float control_percentage);

/*!
 * \brief Updates the value of the output dedicated to the control of the left fan
 *
 * \param[in] control_mode The mode used to update the control output
 * \param[in] control_percentage The power percentage at which to run the fan
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_left_fan_output(enum ControlMode control_mode, float control_percentage);

/*!
 * \brief Updates the value of the output dedicated to the control of the right pump
 *
 * \param[in] control_mode The mode used to update the control output
 * \param[in] control_percentage The power percentage at which to run the pump
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_right_pump_output(enum ControlMode control_mode, float control_percentage);

/*!
 * \brief Updates the value of the output dedicated to the control of the right fan
 *
 * \param[in] control_mode The mode used to update the control output
 * \param[in] control_percentage The power percentage at which to run the fan
 *
 * \note The control percentage parameter is taken into account only if the mode is set to manual, otherwise it is ignored
 *
 * \retval CONTROL_RC_OK if the control output has been updated successfully
 * \retval CONTROL_RC_INVALID_MODE if the control mode is invalid
 * \retval CONTROL_RC_ERROR if there has been issue trying to update the control output
 */
enum ControlReturnCode control_api_update_right_fan_output(enum ControlMode control_mode, float control_percentage);

#endif // CONTROL_API_H
