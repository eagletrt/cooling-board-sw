/*!
 * \file control-api.c
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-07-31
 * \brief Implementation of the APIs of the module dedicated to providing control abstraction over the fans and pumps of the cooling board.
 */

#include "control-api.h"
#include "fsm.h"
#include "temperatures-api.h"
#include "can-communication-api.h"
#include "can-primary-api.h"
#include "pid-controller-api.h"
#include "eagletrt-api.h"

/*!
 * \brief Internal module handler
 * \details Hidden from external linkage to enforce API-only access
 */
EAGLETRT_STATIC struct ControlHandler control_handler;

/*!
 * \brief Invalid control output value
 */
constexpr float control_invalid_output = -1.F;

enum ControlReturnCode control_api_init(struct ControlPidConfig pi_configurations[CONTROL_NAME_COUNT]) {
    if (pi_configurations == nullptr) {
        return CONTROL_RC_NULL_POINTER;
    }

    enum ControlReturnCode return_code = CONTROL_RC_OK;

    arena_allocator_api_init(&control_handler.harena);

    for (uint8_t control_name = CONTROL_NAME_LEFT_PUMP; control_name < CONTROL_NAME_COUNT; control_name++) {
        int pid_init_return_code = pid_controller_api_init(&control_handler.pi_controller[control_name],
                                                           pi_configurations[control_name].kp,
                                                           pi_configurations[control_name].ki,
                                                           0.F,
                                                           pi_configurations[control_name].sample_time,
                                                           pi_configurations[control_name].anti_windup,
                                                           nullptr,
                                                           0);

        control_handler.pi_controller[control_name].set_point = pi_configurations[control_name].set_point;

        if (pid_init_return_code != PID_OK) {
            return_code = CONTROL_RC_ERROR; /*<! silently fail and keep initalizing each controller */
        }
    }

    control_api_set_mode(CONTROL_MODE_AUTOMATIC);

    return return_code;
}

void control_api_deinit(void) {
    arena_allocator_api_free(&control_handler.harena);
}

void control_api_set_mode(enum ControlMode mode) {
    control_handler.mode = mode;
}

void control_api_update_internal_status(void) {
    if (temperatures_api_get_temperatures_status() == true) {
        float front_left_motor_internal_temperature = temperatures_api_get_temperature(TEMPERATURES_NAME_FRONT_LEFT_MOTOR_INTERNAL_TEMPERATURE);
        float rear_left_motor_internal_temperature = temperatures_api_get_temperature(TEMPERATURES_NAME_REAR_LEFT_MOTOR_INTERNAL_TEMPERATURE);
        float front_right_motor_internal_temperature = temperatures_api_get_temperature(TEMPERATURES_NAME_FRONT_RIGHT_MOTOR_INTERNAL_TEMPERATURE);
        float rear_right_motor_internal_temperature = temperatures_api_get_temperature(TEMPERATURES_NAME_REAR_RIGHT_MOTOR_INTERNAL_TEMPERATURE);
        float tsac_temperature = temperatures_api_get_temperature(TEMPERATURES_NAME_TSAC_TEMPERATURE);
        float inverter_temperature = temperatures_api_get_temperature(TEMPERATURES_NAME_INVERTER_TEMPERATURE);

        float left_motors_max_temperature = EAGLETRT_API_MAX(front_left_motor_internal_temperature,
                                                             rear_left_motor_internal_temperature);
        float right_motors_max_temperature = EAGLETRT_API_MAX(front_right_motor_internal_temperature,
                                                              rear_right_motor_internal_temperature);

        constexpr float tsac_reference_temperature = 40.F;
        constexpr float inverter_reference_temperature = 60.F;
        constexpr float motors_reference_temperature = 80.F;

        if (inverter_temperature >= inverter_reference_temperature && left_motors_max_temperature <= motors_reference_temperature) {
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_LEFT_FAN], inverter_temperature);
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_LEFT_PUMP], inverter_temperature);
        } else {
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_LEFT_FAN], left_motors_max_temperature);
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_LEFT_PUMP], left_motors_max_temperature);
        }

        if (tsac_temperature >= tsac_reference_temperature && right_motors_max_temperature <= motors_reference_temperature) {
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_RIGHT_FAN], tsac_temperature);
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_RIGHT_PUMP], tsac_temperature);
        } else {
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_RIGHT_FAN], right_motors_max_temperature);
            pid_controller_api_update(&control_handler.pi_controller[CONTROL_NAME_RIGHT_PUMP], right_motors_max_temperature);
        }

        constexpr float motors_maximum_admissible_temperature = 110.F;

        if (left_motors_max_temperature >= motors_maximum_admissible_temperature) {
            // TODO: decrease set point of left circuit PI configurations
        }

        if (right_motors_max_temperature >= motors_maximum_admissible_temperature) {
            // TODO: decrease set point of right circuit PI configurations
        }

        control_api_update_left_pump_output();
        control_api_update_left_fan_output();
        control_api_update_right_pump_output();
        control_api_update_right_fan_output();
    }
}

EAGLETRT_STATIC enum ControlReturnCode prv_control_update_output(enum ControlName control_name) {
    if (control_name >= CONTROL_NAME_COUNT) {
        return CONTROL_RC_INVALID_NAME;
    }

    switch (control_handler.mode) {
        case CONTROL_MODE_AUTOMATIC:
            control_handler.output[control_name] = EAGLETRT_API_CLAMP(pid_controller_api_compute(&control_handler.pi_controller[control_name]), 0.F, 1.F);
            break;
        case CONTROL_MODE_0:
            control_handler.output[control_name] = 0.F;
            break;
        case CONTROL_MODE_25:
            control_handler.output[control_name] = 0.25F;
            break;
        case CONTROL_MODE_50:
            control_handler.output[control_name] = 0.5F;
            break;
        case CONTROL_MODE_75:
            control_handler.output[control_name] = 0.75F;
            break;
        case CONTROL_MODE_100:
            control_handler.output[control_name] = 1.F;
            break;
        default:
            return CONTROL_RC_INVALID_MODE;
    }

    return CONTROL_RC_OK;
}

enum ControlReturnCode control_api_update_left_pump_output() {
    return prv_control_update_output(CONTROL_NAME_LEFT_PUMP);
}

enum ControlReturnCode control_api_update_left_fan_output() {
    return prv_control_update_output(CONTROL_NAME_LEFT_FAN);
}

enum ControlReturnCode control_api_update_right_pump_output() {
    return prv_control_update_output(CONTROL_NAME_RIGHT_PUMP);
}

enum ControlReturnCode control_api_update_right_fan_output() {
    return prv_control_update_output(CONTROL_NAME_RIGHT_FAN);
}

float control_api_get_output(enum ControlName control_name) {
    if (control_name >= CONTROL_NAME_COUNT) {
        return control_invalid_output;
    }

    return control_handler.output[control_name];
}

enum ControlReturnCode control_api_periodically_send_outputs(uint32_t tick) {
    if (tick - control_handler.last_send_tick_outputs >= can_primary_cycle_time_coolingout) {
        control_handler.last_send_tick_outputs = tick;

        union CanPrimaryMessages message = { .coolingout = {
                                                 .pumpleft = control_handler.output[CONTROL_NAME_LEFT_PUMP],
                                                 .pumpright = control_handler.output[CONTROL_NAME_RIGHT_PUMP],
                                                 .fanleft = control_handler.output[CONTROL_NAME_LEFT_FAN],
                                                 .fanright = control_handler.output[CONTROL_NAME_RIGHT_FAN] } };
        struct CanCommunicationFrame frame = { .id = CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGOUT };

        if (can_primary_api_serialize_from_id(frame.id, &message, frame.data) != -1) {
            frame.length = can_primary_byte_size_coolingout;
            EAGLETRT_API_UNUSED(can_communication_api_add_to_tx_buffer(CAN_COMMUNICATION_NETWORK_PRIMARY, &frame));
        }
    }

    return CONTROL_RC_OK;
}
