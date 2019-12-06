/*
 * usb.c
 *
 * @author: Romain Durand
*/

#include "usb.h"

SetupPacket_t setupPacket;
uint8_t epZeroInBuf[USB_EP0_SIZE];
uint8_t epZeroOutBuf[USB_EP0_SIZE];
