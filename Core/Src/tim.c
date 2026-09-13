/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

#include "eagletrt.h"

#include <math.h>
#include <stdbool.h>

/* USER CODE END 0 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 4799;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PB5     ------> TIM3_CH3
    PB6     ------> TIM3_CH2
    PB7     ------> TIM3_CH4
    PB8     ------> TIM3_CH1
    */
    GPIO_InitStruct.Pin = PUMP_L_CMD_Pin|FAN_L_CMD_Pin|FAN_R_CMD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PUMP_R_CMD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_TIM3;
    HAL_GPIO_Init(PUMP_R_CMD_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

EAGLETRT_STATIC const uint32_t control_to_tim_channel_map[CONTROL_NAME_COUNT] = {
    [CONTROL_NAME_LEFT_PUMP] = TIM_CHANNEL_3,
    [CONTROL_NAME_LEFT_FAN] = TIM_CHANNEL_4,
    [CONTROL_NAME_RIGHT_PUMP] = TIM_CHANNEL_2,
    [CONTROL_NAME_RIGHT_FAN] = TIM_CHANNEL_1
};

/*!
 * \brief Map a control percentage to the duty cycle to write on the pin.
 */
EAGLETRT_STATIC float prv_tim_pwm_percentage_to_duty(enum ControlName control_name, float percentage) {
    // NaN compares false against everything, so this also turns NaN into 0
    if (!(percentage >= 0.F)) {
        percentage = 0.F;
    } else if (percentage > 1.F) {
        percentage = 1.F;
    }

    float level;
    bool inverted;
    switch (control_name) {
        case CONTROL_NAME_LEFT_PUMP:
        case CONTROL_NAME_RIGHT_PUMP:
            level = TIM_PWM_PUMP_MIN_DUTY + percentage * (TIM_PWM_PUMP_MAX_DUTY - TIM_PWM_PUMP_MIN_DUTY);
            inverted = false;
            break;
        case CONTROL_NAME_LEFT_FAN:
        case CONTROL_NAME_RIGHT_FAN:
        default:
            level = TIM_PWM_FAN_MIN_DUTY + percentage * (TIM_PWM_FAN_MAX_DUTY - TIM_PWM_FAN_MIN_DUTY);
            inverted = (TIM_PWM_FAN_INVERTED != 0);
            break;
    }

    return inverted ? (1.F - level) : level;
}

enum ControlReturnCode tim_pwm_start(void) {
    // ARR = f_tim / (f_pwm * (PSC + 1)) - 1
    // TIM3 sits on APB; with the APB prescaler at DIV1 the timer clock equals PCLK1.
    const uint32_t timer_clk = HAL_RCC_GetPCLK1Freq();
    const uint32_t psc = htim3.Instance->PSC;
    const uint32_t arr = (timer_clk / (TIM_PWM_FREQUENCY_HZ * (psc + 1U))) - 1U;
    __HAL_TIM_SET_AUTORELOAD(&htim3, arr);

    enum ControlReturnCode return_code = CONTROL_RC_OK;
    for (uint8_t control_name = 0; control_name < CONTROL_NAME_COUNT; ++control_name) {
        if (tim_pwm_set_control((enum ControlName)control_name, 0.F) != CONTROL_RC_OK) {
            return_code = CONTROL_RC_ERROR;
        }
        if (HAL_TIM_PWM_Start(&htim3, control_to_tim_channel_map[control_name]) != HAL_OK) {
            return_code = CONTROL_RC_ERROR;
        }
    }

    // load ARR and the preloaded CCRs once; afterwards CCR updates land at the next period
    htim3.Instance->EGR = TIM_EGR_UG;

    return return_code;
}

enum ControlReturnCode tim_pwm_set_control(enum ControlName control_name, float percentage) {
    if (control_name >= CONTROL_NAME_COUNT) {
        return CONTROL_RC_INVALID_NAME;
    }

    const float duty = prv_tim_pwm_percentage_to_duty(control_name, percentage);

    // PWM1: output high while CNT < CCR, so CCR = ARR + 1 is a solid 100%
    const uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3);
    const uint32_t pulse = (uint32_t)lroundf(duty * (float)(arr + 1U));
    __HAL_TIM_SET_COMPARE(&htim3, control_to_tim_channel_map[control_name], pulse);

    return CONTROL_RC_OK;
}

/* USER CODE END 1 */

