#include <string.h>
#include <unity.h>
#include "temperatures-api.h"

extern struct TemperaturesHandler temperatures_handler;

constexpr int temperatures_invalid_name = -1;

void setUp(void) {
    memset(&temperatures_handler, 0U, sizeof(temperatures_handler));
}

void test_temperatures_initialization_to_zero(void) {
    struct TemperaturesHandler dummy_zero_handler = { 0 };

    memset(&temperatures_handler, 0xFF, sizeof(temperatures_handler));

    TEST_ASSERT_EQUAL(TEMPERATURES_RC_OK, temperatures_api_init());
    TEST_ASSERT_EQUAL_MEMORY(&dummy_zero_handler, &temperatures_handler, sizeof(temperatures_handler));
}

void test_temperatures_get_temperature_invalid_name(void) {
    TEST_ASSERT_EQUAL_FLOAT(temperatures_api_get_temperature(temperatures_invalid_name), -420.0F);
}

void test_temperatures_get_temperature_valid_name(void) {
    temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE] = 2.7F;

    TEST_ASSERT_EQUAL_FLOAT(temperatures_api_get_temperature(TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE),
                            2.7F);
}

void test_temperatures_set_temperature_invalid_name(void) {
    TEST_ASSERT_EQUAL(temperatures_api_set_temperature(temperatures_invalid_name, 4.3F), TEMPERATURES_RC_INVALID_NAME);
}

void test_temperatures_set_temperature_valid_name(void) {
    temperatures_api_set_temperature(TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE, 1.4F);

    TEST_ASSERT_EQUAL_FLOAT(temperatures_handler.temperatures[TEMPERATURES_NAME_RIGHT_PUMP_EXIT_TEMPERATURE],
                            1.4F);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_temperatures_initialization_to_zero);

    RUN_TEST(test_temperatures_get_temperature_invalid_name);
    RUN_TEST(test_temperatures_get_temperature_valid_name);

    RUN_TEST(test_temperatures_set_temperature_invalid_name);
    RUN_TEST(test_temperatures_set_temperature_valid_name);

    return UNITY_END();
}
