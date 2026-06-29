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
 * \brief Get a pointer to the temperatures buffer of the handler
 *
 * \return A pointer to const float containing the base address of the buffer
 */
const float *temperatures_api_get_temperatures(void);

/*!
 * \brief Set the temperatures buffer contained in the handler by copying over the values from an array taken as a parameter
 *
 * \param[in] temperatures A pointer to an buffer containing the temperatures, assumed to be TEMPERATURES_NAME_COUNT in length
 *
 * \retval TEMPERATURES_RC_NULL_POINTER if the provided buffer is null
 * \retval TEMPERATURES_RC_OK if the values of the input buffer are successfully transferred to the buffer of the handler
 */
enum TemperaturesReturnCode temperatures_api_set_temperatures(const float temperatures[TEMPERATURES_NAME_COUNT]);

/*!
 * \brief Get the value of one of the temperatures contained in the buffer of the handler
 *
 * \param[in] temperature_name The enumeration type value corresponding to the name of the desired temperature
 * \return The value of the temperature
 * \note Returns -1.0 if temperature_name is invalid
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

#endif // TEMPERATURES_API_H
