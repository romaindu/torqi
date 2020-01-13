/*
 * motor.c
 *
 * @author: Romain Durand
*/

#include "motor.h"

#include "sam.h"
#include "torque.h"

#define PWM_DT          30
#define PWM_MIN         100
#define ADC_SMPT        6
#define ADC_LIMIT       1966

#define PHASE_A_ADC_IN  4
#define PHASE_B_ADC_IN  5
#define ISUPPLY_ADC_IN  6

volatile int32_t encoder_count = 0;

void motor_init(void)
{
    uint32_t lin0, lin1, bias;
    const uint32_t per = 512;

    /* PWM TIMER */
    TCC0->CTRLA.bit.SWRST = 1;
    while (TCC0->SYNCBUSY.bit.SWRST);

    TCC0->PER.bit.PER  = per;
    TCC0->CC[0].bit.CC = per/2;
    TCC0->CC[1].bit.CC = per/2;
    TCC0->DBGCTRL.bit.FDDBD = 1;
    TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_DSBOTTOM + TCC_WAVE_RAMP_RAMP1 +
                     TCC_WAVE_POL(0b1111) + TCC_WAVE_SWAP(0b1100);
    TCC0->WEXCTRL.reg = TCC_WEXCTRL_OTMX(1) + TCC_WEXCTRL_DTIEN(0xf) +
                        TCC_WEXCTRL_DTLS(PWM_DT) + TCC_WEXCTRL_DTHS(PWM_DT);
    TCC0->DRVCTRL.reg = TCC_DRVCTRL_INVEN(0xf0) +
                        TCC_DRVCTRL_NRV(0x00) + TCC_DRVCTRL_NRE(0xff);

    TCC0->EVCTRL.reg = TCC_EVCTRL_OVFEO +
                       TCC_EVCTRL_EVACT0_FAULT + TCC_EVCTRL_TCEI0;

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

    ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV32;
    ADC->WINLT.reg = 2048 - ADC_LIMIT;
    ADC->WINUT.reg = 2048 + ADC_LIMIT;
    ADC->WINCTRL.reg = ADC_WINCTRL_WINMODE_MODE4;
    ADC->EVCTRL.reg = ADC_EVCTRL_STARTEI + ADC_EVCTRL_WINMONEO;
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

    /* EVENT1: ADC_WINMON -> resynchronized -> TCC0_EV0 */
    EVSYS->USER.reg = EVSYS_USER_USER(0x04) + EVSYS_USER_CHANNEL(2);
    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(1) +
                         EVSYS_CHANNEL_EVGEN(0x43) +
                         EVSYS_CHANNEL_EDGSEL_RISING_EDGE +
                         EVSYS_CHANNEL_PATH_RESYNCHRONIZED;

    /* EIC (encoder) */
    EIC->CTRL.bit.SWRST = 1;
    while (EIC->STATUS.bit.SYNCBUSY);

    EIC->CONFIG[1].reg = EIC_CONFIG_SENSE0_BOTH + EIC_CONFIG_SENSE7_BOTH;
    EIC->INTENSET.reg = EIC_INTENSET_EXTINT8 + EIC_INTENSET_EXTINT15;
    EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT8 + EIC_INTFLAG_EXTINT15;
    NVIC_EnableIRQ(EIC_IRQn);
    EIC->CTRL.bit.ENABLE = 1;
}

void motor_fault(void)
{
    /* Generate a software event to trigger a non recoverable fault */
    EVSYS->CHANNEL.reg = EVSYS_CHANNEL_CHANNEL(1) +
                         EVSYS_CHANNEL_EVGEN(0x43) +
                         EVSYS_CHANNEL_EDGSEL_RISING_EDGE +
                         EVSYS_CHANNEL_PATH_RESYNCHRONIZED +
                         EVSYS_CHANNEL_SWEVT;
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

int32_t motor_encoder_read(void)
{
    return encoder_count;
}

void motor_encoder_write(int32_t val)
{
    encoder_count = val;
}

static uint32_t map_timer_count(int32_t pwm)
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

    PORT->Group[1].OUTSET.reg = (1 << 0);

    ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

    if (ADC->INPUTCTRL.bit.MUXPOS == PHASE_A_ADC_IN) {
        pwm = torque_on_adc_sample(PHASE_A, ADC->RESULT.reg);
        TCC0->CCB[0].bit.CCB = map_timer_count(pwm);
        ADC->INPUTCTRL.bit.MUXPOS = PHASE_B_ADC_IN;
    }
    else if (ADC->INPUTCTRL.bit.MUXPOS == PHASE_B_ADC_IN) {
        pwm = torque_on_adc_sample(PHASE_B, ADC->RESULT.reg);
        TCC0->CCB[1].bit.CCB = map_timer_count(pwm);
        ADC->INPUTCTRL.bit.MUXPOS = ISUPPLY_ADC_IN;
    }
    else if (ADC->INPUTCTRL.bit.MUXPOS == ISUPPLY_ADC_IN) {
        (int32_t)ADC->RESULT.reg;
        ADC->INPUTCTRL.bit.MUXPOS = PHASE_A_ADC_IN;
    }

    PORT->Group[1].OUTCLR.reg = (1 << 0);
}

void EIC_Handler(void)
{
    PORT->Group[1].OUTSET.reg = (1 << 0);

    static uint8_t enc_state = 0;
    const int8_t enc_table[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    int8_t s;

    EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT8 + EIC_INTFLAG_EXTINT15;

    enc_state <<= 2;
    enc_state |= (PORT->Group[0].IN.reg & (PORT_PA27 + PORT_PA28)) >> PIN_PA27;
    s = enc_table[enc_state & 0b1111];

    torque_on_encoder_count(s);
    encoder_count += s;

    PORT->Group[1].OUTCLR.reg = (1 << 0);
}
