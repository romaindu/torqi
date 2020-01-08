/*
 * descriptors.c
 *
 * @author: Romain Durand
 */

#include "tusb.h"
#include "reports.h"

enum
{
    ITF_NUM_HID,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_HID_INOUT_DESC_LEN)

tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0x2000,
    .idProduct          = 0x2027,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void)
{
    return (uint8_t const *) &desc_device;
}

uint8_t const desc_hid_report[] =
{
    /* IN 1: WHEEL REPORT */
    0x05,0x01,          //    Usage Page Generic Desktop
    0x09,0x04,          //    Usage Joystick
    0xa1,0x01,          //    Collection Application
    0x85,WHEEL_REPORT_ID,
    0xa1,0x00,              //    Collection Physical
    0x09,0x30,                  //    Usage X
    0x09,0x31,                  //    Usage Y
    0x16,0x00,0x80,             //    Logical Minimum (-32768d)
    0x26,0xff,0x7f,             //    Logical Maximum (32767d)
    0x36,0xe4,0xfd,             //    Physical Minimum (-540)
    0x46,0x1c,0x02,             //    Physical Maximum (540d)
    0x75,0x10,                  //    Report Size 10h (16d)
    0x95,0x02,                  //    Report Count 2
    0x81,0x02,                      //    Input (Variable)
    0xc0,                   //    End Collection
    0xa1,0x00,              //    Collection Physical
    0x09,0x33,                  //    Usage Rx
    0x09,0x34,                  //    Usage Ry
    0x09,0x35,                  //    Usage Rz
    0x15,0x00,                  //    Logical Minimum 0h (0d)
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0h (0d)
    0x45,0x64,                  //    Physical Maximum 64h (100d)
    0x75,0x08,                  //    Report Size (8d)
    0x95,0x03,                  //    Report Count (3d)
    0x81,0x02,                  //    Input (Variable)
    0xc0,                   //    End Collection
    0x05,0x09,              //    Usage Page Button
    0x15,0x00,              //    Logical Minimum 0h (0d)
    0x19,0x01,              //    Usage Minimum Button 1
    0x29,0x08,              //    Usage Maximum Button 8
    0x25,0x01,              //    Logical Maximum 1
    0x35,0x00,              //    Physical Minimum 0
    0x45,0x01,              //    Physical Maximum 1
    0x75,0x01,              //    Report Size 1
    0x95,0x08,              //    Report Count 8
    0x81,0x02,                  //    Input (Variable)

    /* OUT 1: SET EFFECT REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x21,              //    Usage Set Effect Report
    0xa1,0x02,              //    Collection Datalink
    0x85,SET_EFFECT_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x25,                  //    Usage Effect Type
    0xa1,0x02,                  //    Collection Datalink
    0x09,0x26,                      //    Usage ET Constant Force
    0x09,0x27,                      //    Usage ET Ramp
    0x09,0x30,                      //    Usage ET Square
    0x09,0x31,                      //    Usage ET Sine
    0x09,0x32,                      //    Usage ET Triangle
    0x09,0x33,                      //    Usage ET Sawtooth Up
    0x09,0x34,                      //    Usage ET Sawtooth Down
    0x09,0x40,                      //    Usage ET Spring
    0x09,0x41,                      //    Usage ET Damper
    0x09,0x43,                      //    Usage ET Friction
    0x15,0x01,                      //    Logical Minimum 1
    0x25,0x0a,                      //    Logical Maximum Ah (10d)
    0x35,0x01,                      //    Physical Minimum 1
    0x45,0x0a,                      //    Physical Maximum Ah (10d)
    0x75,0x08,                      //    Report Size 8
    0x95,0x01,                      //    Report Count 1
    0x91,0x00,                      //    Output
    0xc0,                       //    End Collection
    0x09,0x52,                  //    Usage Gain
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x50,                  //    Usage Duration
    //0x09,0xa7,                  //    Usage Start Delay
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0xff,0x00,             //    Physical Maximum FFh (255d)
    0x66,0x03,0x10,             //    Unit (Seconds)
    0x55,0xfd,                  //    Unit Exponent -3
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x05,0x0f,                  //    Usage Page Physical Interface
    0x65,0x00,                  //    Unit 0
    0x55,0x00,                  //    Unit Exponent 0
    0x09,0x55,                  //    Usage Axes Enable
    0xa1,0x02,                  //    Collection Datalink
    0x05,0x01,                      //    Usage Page Generic Desktop
    0x09,0x30,                      //    Usage X
    0x09,0x31,                      //    Usage Y
    0x15,0x00,                      //    Logical Minimum 0
    0x25,0x01,                      //    Logical Maximum 1
    0x35,0x00,                      //    Physical Minimum 0
    0x45,0x01,                      //    Physical Maximum 1
    0x75,0x01,                      //    Report Size 1
    0x95,0x02,                      //    Report Count 2
    0x91,0x02,                          //    Output (Variable)
    0x95,0x05,                      //    Report Count 5
    0x91,0x03,                          //    Output
    0xc0,                       //    End Collection
    0x05,0x0f,                  //    Usage Page Physical Interface
    0x09,0x56,                  //    Usage Direction Enable
    0x75,0x01,                  //    Report Size 1
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x05,0x0f,                  //    Usage Page Physical Interface
    0x09,0x57,                  //    Usage Direction
    0xa1,0x02,                  //    Collection Datalink
    0x05,0x01,                      //    Usage Page Generic Desktop
    0xa1,0x00,                      //    Collection Physical
    0x09,0x30,                          //    Usage X
    0x09,0x31,                          //    Usage Y
    0x15,0x00,                          //    Logical Minimum 0
    0x26,0xff,0x00,                     //    Logical Maximum 255
    0x35,0x00,                          //    Physical Minimum 0
    0x46,0x68,0x01,                     //    Physical Maximum 168h (360d)
    0x75,0x08,                          //    Report Size 8
    0x95,0x02,                          //    Report Count 2
    0x91,0x02,                              //    Output (Variable)
    0xc0,                           //    End Collection
    0xc0,                       //    End Collection
    0xc0,                   //    End Collection

    /* OUT 2: SET ENVELOPE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x5a,              //    Usage Set Envelope Report
    0xa1,0x02,              //    Collection Datalink
    0x85,SET_ENVELOPE_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x5b,                  //    Usage Attack Level
    0x09,0x5d,                  //    Usage Fade Level
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 8h (8d)
    0x95,0x02,                  //    Report Count 2
    0x91,0x02,                      //    Output (Variable)
    0x09,0x5c,                  //    Usage Attack Time
    0x09,0x5e,                  //    Usage Fade Time
    0x66,0x03,0x10,             //    Unit (Seconds)
    0x55,0xfd,                  //    Unit Exponent -3
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0xff,0x00,             //    Physical Maximum FFh (255d)
    0x75,0x08,                  //    Report Size 8
    0x91,0x02,                      //    Output (Variable)
    0x45,0x00,                  //    Physical Maximum 0
    0x65,0x00,                  //    Unit 0
    0x55,0x00,                  //    Unit Exponent 0
    0xc0,                   //    End Collection

    /* OUT 3: SET CONDITION REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x5f,              //    Usage Set Condition Report
    0xa1,0x02,              //    Collection Datalink
    0x85,SET_CONDITION_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x23,                  //    Usage Parameter Block Offset
    0x15,0x00,                  //    Logical Minimum 0
    0x25,0x01,                  //    Logical Maximum 1
    0x35,0x00,                  //    Physical Minimum 0
    0x45,0x01,                  //    Physical Maximum 1
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x60,                  //    Usage CP Offset
    0x09,0x61,                  //    Usage Positive Coefficient
    0x09,0x62,                  //    Usage Negative Coefficient
    0x15,0x81,                  //    Logical Minimum 81h (-127d)
    0x25,0x7f,                  //    Logical Maximum 7Fh (127d)
    0x36,0xf0,0xd8,             //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 8h (8d)
    0x95,0x03,                  //    Report Count 3
    0x91,0x02,                      //    Output (Variable)
    0x09,0x65,                  //    Usage Dead Band
    0x09,0x63,                  //    Usage Positive Saturation
    0x09,0x64,                  //    Usage Negative Saturation
    0x15,0x00,                  //    Logical Minimum 0
    0x25,0x7f,                  //    Logical Maximum 7Fh (127d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 8h (8d)
    0x95,0x03,                  //    Report Count 3
    0x91,0x02,                      //    Output (Variable)
    0xc0,                   //    End Collection
    
    /* OUT 4: SET PERIODIC REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x6e,              //    Usage Set Effect Report
    0xa1,0x02,              //    Collection Datalink
    0x85,SET_PERIODIC_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                     //    Output (Variable)
    0x09,0x70,                  //    Usage Magnitude
    0x15,0x00,                  //    Logical Minimum 0
    0x25,0x7f,                  //    Logical Maximum 7Fh (127d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 08h (8d)
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                     //    Output (Variable)
    0x09,0x6f,                  //    Usage Offset
    0x15,0x81,                  //    Logical Minimum 81h (-127d)
    0x25,0x7f,                  //    Logical Maximum 7Fh (127d)
    0x36,0xf0,0xd8,             //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 08h (8d)
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                     //    Output (Variable)
    0x09,0x71,                  //    Usage Phase
    0x65,0x14,                  //    Unit (Degrees)
    0x55,0xfe,                  //    Unit Exponent -2
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x47,0xa0,0x8c,0x00,0x00,   //    Physical Maximum 8CA0h (36000d)
    0x75,0x08,                  //    Report Size 8
    0x91,0x02,                     //    Output (Variable)
    0x09,0x72,                  //    Usage Period
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0xff,0x00,             //    Physical Maximum FFh (255d)
    0x66,0x03,0x10,             //    Unit (Seconds)
    0x55,0xfd,                  //    Unit Exponent -3
    0x75,0x08,                  //    Report Size 8h (8d)
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                     //    Output (Variable)
    0x65,0x00,                  //    Unit 0
    0x55,0x00,                  //    Unit Exponent 0
    0xc0,                   //    End Collection

    /* OUT 5: SET CONSTANT FORCE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x73,              //    Usage Set Constant Force Report
    0xa1,0x02,              //    Collection Datalink
    0x85,SET_CONSTANT_FORCE_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x70,                  //    Usage Magnitude
    0x15,0x81,                  //    Logical Minimum 81h (-127d)
    0x25,0x7f,                  //    Logical Maximum 7Fh (127d)
    0x36,0xf0,0xd8,             //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 08h (8d)
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0xc0,                   //    End Collection

    /* OUT 6: SET RAMP FORCE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x74,              //    Usage Set Ramp Force Report
    0xa1,0x02,              //    Collection Datalink
    0x85,SET_RAMP_FORCE_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x14,                  //    Logical Maximum 28h (40d)
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x14,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x75,                  //    Usage Ramp Start
    0x09,0x76,                  //    Usage Ramp End
    0x15,0x81,                  //    Logical Minimum 80h (-127d)
    0x25,0x7f,                  //    Logical Maximum 7Fh (127d)
    0x36,0xf0,0xd8,             //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 08h (8d)
    0x95,0x02,                  //    Report Count 2
    0x91,0x02,                      //    Output (Variable)
    0xc0,                   //    End Collection

    /* OUT 10: EFFECT OPERATION REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x77,              //    Usage Effect Operation Report
    0xa1,0x02,              //    Collection Datalink
    0x85,EFFECT_OPERATION_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                  //    Output (Variable)
    0x09,0x78,                  //    Usage Effect Operation
    0xa1,0x02,                  //    Collection Datalink
    0x09,0x79,                      //    Usage Op Effect Start
    0x09,0x7a,                      //    Usage Op Effect Start Solo
    0x09,0x7b,                      //    Usage Op Effect Stop
    0x15,0x01,                      //    Logical Minimum 1h (1d)
    0x25,0x03,                      //    Logical Maximum 3h (3d)
    0x35,0x01,                      //    Physical Minimum 1h (1d)
    0x45,0x03,                      //    Physical Maximum 3h (3d)
    0x75,0x08,                      //    Report Size 8
    0x95,0x01,                      //    Report Count 1
    0x91,0x00,                          //    Output
    0xc0,                       //    End Collection
    0xc0,                   //    End Collection

    /* OUT 11: PID BLOCK FREE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x90,              //    Usage PID Block Free Report
    0xa1,0x02,              //    Collection Datalink
    0x85,PID_BLOCK_FREE_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x15,0x01,                  //    Logical Minimum 1
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0xc0,                   //    End Collection

    /* OUT 12: PID DEVICE CONTROL REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x96,              //    Usage PID Device Control
    0xa1,0x02,              //    Collection Datalink
    0x85,PID_DEVICE_CONTROL_REPORT_ID,
    0x09,0x97,                  //    Usage Enable Actuators
    0x09,0x98,                  //    Usage Disable Actuators
    0x09,0x99,                  //    Usage Stop All Effects
    0x09,0x9a,                  //    Usage Device Reset
    0x09,0x9b,                  //    Usage Device Pause
    0x09,0x9c,                  //    Usage Device Continue
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x06,                  //    Logical Maximum 6
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x00,                      //    Output
    0xc0,                   //    End Collection

    /* OUT 13: DEVICE GAIN REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x7d,              //    Usage PID Device Gain Report
    0xa1,0x02,              //    Collection (Datalink)
    0x85,PID_DEVICE_GAIN_REPORT_ID,
    0x09,0x7e,                  //    Usage PID Device Gain
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0xc0,                   //    End Collection

    /* FEATURE 1: CREATE NEW EFFECT REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0xab,              //    Usage Create New Effect Report
    0xa1,0x02,              //    Collection Datalink
    0x85,CREATE_NEW_EFFECT_REPORT_ID,
    0x09,0x25,                  //    Usage Effect Type
    0xa1,0x02,                  //    Collection Datalink
    0x09,0x26,                      //    Usage ET Constant Force
    0x09,0x27,                      //    Usage ET Ramp
    0x09,0x30,                      //    Usage ET Square
    0x09,0x31,                      //    Usage ET Sine
    0x09,0x32,                      //    Usage ET Triangle
    0x09,0x33,                      //    Usage ET Sawtooth Up
    0x09,0x34,                      //    Usage ET Sawtooth Down
    0x09,0x40,                      //    Usage ET Spring
    0x09,0x41,                      //    Usage ET Damper
    0x09,0x43,                      //    Usage ET Friction
    0x15,0x01,                      //    Logical Minimum 1
    0x25,0x0a,                      //    Logical Maximum Ah (10d)
    0x35,0x01,                      //    Physical Minimum 1
    0x45,0x0a,                      //    Physical Maximum Ah (10d)
    0x75,0x08,                      //    Report Size 8
    0x95,0x01,                      //    Report Count 1
    0xb1,0x00,                          //    Feature
    0xc0,                       // End Collection
    0xc0,                   //    End Collection

    /* FEATURE 2: BLOCK LOAD REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x89,              //    Usage Block Load Report
    0xa1,0x02,              //    Collection Datalink
    0x85,BLOCK_LOAD_REPORT_ID,
    0x09,0x22,                  //    Usage Effect Block Index
    0x25,0x28,                  //    Logical Maximum 28h (40d)
    0x15,0x01,                  //    Logical Minimum 1
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x28,                  //    Physical Maximum 28h (40d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0xb1,0x02,                      //    Feature (Variable)
    0x09,0x8b,                  //    Usage Block Load Status
    0xa1,0x02,                  //    Collection Datalink
    0x09,0x8c,                      //    Usage Block Load Success
    0x09,0x8d,                      //    Usage Block Load Full
    0x09,0x8e,                      //    Usage PID Block Load Error
    0x25,0x03,                      //    Logical Maximum 3
    0x15,0x01,                      //    Logical Minimum 1
    0x35,0x01,                      //    Physical Minimum 1
    0x45,0x03,                      //    Physical Maximum 3
    0x75,0x08,                      //    Report Size 8
    0x95,0x01,                      //    Report Count 1
    0xb1,0x00,                          //    Feature
    0xc0,                       //    End Collection
    0x09,0xac,                  //    Usage RAM Pool Available
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0xff,0x00,             //    Physical Maximum FFh (255d)
    0x75,0x08,                  //    Report Size 8h (8d)
    0x95,0x01,                  //    Report Count 1
    0xb1,0x02,                      //    Feature (Variable)
    0xc0,                   //    End Collection

    /* FEATURE 3: PID POOL REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x7f,              //    Usage PID Pool Report
    0xa1,0x02,              //    Collection Datalink
    0x85,PID_POOL_REPORT_ID,
    0x09,0x80,                  //    Usage RAM Pool Size
    0x75,0x08,                  //    Report Size 8h (8d)
    0x95,0x01,                  //    Report Count 1
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0xff,0x00,             //    Physical Maximum FFh (255d)
    0xb1,0x02,                      //    Feature (Variable)
    0x09,0x83,                  //    Usage Simultaneous Effects Max
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x46,0xff,0x00,             //    Physical Maximum FFh (255d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0xb1,0x02,                      //    Feature (Variable)
    0x09,0xa9,                  //    Usage Device Managed Pool
    0x09,0xaa,                  //    Usage Shared Parameter Blocks
    0x75,0x01,                  //    Report Size 1
    0x95,0x02,                  //    Report Count 2
    0x15,0x00,                  //    Logical Minimum 0
    0x25,0x01,                  //    Logical Maximum 1
    0x35,0x00,                  //    Physical Minimum 0
    0x45,0x01,                  //    Physical Maximum 1
    0xb1,0x02,                  //    Feature (Variable)
    0x75,0x06,                  //    Report Size 6
    0x95,0x01,                  //    Report Count 1
    0xb1,0x03,                      //    Feature (Constant, Variable)
    0xc0,                   //    End Collection

    0xc0,               //    End Collection
};

uint8_t const * tud_hid_descriptor_report_cb(void)
{
    return desc_hid_report;
}

uint8_t const desc_configuration[] =
{
    // Interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_SELF_POWERED, 0),

    // Interface number, string index, protocol, report descriptor len, EP OUT & IN address, size & polling interval
    TUD_HID_INOUT_DESCRIPTOR(ITF_NUM_HID, 4, HID_PROTOCOL_NONE, sizeof(desc_hid_report), 0x03, 0x83, CFG_TUD_HID_BUFSIZE, 1),
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
    (void) index;
    return desc_configuration;
}

char const* string_desc_arr [] =
{
    (const char[]) { 0x09, 0x04 },  // 0: English (0x0409)
    "Dept Industries",              // 1: Manufacturer
    "Torqi",                        // 2: Product
    "0001",                         // 3: Serials
    "Torqi Force Feedback",         // 4: HID
};

static uint16_t _desc_str[32];

uint16_t const* tud_descriptor_string_cb(uint8_t index)
{
    uint8_t chr_count;

    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    }
    else {
        if (!(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])))
            return NULL;

        const char* str = string_desc_arr[index];

        chr_count = strlen(str);

        if (chr_count > 31)
            chr_count = 31;

        for(uint8_t i=0; i<chr_count; i++)
            _desc_str[1+i] = str[i];
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2*chr_count + 2);

    return _desc_str;
}
