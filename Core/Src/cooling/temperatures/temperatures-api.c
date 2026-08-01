/*!
 * \file temperatures-api.c
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-22
 * \brief Implementation of the APIs of the module dedicated to keeping track of the temperatures read from the sensors
 */

#include <string.h>

#include "temperatures-api.h"
#include "eagletrt-api.h"

constexpr float temperatures_invalid_temperature = -420.0F;

/*!
 * \brief Internal module handler
 * \details Hidden from external linkage to enforce API-only access
 */
EAGLETRT_STATIC struct TemperaturesHandler temperatures_handler;

enum TemperaturesReturnCode temperatures_api_init(void) {
    memset(&temperatures_handler, 0U, sizeof(temperatures_handler));

    return TEMPERATURES_RC_OK;
}

float temperatures_api_get_temperature(enum TemperaturesName temperature_name) {
    if (temperature_name >= TEMPERATURES_NAME_COUNT) {
        return temperatures_invalid_temperature;
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
