/*
 * setup.c
 *
 * @author: Romain Durand
*/

#include "samd21.h"

enum port_group {PORTA, PORTB};

static inline void pin_mux_wrconfig(const enum port_group gr, const int pinmux)
{
    if (pinmux & 0x100000)
        PORT->Group[gr].WRCONFIG.reg =   PORT_WRCONFIG_HWSEL +
                            PORT_WRCONFIG_PMUXEN +
                            PORT_WRCONFIG_WRPMUX +
                            PORT_WRCONFIG_WRPINCFG +
                            PORT_WRCONFIG_PMUX(pinmux) +
                            PORT_WRCONFIG_PINMASK(1 << ((pinmux >> 16) & 0xf));
    else
        PORT->Group[gr].WRCONFIG.reg =   PORT_WRCONFIG_PMUXEN +
                            PORT_WRCONFIG_WRPMUX +
                            PORT_WRCONFIG_WRPINCFG +
                            PORT_WRCONFIG_PMUX(pinmux) +
                            PORT_WRCONFIG_PINMASK(1 << ((pinmux >> 16) & 0xf));
}

void setup_clocks(void)
{
    /* ----------------------- Setup a 48 MHz main clock--------------------- */
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

    /* ------------------------------ SERCOM3 ------------------------------- */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_SERCOM3_CORE;
    PM->APBCMASK.bit.SERCOM3_ = 1;

    /* -------------------------------- USB --------------------------------- */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_USB;

    /* ------------------------------- TCC0 --------------------------------- */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_TCC0_TCC1;
    PM->APBCMASK.bit.TCC0_ = 1;

    /* -------------------------------- ADC --------------------------------- */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN +
                        GCLK_CLKCTRL_GEN_GCLK0 +
                        GCLK_CLKCTRL_ID_ADC;
    PM->APBCMASK.bit.ADC_ = 1;
}

void setup_ports(void)
{
    pin_mux_wrconfig(PORTA, PINMUX_PA22C_SERCOM3_PAD0);
    pin_mux_wrconfig(PORTA, PINMUX_PA23C_SERCOM3_PAD1);

    pin_mux_wrconfig(PORTA, PINMUX_PA24G_USB_DM);
    pin_mux_wrconfig(PORTA, PINMUX_PA25G_USB_DP);
}
