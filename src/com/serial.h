/*
 *	Copyright 2020 Romain Durand
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
*/

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