#include <string.h>

#include "temperatures-api.h"
#include "eagletrt-api.h"

/*!
 * \brief Internal module handler
 * \details Hidden from external linkage to enforce API-only access
 */
EAGLETRT_STATIC struct TemperaturesHandler temperatures_handler;

/*!
 * \brief Check whether a temperature name is valid or not
 *
 * \param[in] temperature_name The enumeration type value corresponding to the temperature name
 * \retval true if the temperature name is valid
 * \retval false if the temperature name is not valid
 */
EAGLETRT_STATIC bool prv_temperatures_is_temperature_name_valid(enum TemperaturesName temperature_name) {
    switch (temperature_name) {
        case TEMPERATURES_RIGHT_PUMP_EXIT_TEMPERATURE:
        case TEMPERATURES_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE:
        case TEMPERATURES_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE:
        case TEMPERATURES_RIGHT_RADIATOR_EXIT_TEMPERATURE:
        case TEMPERATURES_LEFT_PUMP_EXIT_TEMPERATURE:
        case TEMPERATURES_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE:
        case TEMPERATURES_REAR_LEFT_MOTOR_EXIT_TEMPERATURE:
        case TEMPERATURES_LEFT_RADIATOR_EXIT_TEMPERATURE:
            return true;
        default:
            return false;
    }
}

enum TemperaturesReturnCode temperatures_api_init(void) {
    memset(&temperatures_handler, 0U, sizeof(temperatures_handler));

    return TEMPERATURES_RC_OK;
}

const float *temperatures_api_get_temperatures(void) {
    return temperatures_handler.temperatures;
}

enum TemperaturesReturnCode temperatures_api_set_temperatures(const float *temperatures) {
    if (temperatures == nullptr) {
        return TEMPERATURES_RC_NULL_POINTER;
    }

    for (size_t temperature_index = 0U; temperature_index < TEMPERATURES_COUNT; ++temperature_index) {
        temperatures_handler.temperatures[temperature_index] = temperatures[temperature_index];
    }

    return TEMPERATURES_RC_OK;
}

float temperatures_api_get_temperature(enum TemperaturesName temperature_name) {
    if (!prv_temperatures_is_temperature_name_valid(temperature_name)) {
        return -1.0F;
    }

    return temperatures_handler.temperatures[temperature_name];
}

enum TemperaturesReturnCode temperatures_api_set_temperature(enum TemperaturesName temperature_name, float temperature_value) {
    if (!prv_temperatures_is_temperature_name_valid(temperature_name)) {
        return TEMPERATURES_RC_INVALID_NAME;
    }

    temperatures_handler.temperatures[temperature_name] = temperature_value;

    return TEMPERATURES_RC_OK;
}
