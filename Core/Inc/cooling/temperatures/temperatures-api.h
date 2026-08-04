/*!
 * \file temperatures-api.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-22
 * \brief Module dedicated to keeping track of the temperatures read from the sensors
 */

#ifndef TEMPERATURES_API_H
#define TEMPERATURES_API_H

#include "temperatures.h"

/*!
 * \brief Initialize the temperatures module
 *
 * \retval TEMPERATURES_RC_OK if the module was successfully initialized
 */
enum TemperaturesReturnCode temperatures_api_init(void);

/*!
 * \brief Get the value of one of the temperatures contained in the buffer of the handler
 *
 * \param[in] temperature_name The enumeration type value corresponding to the name of the desired temperature
 *
 * \return The value of the temperature
 *
 * \note Returns an invalid temperature value (-420) if temperature_name is invalid
 */
float temperatures_api_get_temperature(enum TemperaturesName temperature_name);

/*!
 * \brief Set the value of one of the temperatures contained in the buffer of the handler
 *
 * \param[in] temperature_name The enumeration type value corresponding to the name of the desired temperature
 * \param[in] value The value of the temperature
 *
 * \retval TEMPERATURES_RC_INVALID_NAME if tempearture_name is invalid
 * \retval TEMPERATURES_RC_OK if the value was correctly set
 */
enum TemperaturesReturnCode temperatures_api_set_temperature(enum TemperaturesName temperature_name, float temperature_value);

/*!
 * \brief Get the status of the temperatures
 *
 * \return The status of the temperatures (whether they are updated or not) as a bool
 */
bool temperatures_api_get_temperatures_status(void);

#endif // TEMPERATURES_API_H
