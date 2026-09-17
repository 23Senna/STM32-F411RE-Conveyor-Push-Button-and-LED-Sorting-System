/*
 * motor.c
 *
 * Implementation for motor.h — see that file for usage notes.
 */

#include "motor.h"

void Motor_Init(Motor_HandleTypeDef *hmotor)
{
    __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->rpwm_channel, 0);
    __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->lpwm_channel, 0);

    HAL_TIM_PWM_Start(hmotor->htim, hmotor->rpwm_channel);
    HAL_TIM_PWM_Start(hmotor->htim, hmotor->lpwm_channel);

    HAL_GPIO_WritePin(hmotor->ren_port, hmotor->ren_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hmotor->len_port, hmotor->len_pin, GPIO_PIN_SET);
}

void Motor_SetSpeed(Motor_HandleTypeDef *hmotor, int8_t speed_percent)
{
    if (speed_percent > 100)  speed_percent = 100;
    if (speed_percent < -100) speed_percent = -100;

    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(hmotor->htim);

    if (speed_percent >= 0) {
        uint32_t duty = ((uint32_t)speed_percent * (arr + 1)) / 100;
        __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->rpwm_channel, duty);
        __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->lpwm_channel, 0);
    } else {
        uint32_t duty = ((uint32_t)(-speed_percent) * (arr + 1)) / 100;
        __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->rpwm_channel, 0);
        __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->lpwm_channel, duty);
    }
}

void Motor_Stop(Motor_HandleTypeDef *hmotor)
{
    __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->rpwm_channel, 0);
    __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->lpwm_channel, 0);
}

void Motor_Disable(Motor_HandleTypeDef *hmotor)
{
    Motor_Stop(hmotor);
    HAL_GPIO_WritePin(hmotor->ren_port, hmotor->ren_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hmotor->len_port, hmotor->len_pin, GPIO_PIN_RESET);
}
