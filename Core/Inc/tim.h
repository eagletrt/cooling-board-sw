/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#include "control.h"

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN Private defines */

/*
 * Actuator PWM tuning.
 *
 * The control module produces a percentage in [0, 1] for every actuator.
 * It is mapped linearly onto [MIN_DUTY, MAX_DUTY] here, so 0% -> MIN_DUTY and
 * 100% -> MAX_DUTY. Fans are wired active-low: the duty written to the pin is
 * (1 - level) when TIM_PWM_FAN_INVERTED is 1.
 */
#define TIM_PWM_FREQUENCY_HZ (10000U)

#define TIM_PWM_PUMP_MIN_DUTY (0.20F)
#define TIM_PWM_PUMP_MAX_DUTY (1.00F)

#define TIM_PWM_FAN_MIN_DUTY (0.25F)
#define TIM_PWM_FAN_MAX_DUTY (1.00F)
#define TIM_PWM_FAN_INVERTED (1)

/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM3_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */

/*!
 * \brief Configure the PWM period and start every actuator channel at 0%.
 * \note Must be called once after MX_TIM3_Init and before tim_pwm_set_control.
 */
enum ControlReturnCode tim_pwm_start(void);

/*!
 * \brief Apply a control percentage to an actuator.
 *
 * \param[in] control_name Actuator to drive.
 * \param[in] percentage   Control effort in [0, 1]; out-of-range values are clamped.
 */
enum ControlReturnCode tim_pwm_set_control(enum ControlName control_name, float percentage);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

