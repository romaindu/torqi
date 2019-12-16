/*
 * mot.c
 *
 * @author: Romain Durand
*/

#include "mot.h"

#include <stdint.h>
#include "samd21.h"
#include "core_cm0plus.h"
#include "com/serial.h"

static const uint8_t  PWM_RES = 9;
static const uint16_t PWM_DT  = 30;
static const uint16_t PWM_MIN = 48;
static const uint8_t  ADC_SMPT = 1;

static uint32_t fixedpt_to_ures(int32_t pwm, uint8_t res)
{
    uint32_t u;

    if (pwm < 0)
        u = (uint32_t)pwm - 0x80000000U;
    else
        u = (uint32_t)pwm + 0x80000000U;

    return u >> (32 - res);
}

static uint32_t split_cycle(uint32_t pwm, uint32_t lmin, uint32_t rmin)
{
    uint32_t r;

    if (pwm < lmin)
        r = lmin;
    else if (pwm > rmin)
        r = rmin;
    else
        r = pwm;

    return r; 
}

void mot_pwm(uint32_t pwm)
{
    uint32_t a, b, u;

    u = fixedpt_to_ures(pwm, PWM_RES);
    putr32(u);

    //TCC0->CCB[1].bit.CC = 
    //TCC0->CCB[3].bit.CC = 
}

void mot_init(void)
{
    uint32_t lin0, lin1, bias;
    uint32_t per;

    /* PWM TIMER */
    TCC0->CTRLA.bit.SWRST = 1;
    while (TCC0->SYNCBUSY.bit.SWRST);

    per = 1 << (PWM_RES);

    TCC0->PER.bit.PER  = per;
    TCC0->CC[0].bit.CC = PWM_MIN;
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

void mot_disable(void)
{
    TCC0->CTRLA.bit.ENABLE = 0;
    while (TCC0->SYNCBUSY.bit.ENABLE);
}

void mot_enable(void)
{
    TCC0->CTRLA.bit.ENABLE = 1;
    while (TCC0->SYNCBUSY.bit.ENABLE);
}

void ADC_Handler(void)
{
    PORT->Group[1].OUTSET.reg = (1 << 17);
    ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;
    PORT->Group[1].OUTCLR.reg = (1 << 17);
}
