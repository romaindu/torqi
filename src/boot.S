/*
 * Boot sequence
 *
 * @author: Romain Durand
*/

.section 	.vectors
_vtable:
	.long	Reset_Handler
	.long	NMI_Handler
	.long	HardFault_Handler
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	SVCall_Handler
	.long 	0
	.long 	0
	.long 	PendSV_Handler
	.long 	SysTick_Handler
	.long 	PM_Handler
	.long 	SYSCTRL_Handler
	.long 	WDT_Handler
	.long 	RTC_Handler
	.long 	EIC_Handler
	.long 	NVMCTRL_Handler
	.long 	DMAC_Handler
	.long 	USB_Handler
	.long 	EVSYS_Handler
	.long 	SERCOM0_Handler
	.long 	SERCOM1_Handler
	.long 	SERCOM2_Handler
	.long 	SERCOM3_Handler
	.long 	SERCOM4_Handler
	.long 	SERCOM5_Handler
	.long 	TCC0_Handler
	.long 	TCC1_Handler
	.long 	TCC2_Handler
	.long 	TC3_Handler
	.long 	TC4_Handler
	.long 	TC5_Handler
	.long 	TC6_Handler
	.long 	TC7_Handler
	.long 	ADC_Handler
	.long 	AC_Handler
	.long 	DAC_Handler
	.long 	PTC_Handler
	.long 	I2S_Handler

.text

_init0:

_crt0:
	/* Copy .data & .bss sections */
	ldr		r0, =__etext
	ldr		r1, =__data
	ldr		r2, =__edata
	b		1f
0:	ldr		r3, [r0]
	add		r0, #4
	str 	r3, [r1]
	add		r1, #4
1:	cmp		r1, r2
	blt		0b
	mov		r0, #0
	ldr		r1, =__bss
	ldr		r2, =__ebss
2:	cmp		r1, r2
	bge 	3f
	str 	r0, [r1]
	add		r1, #4
	b 		2b
3:	/* Setup hardware */
	ldr		r0, =setup_clocks
	blx		r0
	ldr		r0, =setup_ports
	blx		r0
	/* Branch to main */
	ldr		r0, =main
	blx		r0
	b 		.


.thumb_func
Reset_Handler:
	b	_init0

.thumb_func
.weak NMI_Handler
NMI_Handler:
	b	.

.thumb_func
.weak HardFault_Handler
HardFault_Handler:
	b	.

.thumb_func
.weak SVCall_Handler
SVCall_Handler:
	b	.

.thumb_func
.weak PendSV_Handler
PendSV_Handler:
	b	.

.thumb_func
.weak SysTick_Handler
SysTick_Handler:
	b	.

.thumb_func
.weak PM_Handler
PM_Handler:
	b	.

.thumb_func
.weak SYSCTRL_Handler
SYSCTRL_Handler:
	b	.

.thumb_func
.weak WDT_Handler
WDT_Handler:
	b	.
	
.thumb_func
.weak RTC_Handler
RTC_Handler:
	b	.
	
.thumb_func
.weak EIC_Handler
EIC_Handler:
	b	.
	
.thumb_func
.weak NVMCTRL_Handler
NVMCTRL_Handler:
	b	.
	
.thumb_func
.weak DMAC_Handler
DMAC_Handler:
	b	.
	
.thumb_func
.weak USB_Handler
USB_Handler:
	b	.
	
.thumb_func
.weak EVSYS_Handler
EVSYS_Handler:
	b	.
	
.thumb_func
.weak SERCOM0_Handler
SERCOM0_Handler:
	b	.
	
.thumb_func
.weak SERCOM1_Handler
SERCOM1_Handler:
	b	.
	
.thumb_func
.weak SERCOM2_Handler
SERCOM2_Handler:
	b	.
	
.thumb_func
.weak SERCOM3_Handler
SERCOM3_Handler:
	b	.
	
.thumb_func
.weak SERCOM4_Handler
SERCOM4_Handler:
	b	.
	
.thumb_func
.weak SERCOM5_Handler
SERCOM5_Handler:
	b	.
	
.thumb_func
.weak TCC0_Handler
TCC0_Handler:
	b	.
	
.thumb_func
.weak TCC1_Handler
TCC1_Handler:
	b	.
	
.thumb_func
.weak TCC2_Handler
TCC2_Handler:
	b	.
	
.thumb_func
.weak TC3_Handler
TC3_Handler:
	b	.
	
.thumb_func
.weak TC4_Handler
TC4_Handler:
	b	.
	
.thumb_func
.weak TC5_Handler
TC5_Handler:
	b	.
	
.thumb_func
.weak TC6_Handler
TC6_Handler:
	b	.
	
.thumb_func
.weak TC7_Handler
TC7_Handler:
	b	.
	
.thumb_func
.weak ADC_Handler
ADC_Handler:
	b	.
	
.thumb_func
.weak AC_Handler
AC_Handler:
	b	.
	
.thumb_func
.weak DAC_Handler
DAC_Handler:
	b	.
	
.thumb_func
.weak PTC_Handler
PTC_Handler:
	b	.
	
.thumb_func
.weak I2S_Handler
I2S_Handler:
	b	.
