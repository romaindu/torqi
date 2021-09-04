/*
 * motor.c
 *
 * @author: Romain Durand
*/

#include "motor.h"

#include "sam.h"
#include "mot/torqi.h"

#define PWM_DT          30
#define PWM_MIN         100
#define ADC_SMPT        1
#define ADC_HARDLIMIT   1600    // 7.8 amps last chance limit

#define PHASE_A_ADC_IN  4
#define PHASE_B_ADC_IN  5
#define VDIODE_ADC_IN   1

volatile int32_t motor_encoder_count = 0;

void motor_init(void)
{
    uint32_t lin0, lin1, bias;
    const uint32_t per = 512;

    /* DAC for midpoint (1.65V) */
    DAC->CTRLA.bit.SWRST = 1;
    while (DAC->STATUS.bit.SYNCBUSY);

    DAC->CTRLB.reg = DAC_CTRLB_REFSEL_AVCC + DAC_CTRLB_EOEN;
    DAC->DATA.reg = 0x200;
    DAC->CTRLA.bit.ENABLE = 1;
    while (DAC->STATUS.bit.SYNCBUSY);

    /* PWM TIMER */
    TCC0->CTRLA.bit.SWRST = 1;
    while (TCC0->SYNCBUSY.bit.SWRST);

    TCC0->PER.bit.PER  = per;
    TCC0->CC[0].bit.CC = per/2;
    TCC0->CC[1].bit.CC = per/2;
    TCC0->DBGCTRL.bit.FDDBD = 1;
    TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_DSBOTTOM + TCC_WAVE_RAMP_RAMP1 +
                     TCC_WAVE_POL(0b1111) + TCC_WAVE_SWAP(0b1001);
    TCC0->WEXCTRL.reg = TCC_WEXCTRL_OTMX(1) + TCC_WEXCTRL_DTIEN(0xf) +
                        TCC_WEXCTRL_DTLS(PWM_DT) + TCC_WEXCTRL_DTHS(PWM_DT);
    TCC0->DRVCTRL.reg = TCC_DRVCTRL_INVEN(0x00) +
                        TCC_DRVCTRL_NRV(0x00) + TCC_DRVCTRL_NRE(0xff);

    TCC0->EVCTRL.reg = TCC_EVCTRL_OVFEO +
                       TCC_EVCTRL_EVACT0_FAULT + TCC_EVCTRL_TCEI0;

    /* Brake PWM */
    TC3->COUNT16.CTRLA.bit.SWRST = 1;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);

    TC3->COUNT8.PER.reg = 0xff;
    TC3->COUNT8.CC[0].reg = 0;
    TC3->COUNT8.CC[1].reg = 0xff;
    TC3->COUNT8.DBGCTRL.bit.DBGRUN = 1;
    TC3->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 + TC_CTRLA_PRESCALER_DIV64 + TC_CTRLA_WAVEGEN_NPWM;
    TC3->COUNT8.CTRLBSET.reg = TC_CTRLBSET_ONESHOT;
    TC3->COUNT8.CTRLC.reg = TC_CTRLC_INVEN(0b10);
    TC3->COUNT8.CTRLA.bit.ENABLE = 1;
    while (TC3->COUNT8.STATUS.bit.SYNCBUSY);

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

    ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV32 + ADC_CTRLB_DIFFMODE;
    ADC->WINLT.reg = -ADC_HARDLIMIT;
    ADC->WINUT.reg = +ADC_HARDLIMIT;
    ADC->WINCTRL.reg = ADC_WINCTRL_WINMODE_MODE4;
    ADC->EVCTRL.reg = ADC_EVCTRL_STARTEI + ADC_EVCTRL_WINMONEO;
    ADC->SAMPCTRL.bit.SAMPLEN = ADC_SMPT;
    ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INT1V;
    ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS_PIN4 + ADC_INPUTCTRL_MUXNEG_PIN0 +
                         ADC_INPUTCTRL_GAIN_DIV2;

    ADC->CTRLA.bit.ENABLE = 1;
    while (ADC->STATUS.bit.SYNCBUSY);

    /* The first conversion is garbage */
    ADC->SWTRIG.bit.START = 1;
    while (!ADC->INTFLAG.bit.RESRDY);
    ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

    /*Interrupt on result ready */
    NVIC_EnableIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn, 0);
    ADC->INTENSET.reg = ADC_INTENSET_RESRDY;

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

    /* AC for power supply monitoring */
    AC->CTRLA.bit.SWRST = 1;
    while (AC->STATUSB.bit.SYNCBUSY);
    AC->WINCTRL.bit.WEN0 = 1;
    AC->COMPCTRL[0].reg = AC_COMPCTRL_ENABLE + AC_COMPCTRL_HYST +
                          AC_COMPCTRL_MUXPOS_PIN2 + AC_COMPCTRL_MUXNEG_VSCALE;
    AC->COMPCTRL[1].reg = AC_COMPCTRL_ENABLE + AC_COMPCTRL_HYST +
                          AC_COMPCTRL_MUXPOS_PIN2 + AC_COMPCTRL_MUXNEG_VSCALE;
    AC->SCALER[0].bit.VALUE = 48; // 1.5V -> 15V
    AC->SCALER[1].bit.VALUE = 29; // 2.5V -> 35V
    AC->CTRLA.bit.ENABLE = 1;

    /* EIC (encoder) */
    EIC->CTRL.bit.SWRST = 1;
    while (EIC->STATUS.bit.SYNCBUSY);

    EIC->CONFIG[1].reg = EIC_CONFIG_SENSE0_BOTH + EIC_CONFIG_SENSE7_BOTH;
    EIC->INTENSET.reg = EIC_INTENSET_EXTINT8 + EIC_INTENSET_EXTINT15;
    EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT8 + EIC_INTFLAG_EXTINT15;
    NVIC_EnableIRQ(EIC_IRQn);
    NVIC_SetPriority(EIC_IRQn, 0);
    EIC->CTRL.bit.ENABLE = 1;
}

int motor_powergood(void)
{
    return (AC->STATUSA.bit.WSTATE0 == AC_STATUSA_WSTATE0_INSIDE_Val);
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

static uint32_t _map_timer(int32_t pwm)
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

static void _brake(int32_t adc)
{
    static const uint8_t TCPER = 42;
    static const int32_t BRAKE_GAIN = 4;

    uint32_t u = 0;

    if (adc > 0) {
        u = BRAKE_GAIN*adc;
    }

    if (u < 0)
        u = 0;
    else if (u > TCPER)
        u = TCPER;

    TC3->COUNT8.PER.reg = TCPER;
    TC3->COUNT8.CC[0].reg = u;
    TC3->COUNT8.CC[1].reg = TCPER+1-u;
    TC3->COUNT8.CTRLBSET.reg = TC_CTRLBSET_CMD_RETRIGGER;
}

void ADC_Handler(void)
{
    int32_t adc, pwm;

    PORT->Group[1].OUTSET.reg = (1 << 0);

    adc = *(int16_t*)(&ADC->RESULT.reg);
    ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

    if (ADC->INPUTCTRL.bit.MUXPOS == PHASE_A_ADC_IN) {
        pwm = torqi_sample(PHASE_A, adc);
        TCC0->CCB[0].bit.CCB = _map_timer(-pwm);
        ADC->INPUTCTRL.bit.MUXPOS = PHASE_B_ADC_IN;
    }
    else if (ADC->INPUTCTRL.bit.MUXPOS == PHASE_B_ADC_IN) {
        pwm = torqi_sample(PHASE_B, adc);
        TCC0->CCB[1].bit.CCB = _map_timer(pwm);
        ADC->INPUTCTRL.bit.MUXPOS = VDIODE_ADC_IN;
    }
    else { //if (ADC->INPUTCTRL.bit.MUXPOS == VDIODE_ADC_IN) {
        _brake(adc);
        ADC->INPUTCTRL.bit.MUXPOS = PHASE_A_ADC_IN;
    }

    PORT->Group[1].OUTCLR.reg = (1 << 0);
}

void EIC_Handler(void)
{
    PORT->Group[1].OUTSET.reg = (1 << 0);

    static uint8_t enc_state = 0;
    const int8_t enc_table[16] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};
    int8_t s;

    EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT8 + EIC_INTFLAG_EXTINT15;

    enc_state <<= 2;
    enc_state |= (PORT->Group[0].IN.reg & (PORT_PA27 + PORT_PA28)) >> PIN_PA27;
    s = enc_table[enc_state & 0b1111];

    torqi_count(s);
    motor_encoder_count += s;

    PORT->Group[1].OUTCLR.reg = (1 << 0);
}
