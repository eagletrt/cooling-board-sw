/*!
 * \file control-api.c
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-07-31
 * \brief Implementation of the APIs of the module dedicated to providing control abstraction over the fans and pumps of the cooling board.
 */

#include "control-api.h"
#include "pid-controller-api.h"
#include "eagletrt.h"

EAGLETRT_STATIC struct ControlHandler control_handler;

enum ControlReturnCode control_api_init(struct ControlPidConfig pi_configurations[CONTROL_NAME_COUNT]) {
    enum ControlReturnCode return_code = CONTROL_RC_OK;

    arena_allocator_api_init(&control_handler.harena);

    for (uint8_t control_name = CONTROL_NAME_LEFT_PUMP; control_name < CONTROL_NAME_COUNT; control_name++) {
        int pid_init_return_code = pid_controller_api_init(&control_handler.pi_controller[control_name],
                                                           pi_configurations[control_name].ki,
                                                           pi_configurations[control_name].ki,
                                                           0.0f,
                                                           pi_configurations[control_name].sample_time,
                                                           pi_configurations[control_name].anti_windup,
                                                           nullptr,
                                                           0);

        control_handler.pi_controller[control_name].set_point = pi_configurations[control_name].set_point;

        if (pid_init_return_code != PID_OK) {
            return_code = CONTROL_RC_ERROR; /*<! silently fail and keep initalizing each controller */
        }
    }

    return return_code;
}

EAGLETRT_STATIC enum ControlReturnCode prv_control_update_output(enum ControlName control_name, enum ControlMode control_mode, float control_percentage) {
    if (control_name >= CONTROL_NAME_COUNT) {
        return CONTROL_RC_INVALID_NAME;
    }

    switch (control_mode) {
        case CONTROL_MODE_AUTOMATIC:
            control_handler.output[control_name] = pid_controller_api_compute(&control_handler.pi_controller[control_name]);
            break;
        case CONTROL_MODE_MANUAL:
            control_handler.output[control_name] = control_percentage;
            break;
        default:
            return CONTROL_RC_INVALID_MODE;
    }

    return CONTROL_RC_OK;
}

enum ControlReturnCode control_api_update_left_pump_output(enum ControlMode control_mode, float control_percentage) {
    return prv_control_update_output(CONTROL_NAME_LEFT_PUMP, control_mode, control_percentage);
}

enum ControlReturnCode control_api_update_left_fan_output(enum ControlMode control_mode, float control_percentage) {
    return prv_control_update_output(CONTROL_NAME_LEFT_FAN, control_mode, control_percentage);
}

enum ControlReturnCode control_api_update_right_pump_output(enum ControlMode control_mode, float control_percentage) {
    return prv_control_update_output(CONTROL_NAME_RIGHT_PUMP, control_mode, control_percentage);
}

enum ControlReturnCode control_api_update_right_fan_output(enum ControlMode control_mode, float control_percentage) {
    return prv_control_update_output(CONTROL_NAME_RIGHT_FAN, control_mode, control_percentage);
}
