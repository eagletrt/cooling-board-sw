/*!
 * \file temperatures.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-22
 * \brief Module dedicated to keeping track of the temperatures read from the sensors
 */

#ifndef TEMPERATURES_H
#define TEMPERATURES_H

/*! \brief The number of temperatures that are read from the sensors */
#define TEMPERATURES_COUNT (8U)

/*!
 * \brief Return codes for the temperatures module functions
 */
enum TemperaturesReturnCode {
    TEMPERATURES_RC_OK = 0,       /*!< The function executed succesfully */
    TEMPERATURES_RC_NULL_POINTER, /*!< An argument of pointer type was null or a null pointer was used inside the function */
    TEMPERATURES_RC_ERROR         /*!< The function encountered an error */
};

/*!
 * \brief Structure that is used to handle the temperatures
 */
struct TemperaturesHandler {
    float temperatures[TEMPERATURES_COUNT];
};

#endif // TEMPERATURES_H
