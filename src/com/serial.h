#ifndef DEBUG_H
#define DEBUG_H

static inline void serial_init(void)
{
	#ifdef DEBUG_MODE

		SERCOM3->USART.CTRLA.bit.SWRST = 1;
	    while (SERCOM3->USART.SYNCBUSY.bit.SWRST);

	    /* Configure a 115200 baud TX only UART */
	    SERCOM3->USART.BAUD.reg = SERCOM_USART_BAUD_USARTFP_BAUD(63019);
	    SERCOM3->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN;
	    SERCOM3->USART.CTRLA.reg =  SERCOM_USART_CTRLA_DORD +
	                                SERCOM_USART_CTRLA_MODE_USART_INT_CLK +
	                                SERCOM_USART_CTRLA_ENABLE;

	    while (SERCOM3->USART.SYNCBUSY.bit.ENABLE);
	    
	#else

	#endif
}

#endif