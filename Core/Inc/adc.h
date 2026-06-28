/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */

constexpr size_t ADC_CHANNEL_COUNT = 9U;
constexpr size_t ADC_TEMPERATURES_COUNT = 8U;

constexpr float ADC_VOLTAGE_REFERENCE = 3.3F;
constexpr uint16_t ADC_MAX_VALUE = 4095U;

constexpr unsigned int PULLUP_RESISTANCE_OHM = 10000U;
constexpr unsigned int NTC_REFERENCE_RESISTANCE_OHM = 10000U;
constexpr unsigned int NTC_BETA_COEFFICIENT = 3950U;

constexpr float NTC_REFERENCE_TEMPERATURE_KELVIN = 298.15F;
constexpr float ZERO_CELSIUS_IN_KELVIN = 273.15F;
constexpr float NTC_MIN_TEMPERATURE_CELSIUS = -55.0F;
constexpr float NTC_MAX_TEMPERATURE_CELSIUS = 150.0F;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
