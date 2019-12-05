/*
 * debug.c
 *
 * @author: Romain Durand
*/

#include "serial.h"

#include "samd21.h"

void debug_init(void)
{
    SERCOM3->USART.CTRLA.bit.SWRST = 1;

    while (SERCOM3->USART.SYNCBUSY.bit.SWRST);

    /* Configure a 250000 baud TX only UART */
    SERCOM3->USART.BAUD.reg = SERCOM_USART_BAUD_USARTFP_BAUD(60074);
    SERCOM3->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN;
    SERCOM3->USART.CTRLA.reg =  SERCOM_USART_CTRLA_DORD +
                                SERCOM_USART_CTRLA_MODE_USART_INT_CLK +
                                SERCOM_USART_CTRLA_ENABLE;

    while (SERCOM3->USART.SYNCBUSY.bit.ENABLE);
}

inline void _putc(int c)
{
    while (!(SERCOM3->USART.INTFLAG.bit.DRE));
    SERCOM3->USART.DATA.reg = c;
}

inline void _puts(const char *s)
{
    while (*s) _putc(*s++);
}

void _puth8(char c)
{
    const int8_t lut[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    _putc(lut[(c >> 4) & 0x0f]);
    _putc(lut[c & 0x0f]);
}

void _puth32(int d)
{
    const int8_t lut[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 7; i >= 0; --i) {
        _putc(lut[(d >> 4*i) & 0x0f]);
    }
}

void _putm(int *ptr, int size)
{
    int *p, i;

    while (size > 0) {
        _puth32((int32_t)ptr);
        _putc(' ');
        _putc(' ');

        p = ptr;

        for (i = 0; i < 16; ++i) {
            if (size - i > 0) {
                _puth8(*p++);
            } else {
                _putc(' ');
                _putc(' ');
            }
            _putc(' ');
        }

        p = ptr;

        _putc(' ');
        _putc(' ');
        _putc('|');
        
        for (i = 0; i < 16; ++i) {
            if (size - i > 0) {
                if (*p >= 0x20 && *p < 0x7F) _putc(*p++);
                else {_putc('.'); p++;};
            } else {
                _putc(' ');
            }
        }

        _putc('|');
        _putc('\n');

        size -= 16;
        ptr += 16;
    }
}

