/*
 * ffb.c
 *
 * @author: Romain Durand
*/

#include "ffb.h"

#include "sam.h"
#include "printf.h"

#include "usb/reports.h"

static inline void __FFB_ENTER_CRITICAL(void)
{
    NVIC_DisableIRQ(SysTick_IRQn);
}

static inline void __FFB_LEAVE_CRITICAL(void)
{
    NVIC_EnableIRQ(SysTick_IRQn);
}

void ffb_init(void)
{
    SysTick_Config(48000UL);
    NVIC_EnableIRQ(SysTick_IRQn);
}


void SysTick_Handler(void)
{
    PORT->Group[1].OUTTGL.reg = (1 << 30);
}

void ffb_on_set_effect_report(uint8_t const* report)
{
    printf("Set effect report\n");
}

void ffb_on_set_envelope_report(uint8_t const* report)
{

}

void ffb_on_set_condition_report(uint8_t const* report)
{

}

void ffb_on_set_periodic_report(uint8_t const* report)
{

}

void ffb_on_set_constant_force_report(uint8_t const* report)
{

}

void ffb_on_set_ramp_force_report(uint8_t const* report)
{

}

void ffb_on_create_new_effect_report(uint8_t const* report)
{
    printf("Create new effect report\n");
}

void ffb_on_effect_operation_report(uint8_t const* report)
{

}

void ffb_on_pid_block_free_report(uint8_t const* report)
{

}

void ffb_on_device_control_report(uint8_t const* report)
{

}

void ffb_on_device_gain_report(uint8_t const* report)
{

}

