#include <string.h>
#include <unity.h>
#include "temperatures-api.h"

extern struct TemperaturesHandler temperatures_handler;

constexpr int TEMPERATURES_NAME_INVALID_NAME = -1;

void setUp(void) {
    memset(&temperatures_handler, 0, sizeof(temperatures_handler));
}

void test_temperatures_get_temperature_invalid_name(void) {
    TEST_ASSERT_EQUAL_FLOAT(temperatures_api_get_temperature(TEMPERATURES_NAME_INVALID_NAME), -420.0F);
}

void test_temperatures_get_temperature_valid_name(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE] = 2.7F;

    TEST_ASSERT_EQUAL_FLOAT(temperatures_api_get_temperature(TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE),
                            2.7F);
}

void test_temperatures_set_temperature_invalid_name(void) {
    TEST_ASSERT_EQUAL(temperatures_api_set_temperature(TEMPERATURES_NAME_INVALID_NAME, 4.3F), TEMPERATURES_RC_INVALID_NAME);
}

void test_temperatures_set_temperature_valid_name(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE, 1.4F);

    TEST_ASSERT_EQUAL_FLOAT(temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE],
                            1.4F);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_temperatures_get_temperature_invalid_name);
    RUN_TEST(test_temperatures_get_temperature_valid_name);

    RUN_TEST(test_temperatures_set_temperature_invalid_name);
    RUN_TEST(test_temperatures_set_temperature_valid_name);

    return UNITY_END();
}
