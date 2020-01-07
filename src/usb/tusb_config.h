#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#define CFG_TUSB_DEBUG          0

#define CFG_TUSB_MCU            OPT_MCU_SAMD21

#define CFG_TUSB_RHPORT0_MODE   OPT_MODE_DEVICE
#define CFG_TUSB_OS             OPT_OS_NONE

#define CFG_TUD_HID             1
#define CFG_TUD_HID_BUFSIZE     16

#endif