/*
 * motor.h
 *
 * Driver module for the conveyor belt DC gearmotor via an IBT2
 * (BTS7960) H-bridge module: 2 PWM channels (RPWM = forward,
 * LPWM = reverse) + 2 GPIO enable pins (R_EN, L_EN).
 *
 * Drop into Core/Inc/. Requires a timer configured with BOTH
 * channels as PWM (CubeMX) and the two enable pins as GPIO_Output.
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t rpwm_channel;   /* PWM channel driving forward (RPWM) */
    uint32_t lpwm_channel;   /* PWM channel driving reverse (LPWM) */

    GPIO_TypeDef *ren_port;  uint16_t ren_pin;   /* R_EN */
    GPIO_TypeDef *len_port;  uint16_t len_pin;   /* L_EN */
} Motor_HandleTypeDef;

/* Call once after MX_GPIO_Init() and MX_TIMx_Init(). Enables R_EN/L_EN
 * and starts both PWM channels at 0% duty. */
void Motor_Init(Motor_HandleTypeDef *hmotor);

/* speed_percent: -100..100. Positive = forward (RPWM), negative =
 * reverse (LPWM), 0 = stop. Only one of RPWM/LPWM is ever driven at a
 * time -- both active together is not safe on IBT2/BTS7960. */
void Motor_SetSpeed(Motor_HandleTypeDef *hmotor, int8_t speed_percent);

/* Coast to stop: both PWM channels to 0%, R_EN/L_EN stay enabled. */
void Motor_Stop(Motor_HandleTypeDef *hmotor);

/* IBT2/BTS7960 has no true short-brake via logic pins alone (each
 * side is an independent half-bridge, not a center-tapped full
 * bridge). This disables the output stage instead (R_EN/L_EN low) --
 * it coasts, same electrical result as Motor_Stop, but also drops
 * the driver's outputs to high-Z. */
void Motor_Disable(Motor_HandleTypeDef *hmotor);

#endif /* MOTOR_H */
