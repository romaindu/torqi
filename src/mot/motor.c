/*
 * mot.c
 *
 * @author: Romain Durand
*/

#include "motor.h"

#include <stdint.h>
#include "samd21.h"
#include "core_cm0plus.h"
#include "com/serial.h"
#include "controller.h"

#define PHASE_A_ADC_IN  4
#define PHASE_B_ADC_IN  5

#define PWM_DT          30
#define PWM_MIN         48
#define ADC_SMPT        1

void motor_init(void)
{
    uint32_t lin0, lin1, bias;
    uint32_t per;

    /* PWM TIMER */
    TCC0->CTRLA.bit.SWRST = 1;
    while (TCC0->SYNCBUSY.bit.SWRST);

    per = 512;

    TCC0->PER.bit.PER  = per;
    TCC0->CC[0].bit.CC = per/2;
    TCC0->CC[1].bit.CC = per/2;
    TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_DSBOTTOM + TCC_WAVE_RAMP_RAMP1 +
                     TCC_WAVE_POL(0b1111) + TCC_WAVE_SWAP(0b1100);
    TCC0->WEXCTRL.reg = TCC_WEXCTRL_OTMX(1) + TCC_WEXCTRL_DTIEN(0xf) +
                        TCC_WEXCTRL_DTLS(PWM_DT) + TCC_WEXCTRL_DTHS(PWM_DT);
    TCC0->DRVCTRL.reg = TCC_DRVCTRL_INVEN(0xf0) +
                        TCC_DRVCTRL_NRV(0x00) + TCC_DRVCTRL_NRE(0xff);

    TCC0->EVCTRL.bit.OVFEO = 1;

    /* ADC */
    ADC->CTRLA.bit.SWRST = 1;
    while (ADC->STATUS.bit.SYNCBUSY);

    /* Set factory calibration values */
    lin0 = (*((uint32_t *)(ADC_FUSES_LINEARITY_0_ADDR)) &
                  ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;
    lin1 = (*((uint32_t *)(ADC_FUSES_LINEARITY_1_ADDR)) &
                  ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos;
    bias = (*((uint32_t *)(ADC_FUSES_BIASCAL_ADDR)) &
            ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
    ADC->CALIB.reg = ADC_CALIB_LINEARITY_CAL(lin0 | (lin1 << 5)) +
                     ADC_CALIB_BIAS_CAL(bias);

    ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV4;
    ADC->EVCTRL.bit.STARTEI = 1;
    ADC->SAMPCTRL.bit.SAMPLEN = ADC_SMPT;
    ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_AREFA;
    ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS_PIN4 + ADC_INPUTCTRL_MUXNEG_GND +
                         ADC_INPUTCTRL_GAIN_DIV2;

    ADC->CTRLA.bit.ENABLE = 1;
    while (ADC->STATUS.bit.SYNCBUSY);

    /* The first conversion is garbage */
    ADC->SWTRIG.bit.START = 1;
    while (!ADC->INTFLAG.bit.RESRDY);
    ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

    /*Interrupt on result ready */
    ADC->INTENSET.bit.RESRDY = 1;
    NVIC_EnableIRQ(ADC_IRQn);

    /* EVENT0: TCC0_OVF -> asynchronous -> ADC_START */
    EVSYS->USER.reg = EVSYS_USER_USER(0x17) + EVSYS_USER_CHANNEL(1);
    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(0) +
                         EVSYS_CHANNEL_EVGEN(0x22) +
                         EVSYS_CHANNEL_PATH_ASYNCHRONOUS;
}

void motor_disable(void)
{
    TCC0->CTRLA.bit.ENABLE = 0;
    while (TCC0->SYNCBUSY.bit.ENABLE);
}

void motor_enable(void)
{
    TCC0->CTRLA.bit.ENABLE = 1;
    while (TCC0->SYNCBUSY.bit.ENABLE);
}

static uint32_t pwm_to_count(int32_t pwm)
{
    uint32_t u;
    uint32_t max = 512 - PWM_MIN;

    if (pwm < 0)
        u = (uint32_t)pwm - 0x80000000U;
    else
        u = (uint32_t)pwm + 0x80000000U;

    u >>= 23;

    if (u < PWM_MIN)
        u = PWM_MIN;
    else if (u > max)
        u = max;

    return u;
}

void ADC_Handler(void)
{
    int32_t pwm;

    ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

    if (ADC->INPUTCTRL.bit.MUXPOS == PHASE_A_ADC_IN) {
        pwm = controller_compute_pwm(PHASE_A, ADC->RESULT.reg);
        TCC0->CCB[0].bit.CCB = pwm_to_count(pwm);
        ADC->INPUTCTRL.bit.MUXPOS = PHASE_B_ADC_IN;
    }
    else {
        pwm = controller_compute_pwm(PHASE_B, ADC->RESULT.reg);
        TCC0->CCB[1].bit.CCB = pwm_to_count(pwm);
        ADC->INPUTCTRL.bit.MUXPOS = PHASE_A_ADC_IN;
    }
}
