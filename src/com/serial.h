#ifndef DEBUG_H
#define DEBUG_H

#include "samd21.h"

#ifdef DEBUG_MODE

    void debug_init(void);
    void _putc(int c);
    void _puth8(char c);
    void _puth32(int d);
    void _putm(void *ptr, int size);
    void _puts(const char *s);

    #define com_init()          debug_init()
    #define puts(str)           _puts(__extension__({static const int8_t debug_str[] = str; debug_str;}))
    #define putm(ptr,size)      _putm(ptr,size)
    #define putr32(reg)         puts(#reg "=0x"); _puth32(reg); _putc('\n')

#else

    #define com_init()
    #define puts(str)
    #define putm(ptr,size)
    #define putr32(reg)

#endif

#endif