/*
 * mot.c
 *
 * @author: Romain Durand
*/

#include "mot.h"

#include <stdint.h>
#include "samd21.h"

#define PWM_DT          10
#define ADC_SMPTIME     5

void mot_init(void)
{
    int lin0, lin1, bias;

    /* EVENT: channel 0: TCC0_TRIG -> ADC_START (async) */
    EVSYS->USER.reg = EVSYS_USER_CHANNEL(0) + EVSYS_USER_USER(0x17);
    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(0) +
                         EVSYS_CHANNEL_EVGEN(0x23) +
                         EVSYS_CHANNEL_PATH_ASYNCHRONOUS;

    /* PWM TIMER */
    TCC0->CTRLA.bit.SWRST = 1;
    while (TCC0->SYNCBUSY.bit.SWRST);

    TCC0->PER.bit.PER = 511;
    TCC0->CC[0].bit.CC = 255;
    TCC0->CC[1].bit.CC = 255;
    TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_DSBOTH + TCC_WAVE_RAMP_RAMP1 +
                     TCC_WAVE_POL1 + TCC_WAVE_SWAP0 + TCC_WAVE_SWAP3;
    TCC0->WEXCTRL.reg = TCC_WEXCTRL_OTMX(1) + TCC_WEXCTRL_DTIEN(0xf) +
                        TCC_WEXCTRL_DTLS(PWM_DT) + TCC_WEXCTRL_DTHS(PWM_DT);
    TCC0->DRVCTRL.reg = TCC_DRVCTRL_INVEN(0xf0) +
                        TCC_DRVCTRL_NRV(0x00) + TCC_DRVCTRL_NRE(0xff);


    /* ADC */
    ADC->CTRLA.bit.SWRST = 1;
    while (ADC->STATUS.bit.SYNCBUSY);

    ADC->CTRLA.bit.ENABLE = 1;
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

    ADC->SAMPCTRL.bit.SAMPLEN = ADC_SMPTIME;
    ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_AREFA;
    ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS_PIN4 + ADC_INPUTCTRL_MUXNEG_GND +
                         ADC_INPUTCTRL_GAIN_DIV2;

    /* Start conversion on event and interrupt on result ready */
    ADC->EVCTRL.bit.STARTEI = 1;
    ADC->INTENSET.bit.RESRDY = 1;

    /* The first conversion is garbage */
    ADC->SWTRIG.bit.START = 1;
    while (!ADC->INTFLAG.bit.RESRDY);
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
