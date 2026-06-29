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

constexpr size_t adc_channel_count = 9U;
constexpr size_t adc_temperatures_count = 8U;

constexpr float adc_voltage_reference = 3.3F;
constexpr uint16_t adc_max_value = 4095U;

constexpr unsigned int pullup_resistance_ohm = 10000U;
constexpr unsigned int ntc_reference_resistance_ohm = 10000U;
constexpr unsigned int ntc_beta_coefficient = 3950U;

constexpr float ntc_reference_temperature_kelvin = 298.15F;
constexpr float zero_celsius_in_kelvin = 273.15F;
constexpr float ntc_min_temperature_celsius = -55.0F;
constexpr float ntc_max_temperature_celsius = 150.0F;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

/*!
 * \brief Begin an ADC conversion
 * \note Once the conversion is over, HAL_ADC_ConvCpltCallback is triggered
 */
void adc_start_conversion(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
