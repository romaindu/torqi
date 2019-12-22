/*
 * debug.c
 *
 * @author: Romain Durand
*/

#include "sam.h"

void _putchar(char c)
{
    #ifdef DEBUG_MODE

        while (!(SERCOM3->USART.INTFLAG.bit.DRE));
        SERCOM3->USART.DATA.reg = c;

    #endif
}
