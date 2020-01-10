/*
 * setup.h
 *
 * @author: Romain Durand
*/

#include "sam.h"

void setup_clocks(void)
{
    /* Enable the XOSC32K oscillator */
    SYSCTRL->XOSC32K.reg =  SYSCTRL_XOSC32K_XTALEN +
                            SYSCTRL_XOSC32K_EN32K +
                            SYSCTRL_XOSC32K_STARTUP(3);
    SYSCTRL->XOSC32K.bit.ENABLE = 1;

    /* Wait for oscillator startup */
    while (!(SYSCTRL->PCLKSR.bit.XOSC32KRDY));

    /* Switch GCLK2 to XOSC32K */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN +
                        GCLK_GENCTRL_ID(2) +
                        GCLK_GENCTRL_SRC_XOSC32K;

    /* Route GCLK2 to DFLL48M_REF */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK2 +
                        GCLK_CLKCTRL_ID_DFLL48;

    /* Enable the DFLL */
    SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;

    /* Set mul=1464, cstep=7, fstep=63 */
    SYSCTRL->DFLLMUL.reg =  SYSCTRL_DFLLMUL_MUL(1464) +
                            SYSCTRL_DFLLMUL_FSTEP(63) +
                            SYSCTRL_DFLLMUL_CSTEP(7);

    /* Run in closed loop mode */
    SYSCTRL->DFLLCTRL.bit.MODE = 1;

    /* Wait for DFLL lock fine */
    while (!(SYSCTRL->PCLKSR.bit.DFLLLCKF));

    /* Set one wait state for 48 MHz NVM operation */
    NVMCTRL->CTRLB.bit.RWS = 1;

    /* Set DFLL as GCLK0(main) clk source */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN +
                        GCLK_GENCTRL_ID(0) +
                        GCLK_GENCTRL_SRC_DFLL48M;

    /* OSC8M is by default divided by 8, divide by 1 */
    SYSCTRL->OSC8M.bit.PRESC = 0;

    /* Set OSC8M as GCLK1 clk source */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN +
                        GCLK_GENCTRL_ID(1) +
                        GCLK_GENCTRL_SRC_OSC8M;

    /* SERCOM3 runs on GCLK0 (48MHz) */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_SERCOM3_CORE;
    PM->APBCMASK.bit.SERCOM3_ = 1;

    /* USB runs on GCLK0 (48MHz) */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_USB;
    PM->APBBMASK.bit.USB_ = 1;

    /* TCC0 & TCC1 run on GCLK0 (48MHz) */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_TCC0_TCC1;
    PM->APBCMASK.bit.TCC0_ = 1;

    /* TCC2 & TC3 run on GCLK0 (48MHz) */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_TCC2_TC3;
    PM->APBCMASK.bit.TC3_ = 1;

    /* ADC runs on GCLK1 (8MHz) */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK1 +
                        GCLK_CLKCTRL_ID_ADC;
    PM->APBCMASK.bit.ADC_ = 1;

    /* EIC runs on GCLK0 (48MHz) */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_EIC;
    PM->APBAMASK.bit.EIC_ = 1;

    /* EVSYS */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_EVSYS_1;
    PM->APBCMASK.bit.EVSYS_ = 1;

    /* DAC for FFB debug (32 kHz) */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK2 +
                        GCLK_CLKCTRL_ID_DAC;
    PM->APBCMASK.bit.DAC_ = 1;

    /* Set the USB priority to the minimum */
    NVIC_SetPriority(USB_IRQn, 3);
}

static inline void pin_mux_wrconfig(const int pinmux)
{
    int gr = (pinmux & 0x200000) ? 1:0;
    int co = (pinmux & 0x100000) ? PORT_WRCONFIG_HWSEL:0;

    PORT->Group[gr].WRCONFIG.reg =   co +
                        PORT_WRCONFIG_PMUXEN +
                        PORT_WRCONFIG_WRPMUX +
                        PORT_WRCONFIG_WRPINCFG +
                        PORT_WRCONFIG_PMUX(pinmux) +
                        PORT_WRCONFIG_PINMASK(1 << ((pinmux >> 16) & 0xf));
}

void setup_ports(void)
{
    pin_mux_wrconfig(PINMUX_PA22C_SERCOM3_PAD0);
    pin_mux_wrconfig(PINMUX_PA23C_SERCOM3_PAD1);

    pin_mux_wrconfig(PINMUX_PA24G_USB_DM);
    pin_mux_wrconfig(PINMUX_PA25G_USB_DP);

    pin_mux_wrconfig(PINMUX_PA08E_TCC0_WO0);
    pin_mux_wrconfig(PINMUX_PA09E_TCC0_WO1);
    pin_mux_wrconfig(PINMUX_PA10F_TCC0_WO2);
    pin_mux_wrconfig(PINMUX_PA11F_TCC0_WO3);
    pin_mux_wrconfig(PINMUX_PA14F_TCC0_WO4);
    pin_mux_wrconfig(PINMUX_PA15F_TCC0_WO5);
    pin_mux_wrconfig(PINMUX_PA16F_TCC0_WO6);
    pin_mux_wrconfig(PINMUX_PA17F_TCC0_WO7);

    pin_mux_wrconfig(PINMUX_PA03B_ADC_AIN1);
    pin_mux_wrconfig(PINMUX_PA04B_ADC_AIN4);
    pin_mux_wrconfig(PINMUX_PA05B_ADC_AIN5);

    pin_mux_wrconfig(PINMUX_PA27A_EIC_EXTINT15);
    pin_mux_wrconfig(PINMUX_PA28A_EIC_EXTINT8);

    /* DAC for FFB debug on PA02 */
    pin_mux_wrconfig(PINMUX_PA02B_DAC_VOUT);
}
