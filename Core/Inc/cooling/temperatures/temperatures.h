/*!
 * \file temperatures.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-22
 * \brief Module dedicated to keeping track of the temperatures read from the sensors
 */

#ifndef TEMPERATURES_H
#define TEMPERATURES_H

#include <stddef.h>

/*! \brief The number of temperatures that are read from the sensors */
constexpr size_t TEMPERATURES_COUNT = 10U;

/*!
 * \brief Return codes for the temperatures module functions
 */
enum TemperaturesReturnCode {
    TEMPERATURES_RC_OK = 0,       /*!< The function executed succesfully */
    TEMPERATURES_RC_NULL_POINTER, /*!< An argument of pointer type was null or a null pointer was used inside the function */
    TEMPERATURES_RC_INVALID_NAME, /*!< The name of the temperature that was provided is not valid */
    TEMPERATURES_RC_ERROR         /*!< The function encountered an error */
};

/*!
 * \brief Names for the temperatures read, one from each of the sensors
 */
enum TemperaturesName {
    TEMPERATURES_RIGHT_PUMP_EXIT_TEMPERATURE,
    TEMPERATURES_FRONT_RIGHT_MOTOR_EXIT_TEMPERATURE,
    TEMPERATURES_REAR_RIGHT_MOTOR_EXIT_TEMPERATURE,
    TEMPERATURES_RIGHT_RADIATOR_EXIT_TEMPERATURE,
    TEMPERATURES_LEFT_PUMP_EXIT_TEMPERATURE,
    TEMPERATURES_FRONT_LEFT_MOTOR_EXIT_TEMPERATURE,
    TEMPERATURES_REAR_LEFT_MOTOR_EXIT_TEMPERATURE,
    TEMPERATURES_LEFT_RADIATOR_EXIT_TEMPERATURE,
    TEMPERATURES_TSAC_TEMPERATURE,
    TEMPERATURES_INVERTER_TEMPERATURE
};

/*!
 * \brief Structure that is used to handle the temperatures
 */
struct TemperaturesHandler {
    float temperatures[TEMPERATURES_COUNT];
};

#endif // TEMPERATURES_H
