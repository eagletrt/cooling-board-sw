#include <string.h>
#include <unity.h>
#include "temperatures-api.h"

extern struct TemperaturesHandler temperatures_handler;

constexpr int TEMPERATURES_NAME_INVALID_NAME = -1;

void setUp(void) {
    memset(&temperatures_handler, 0, sizeof(temperatures_handler));
}

void test_temperatures_set_temperatures_with_null_pointer_argument(void) {
    TEST_ASSERT_EQUAL(temperatures_api_set_temperatures(nullptr), TEMPERATURES_RC_NULL_POINTER);
}

void test_temperatures_set_temperatures_with_valid_argument(void) {
    const float temperatures[TEMPERATURES_NAME_COUNT] = {
        .2f, 5.1f, 22.3f, 3.6f, 13.2f, 51.8f, 35.2f, 7.8f, 44.1f, 2.2f
    };

    TEST_ASSERT_EQUAL(temperatures_api_set_temperatures(temperatures), TEMPERATURES_RC_OK);
}

void test_temperatures_get_temperatures(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE] = 2.7f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE] = 18.6f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE] = 22.9f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_RADIATOR_EXIT_TEMPERATURE] = 50.1f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_LEFT_PUMP_EXIT_TEMPERATURE] = 1.7f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE] = 3.3f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_REAR_LEFT_MOTOR_EXIT_TEMPERATURE] = 26.3f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_LEFT_RADIATOR_EXIT_TEMPERATURE] = 5.1f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_TSAC_TEMPERATURE] = 4.4f;
    temperatures_handler.temperatures[TEMPERATURES_NAME_INVERTER_TEMPERATURE] = 12.5f;

    const float *received_temperatures = temperatures_api_get_temperatures();

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(temperatures_handler.temperatures, received_temperatures, TEMPERATURES_NAME_COUNT);
}

void test_temperatures_buffer_round_trip(void) {
    const float expected_temperatures[TEMPERATURES_NAME_COUNT] = {
        .5f, 1.3f, .4f, 3.7f, 23.2f, 44.1f, 5.2f, 8.8f, 93.4f, 1.8f
    };

    temperatures_api_set_temperatures(expected_temperatures);
    const float *received_temperatures = temperatures_api_get_temperatures();

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_temperatures, received_temperatures, TEMPERATURES_NAME_COUNT);
}

void test_temperatures_get_temperature_invalid_name(void) {
    TEST_ASSERT_EQUAL_FLOAT(temperatures_api_get_temperature(TEMPERATURES_NAME_INVALID_NAME), -1.0F);
}

void test_temperatures_get_temperature_right_pump_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE] = 2.7f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE),
                                    2.7f,
                                    "TEMPERATURES_RIGHT_PUMP_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_front_right_motor_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE] = 18.6f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE),
                                    18.6f,
                                    "TEMPERATURES_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_rear_right_motor_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE] = 22.9f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE),
                                    22.9f,
                                    "TEMPERATURES_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_right_radiator_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_RADIATOR_EXIT_TEMPERATURE] = 50.1f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_RIGHT_RADIATOR_EXIT_TEMPERATURE),
                                    50.1f,
                                    "TEMPERATURES_RIGHT_RADIATOR_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_left_pump_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_LEFT_PUMP_EXIT_TEMPERATURE] = 1.7f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_LEFT_PUMP_EXIT_TEMPERATURE),
                                    1.7f,
                                    "TEMPERATURES_LEFT_PUMP_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_front_left_motor_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE] = 3.3f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE),
                                    3.3f,
                                    "TEMPERATURES_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_rear_left_motor_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_REAR_LEFT_MOTOR_EXIT_TEMPERATURE] = 26.3f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_REAR_LEFT_MOTOR_EXIT_TEMPERATURE),
                                    26.3f,
                                    "TEMPERATURES_REAR_LEFT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_left_radiator_exit(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_LEFT_RADIATOR_EXIT_TEMPERATURE] = 5.1f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_LEFT_RADIATOR_EXIT_TEMPERATURE),
                                    5.1f,
                                    "TEMPERATURES_LEFT_RADIATOR_EXIT_TEMPERATURE");
}

void test_temperatures_get_temperature_tsac(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_TSAC_TEMPERATURE] = 4.4f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_TSAC_TEMPERATURE),
                                    4.4f,
                                    "TEMPERATURES_TSAC_TEMPERATURE");
}

void test_temperatures_get_temperature_inverter(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_INVERTER_TEMPERATURE] = 12.5f;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_api_get_temperature(TEMPERATURES_NAME_INVERTER_TEMPERATURE),
                                    12.5f,
                                    "TEMPERATURES_INVERTER_TEMPERATURE");
}

void test_temperatures_set_temperature_invalid_name(void) {
    TEST_ASSERT_EQUAL(temperatures_api_set_temperature(TEMPERATURES_NAME_INVALID_NAME, 4.3f), TEMPERATURES_RC_INVALID_NAME);
}

void test_temperatures_set_temperature_right_pump_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE, 1.4f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE], 1.4f, "TEMPERATURES_RIGHT_PUMP_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_front_right_motor_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE, 3.5f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE], 3.5f, "TEMPERATURES_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_rear_right_motor_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE, 83.4f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE], 83.4f, "TEMPERATURES_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_right_radiator_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_RIGHT_RADIATOR_EXIT_TEMPERATURE, 13.2f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_RADIATOR_EXIT_TEMPERATURE], 13.2f, "TEMPERATURES_RIGHT_RADIATOR_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_left_pump_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_LEFT_PUMP_EXIT_TEMPERATURE, 67.6f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_LEFT_PUMP_EXIT_TEMPERATURE], 67.6f, "TEMPERATURES_LEFT_PUMP_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_front_left_motor_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE, 11.8f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE], 11.8f, "TEMPERATURES_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_rear_left_motor_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_REAR_LEFT_MOTOR_EXIT_TEMPERATURE, 54.6f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_REAR_LEFT_MOTOR_EXIT_TEMPERATURE], 54.6f, "TEMPERATURES_REAR_LEFT_MOTOR_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_left_radiator_exit(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_LEFT_RADIATOR_EXIT_TEMPERATURE, 4.3f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_LEFT_RADIATOR_EXIT_TEMPERATURE], 4.3f, "TEMPERATURES_LEFT_RADIATOR_EXIT_TEMPERATURE");
}

void test_temperatures_set_temperature_tsac(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_TSAC_TEMPERATURE, 2.1f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_TSAC_TEMPERATURE], 2.1f, "TEMPERATURES_TSAC_TEMPERATURE");
}

void test_temperatures_set_temperature_inverter(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_INVERTER_TEMPERATURE, 11.0f);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(temperatures_handler.temperatures[TEMPERATURES_NAME_INVERTER_TEMPERATURE], 11.0f, "TEMPERATURES_INVERTER_TEMPERATURE");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_temperatures_set_temperatures_with_null_pointer_argument);
    RUN_TEST(test_temperatures_set_temperatures_with_valid_argument);

    RUN_TEST(test_temperatures_get_temperatures);
    RUN_TEST(test_temperatures_buffer_round_trip);

    RUN_TEST(test_temperatures_get_temperature_invalid_name);

    RUN_TEST(test_temperatures_get_temperature_right_pump_exit);
    RUN_TEST(test_temperatures_get_temperature_front_right_motor_exit);
    RUN_TEST(test_temperatures_get_temperature_rear_right_motor_exit);
    RUN_TEST(test_temperatures_get_temperature_right_radiator_exit);
    RUN_TEST(test_temperatures_get_temperature_left_pump_exit);
    RUN_TEST(test_temperatures_get_temperature_front_left_motor_exit);
    RUN_TEST(test_temperatures_get_temperature_rear_left_motor_exit);
    RUN_TEST(test_temperatures_get_temperature_left_radiator_exit);
    RUN_TEST(test_temperatures_get_temperature_tsac);
    RUN_TEST(test_temperatures_get_temperature_inverter);

    RUN_TEST(test_temperatures_set_temperature_invalid_name);

    RUN_TEST(test_temperatures_set_temperature_right_pump_exit);
    RUN_TEST(test_temperatures_set_temperature_front_right_motor_exit);
    RUN_TEST(test_temperatures_set_temperature_rear_right_motor_exit);
    RUN_TEST(test_temperatures_set_temperature_right_radiator_exit);
    RUN_TEST(test_temperatures_set_temperature_left_pump_exit);
    RUN_TEST(test_temperatures_set_temperature_front_left_motor_exit);
    RUN_TEST(test_temperatures_set_temperature_rear_left_motor_exit);
    RUN_TEST(test_temperatures_set_temperature_left_radiator_exit);
    RUN_TEST(test_temperatures_set_temperature_tsac);
    RUN_TEST(test_temperatures_set_temperature_inverter);

    return UNITY_END();
}
