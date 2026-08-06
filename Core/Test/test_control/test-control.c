#include <unity.h>

#include <string.h>
#include "eagletrt.h"
#include "temperatures-api.h"
#include "control-api.h"

extern struct ControlHandler control_handler;

EAGLETRT_STATIC struct ControlPidConfig test_config[CONTROL_NAME_COUNT] = {
    [CONTROL_NAME_LEFT_PUMP] = { .5f, .4f, 20.5f, .01f, 100.f },
    [CONTROL_NAME_LEFT_FAN] = { .1f, .7f, 32.f, .05f, 100.f },
    [CONTROL_NAME_RIGHT_PUMP] = { .6f, .4f, 19.f, .1f, 125.f },
    [CONTROL_NAME_RIGHT_FAN] = { .3f, .8f, 50.f, .1f, 130.f },
};

void tearDown(void) {
    control_api_deinit();
}

void test_control_null_pointer_as_init_argument(void) {
    TEST_ASSERT_EQUAL(CONTROL_RC_NULL_POINTER, control_api_init(nullptr));
}

void test_control_valid_config_as_init_argument(void) {
    TEST_ASSERT_EQUAL(CONTROL_RC_OK, control_api_init(test_config));
}

void test_control_init_successfully_works_on_pid_controller(void) {
    control_api_init(test_config);

    TEST_ASSERT_EQUAL_FLOAT(control_handler.pi_controller[CONTROL_NAME_LEFT_PUMP].kp, test_config[CONTROL_NAME_LEFT_PUMP].kp);
    TEST_ASSERT_EQUAL_FLOAT(control_handler.pi_controller[CONTROL_NAME_LEFT_PUMP].ki, test_config[CONTROL_NAME_LEFT_PUMP].ki);
    TEST_ASSERT_EQUAL_FLOAT(control_handler.pi_controller[CONTROL_NAME_LEFT_PUMP].set_point, test_config[CONTROL_NAME_LEFT_PUMP].set_point);
    TEST_ASSERT_EQUAL_FLOAT(control_handler.pi_controller[CONTROL_NAME_LEFT_PUMP].sample_time, test_config[CONTROL_NAME_LEFT_PUMP].sample_time);
    TEST_ASSERT_EQUAL_FLOAT(control_handler.pi_controller[CONTROL_NAME_LEFT_PUMP].anti_windUp, test_config[CONTROL_NAME_LEFT_PUMP].anti_windup);
}

void test_control_update_output_in_invalid_mode(void) {
    control_api_init(test_config);

    constexpr uint8_t control_mode_invalid = UINT8_MAX;

    TEST_ASSERT_EQUAL(CONTROL_RC_INVALID_MODE, control_api_update_left_fan_output(control_mode_invalid, 0.f));
}

void test_control_update_output_works_as_expected_in_manual_mode(void) {
    control_api_init(test_config);

    constexpr float test_percentage = 0.8f;

    TEST_ASSERT_EQUAL(CONTROL_RC_OK, control_api_update_left_fan_output(CONTROL_MODE_MANUAL, test_percentage));
    TEST_ASSERT_EQUAL_FLOAT(test_percentage, control_handler.output[CONTROL_NAME_LEFT_FAN]);
}

void test_control_update_output_clamps_value_under_range_in_manual_mode(void) {
    control_api_init(test_config);

    constexpr float test_percentage_below_range = -0.6f;

    TEST_ASSERT_EQUAL(CONTROL_RC_OK, control_api_update_right_fan_output(CONTROL_MODE_MANUAL, test_percentage_below_range));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, control_handler.output[CONTROL_NAME_RIGHT_FAN]);
}

void test_control_update_output_clamps_value_over_range_in_manual_mode(void) {
    control_api_init(test_config);

    constexpr float test_percentage_above_range = 1.4f;

    TEST_ASSERT_EQUAL(CONTROL_RC_OK, control_api_update_right_pump_output(CONTROL_MODE_MANUAL, test_percentage_above_range));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, control_handler.output[CONTROL_NAME_RIGHT_PUMP]);
}

void test_control_update_output_is_deterministic_in_automatic_mode(void) {
    control_api_init(test_config);

    constexpr float test_process_variable = 48.f;

    temperatures_api_set_temperature(TEMPERATURES_NAME_REAR_RIGHT_MOTOR_INTERNAL_TEMPERATURE, test_process_variable);

    control_api_update_internal_status();

    control_api_update_right_fan_output(CONTROL_MODE_AUTOMATIC, 0.f);

    float expected_error = test_config[CONTROL_NAME_RIGHT_FAN].set_point - test_process_variable;
    float expected_integrator = expected_error * test_config[CONTROL_NAME_RIGHT_FAN].sample_time;
    float expected_output = test_config[CONTROL_NAME_RIGHT_FAN].kp * expected_error + test_config[CONTROL_NAME_RIGHT_FAN].ki * expected_integrator;

    TEST_ASSERT_EQUAL_FLOAT(expected_output, control_api_get_output(CONTROL_NAME_RIGHT_FAN));
}

void test_control_update_output_clamps_value_under_range_in_automatic_mode(void) {
    control_api_init(test_config);

    float test_process_variable_over_right_fan_set_point = test_config[CONTROL_NAME_RIGHT_FAN].set_point + 10.f;

    temperatures_api_set_temperature(TEMPERATURES_NAME_REAR_RIGHT_MOTOR_INTERNAL_TEMPERATURE, test_process_variable_over_right_fan_set_point);

    control_api_update_internal_status();

    control_api_update_right_fan_output(CONTROL_MODE_AUTOMATIC, 0.f);

    TEST_ASSERT_EQUAL_FLOAT(0.f, control_api_get_output(CONTROL_NAME_RIGHT_FAN));
}

void test_control_update_output_clamps_value_over_range_in_automatic_mode(void) {
    control_api_init(test_config);

    float test_process_variable_under_left_pump_set_point = test_config[CONTROL_NAME_LEFT_PUMP].set_point - 10.f;

    temperatures_api_set_temperature(TEMPERATURES_NAME_FRONT_LEFT_MOTOR_INTERNAL_TEMPERATURE, test_process_variable_under_left_pump_set_point);

    control_api_update_internal_status();

    control_api_update_left_pump_output(CONTROL_MODE_AUTOMATIC, 0.f);

    TEST_ASSERT_EQUAL_FLOAT(1.f, control_api_get_output(CONTROL_NAME_LEFT_PUMP));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_control_null_pointer_as_init_argument);
    RUN_TEST(test_control_valid_config_as_init_argument);
    RUN_TEST(test_control_init_successfully_works_on_pid_controller);

    RUN_TEST(test_control_update_output_in_invalid_mode);
    RUN_TEST(test_control_update_output_works_as_expected_in_manual_mode);
    RUN_TEST(test_control_update_output_clamps_value_under_range_in_manual_mode);
    RUN_TEST(test_control_update_output_clamps_value_over_range_in_manual_mode);

    RUN_TEST(test_control_update_output_is_deterministic_in_automatic_mode);
    RUN_TEST(test_control_update_output_clamps_value_under_range_in_automatic_mode);
    RUN_TEST(test_control_update_output_clamps_value_over_range_in_automatic_mode);

    return UNITY_END();
}
