/*
 * hid.c
 *
 * @author: Romain Durand
*/

#include "hid.h"

#include <stdint.h>
#include "com/serial.h"

#define MIN(a, b)   ((a < b)?(a):(b))
#define IS_HID_CLASS_REQUEST(x)   (((x) & 0x7F) == 0x21)

enum {
    GET_REPORT = 1,
    SET_REPORT = 9,
    SET_IDLE = 10,
};

enum {
    INPUT = 1,
    OUTPUT = 2,
    FEATURE = 3,
};

static uint8_t epInBuf[HID_EP_IN_SIZE];
static uint8_t epOutBuf[HID_EP_OUT_SIZE];

void hid_configure(uint16_t confNum)
{
    usbm_configure_ep(HID_EP_IN_ADDR, HID_EP_IN_SIZE, EP_INTERRUPT, epInBuf);
    usbm_configure_ep(HID_EP_OUT_ADDR, HID_EP_OUT_SIZE, EP_INTERRUPT, epOutBuf);
}

void hid_on_setup_request(SetupPacket_t setupPacket)
{
    uint8_t reportID, reportType;
    uint16_t reportSize;

    if (IS_HID_CLASS_REQUEST(setupPacket.bmRequestType) &&
            setupPacket.wIndex == HID_INTERFACE_NUM) {
        reportID = setupPacket.wValue & 0xf;
        reportType = setupPacket.wValue >> 8;

        switch (setupPacket.bRequest) {
            case GET_REPORT:
                puts("HID: GET_REPORT\n");
                if (reportType == FEATURE)
                    reportSize = hid_cb_get_feature(reportID, epInBuf);
                else
                    reportSize = hid_cb_get_input(reportID, epInBuf);
                usbm_ep_send_in(0x80, MIN(reportSize, setupPacket.wLength));
                usbm_ep_clr_out(0x00);
                break;
            case SET_REPORT:
                puts("HID: SET_REPORT\n");
                break;
            case SET_IDLE:
                puts("HID: SET_IDLE\n");
                usbm_ep_send_in(0x80, 0);
                break;
        }
    }
}

void hid_on_in_xfer(uint8_t ep)
{

}

void hid_on_out_xfer(uint8_t ep, uint8_t bc)
{

}
