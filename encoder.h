/*
 * encoder.h
 *
 * Pulse-counting module for a single-channel IR reflectance sensor
 * (A0/D0/GND/VCC module, LM393-based) reading a striped disc/wheel
 * mounted on the conveyor's output shaft.
 *
 * IMPORTANT: this sensor has only ONE output channel, so it cannot
 * tell direction by itself (a real quadrature encoder has two,
 * 90-degrees apart, for that). Direction here comes from whatever
 * you already commanded the motor to do -- call Encoder_SetDirection()
 * every time you call Motor_SetSpeed(), and this module does the
 * rest.
 *
 * Drop into Core/Inc/. Requires the sensor's D0 pin configured as
 * GPIO_EXTI (CubeMX) with its NVIC interrupt enabled.
 */

#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"

/* The pin the sensor's D0 line is wired to -- update this if you use
 * a different pin than PA4. Kept as one macro so it only needs to
 * match CubeMX's choice in one place. */
#define ENCODER_D0_PIN GPIO_PIN_4

/* Call once after MX_GPIO_Init(). Resets counters -- the GPIO_EXTI +
 * NVIC setup itself is done in CubeMX, not here. */
void Encoder_Init(void);

/* Tell the module which way the motor is currently commanded:
 * dir > 0 = forward, dir < 0 = reverse, 0 = stopped/coasting.
 * Call this alongside Motor_SetSpeed()/Motor_Stop(). */
void Encoder_SetDirection(int8_t dir);

/* Raw edge count since the last reset (always counts up, ignores direction). */
int32_t Encoder_GetPulseCount(void);

/* Signed position in encoder pulses, direction-aware. */
int32_t Encoder_GetPosition(void);

/* Position converted to millimetres of belt travel.
 * pulses_per_rev = number of stripes/slots on your disc (you built it,
 *   so you know this number).
 * wheel_circumference_mm = pi * diameter of the shaft/pulley the disc
 *   is mounted on (measure the pulley the belt actually runs over). */
float Encoder_GetPositionMM(uint16_t pulses_per_rev, float wheel_circumference_mm);

void Encoder_Reset(void);

/* Called internally from HAL_GPIO_EXTI_Callback() (defined in
 * encoder.c) -- you shouldn't need to call this yourself. */
void Encoder_OnPulse(void);

#endif /* ENCODER_H */
