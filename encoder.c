/*
 * encoder.c
 *
 * Implementation for encoder.h — see that file for usage notes.
 *
 * NOTE: this file owns the project's HAL_GPIO_EXTI_Callback(). If you
 * add another EXTI-driven input later (a push button, a limit
 * switch, etc.), add its own "else if (GPIO_Pin == ...)" branch
 * below rather than defining HAL_GPIO_EXTI_Callback a second time
 * elsewhere -- only one definition can exist in the whole project.
 */

#include "encoder.h"

static volatile int32_t s_pulseCount = 0;
static volatile int32_t s_position   = 0;
static volatile int8_t  s_direction  = 0;

void Encoder_Init(void)
{
    s_pulseCount = 0;
    s_position   = 0;
    s_direction  = 0;
}

void Encoder_SetDirection(int8_t dir)
{
    if (dir > 0)      s_direction = 1;
    else if (dir < 0) s_direction = -1;
    else              s_direction = 0;
}

void Encoder_OnPulse(void)
{
    s_pulseCount++;
    s_position += s_direction;
}

int32_t Encoder_GetPulseCount(void)
{
    return s_pulseCount;
}

int32_t Encoder_GetPosition(void)
{
    return s_position;
}

float Encoder_GetPositionMM(uint16_t pulses_per_rev, float wheel_circumference_mm)
{
    return ((float)s_position / (float)pulses_per_rev) * wheel_circumference_mm;
}

void Encoder_Reset(void)
{
    s_pulseCount = 0;
    s_position   = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ENCODER_D0_PIN)
    {
        Encoder_OnPulse();
    }
}
