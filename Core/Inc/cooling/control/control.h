/*!
 * \file control.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-07-31
 * \brief Type definitions for the module dedicated to providing control abstraction over the fans and pumps of the cooling board.
 */

#include <stdint.h>

#include "arena-allocator.h"
#include "pid-controller.h"

#ifndef CONTROL_H
#define CONTROL_H

/*!
 * \brief Return codes for the control module functions
 */
enum ControlReturnCode {
    CONTROL_RC_OK,           /*!< The function executed succesfully */
    CONTROL_RC_NULL_POINTER, /*!< An argument of pointer type was null or a null pointer was used inside the function */
    CONTROL_RC_INVALID_NAME, /*!< The name of the control output that was provided is not valid */
    CONTROL_RC_INVALID_MODE, /*!< The mode that was provided is not valid */
    CONTROL_RC_ERROR         /*!< The function encountered an error */
};

/*!
 * \brief Names for the control outputs of the module
 */
enum ControlName : uint8_t {
    CONTROL_NAME_LEFT_PUMP,  /*!< The left pump control output */
    CONTROL_NAME_LEFT_FAN,   /*!< The left fan control output */
    CONTROL_NAME_RIGHT_PUMP, /*!< The right pump control output */
    CONTROL_NAME_RIGHT_FAN,  /*!< The right fan control output */
    CONTROL_NAME_COUNT
};

/*!
 * \brief List of all the different modes in which you can control the outputs
 */
enum ControlMode : uint8_t {
    CONTROL_MODE_AUTOMATIC, /*!< automatic mode, output value gets handled by the PID */
    CONTROL_MODE_MANUAL     /*!< manual mode, output value has to be adjusted by hand */

};

/*!
 * \brief The configuration of the PI controller
 */
struct ControlPidConfig {
    float kp;          /*!< Proportional gain (Kp) */
    float ki;          /*!< Integral gain (Ki) */
    float set_point;   /*!< Set point */
    float sample_time; /*!< Sample time in seconds */
    float anti_windup; /*!< Anti-windup limit */
};

/*!
 * \brief The handler containing the handler to the arena, the various controllers and the output values
 */
struct ControlHandler {
    struct ArenaAllocatorHandler harena;                    /*!< Arena allocator used by the PI controllers */
    struct PidController pi_controller[CONTROL_NAME_COUNT]; /*!< The PI controllers */
    float output[CONTROL_NAME_COUNT];                       /*!< The output values */
    uint32_t last_send_tick_outputs;
};

#endif // CONTROL_H
