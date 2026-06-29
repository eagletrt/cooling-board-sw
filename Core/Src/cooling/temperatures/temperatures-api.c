#include <string.h>

#include "temperatures-api.h"
#include "eagletrt-api.h"

/*!
 * \brief Internal module handler
 * \details Hidden from external linkage to enforce API-only access
 */
EAGLETRT_STATIC struct TemperaturesHandler temperatures_handler;

enum TemperaturesReturnCode temperatures_api_init(void) {
    memset(&temperatures_handler, 0U, sizeof(temperatures_handler));

    return TEMPERATURES_RC_OK;
}

const float *temperatures_api_get_temperatures(void) {
    return temperatures_handler.temperatures;
}

enum TemperaturesReturnCode temperatures_api_set_temperatures(const float temperatures[TEMPERATURES_NAME_COUNT]) {
    if (temperatures == nullptr) {
        return TEMPERATURES_RC_NULL_POINTER;
    }

    for (size_t temperature_index = 0U; temperature_index < (size_t)TEMPERATURES_NAME_COUNT; ++temperature_index) {
        temperatures_handler.temperatures[temperature_index] = temperatures[temperature_index];
    }

    return TEMPERATURES_RC_OK;
}

float temperatures_api_get_temperature(enum TemperaturesName temperature_name) {
    if (temperature_name >= TEMPERATURES_NAME_COUNT) {
        return -1.0F;
    }

    return temperatures_handler.temperatures[temperature_name];
}

enum TemperaturesReturnCode temperatures_api_set_temperature(enum TemperaturesName temperature_name, float temperature_value) {
    if (temperature_name >= TEMPERATURES_NAME_COUNT) {
        return TEMPERATURES_RC_INVALID_NAME;
    }

    temperatures_handler.temperatures[temperature_name] = temperature_value;

    return TEMPERATURES_RC_OK;
}
