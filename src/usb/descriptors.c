/*
 * descriptors.c
 * @author: Romain Durand
 */

#include "descriptors.h"

const DeviceDescriptor_t deviceDescriptor = {
    .bLength                =   18,
    .bDescriptorType        =   0x01,
    .bcdUSB                 =   0x0200,     // USB 2.0
    .bDeviceClass           =   0,          // Interface defined
    .bDeviceSubClass        =   0,          // Interface defined
    .bDeviceProtocol        =   0,          // Interface defined
    .bMaxPacketSize         =   USB_EP0_SIZE,
    .idVendor               =   0x2000,
    .idProduct              =   0x2000,
    .bcdDevice              =   0x0030,     // v0.3
    .iManufacturer          =   1,
    .iProduct               =   2,
    .iSerialNumber          =   3,
    .bNumConfigurations     =   1,
};

const uint8_t hidReportDescriptor[] = {
    /* IN 1: WHEEL REPORT */
    0x05,0x01,          //    Usage Page Generic Desktop
    0x09,0x04,          //    Usage Joystick
    0xa1,0x01,          //    Collection Application
    0x85,0x01,              //    Report ID 1
    0xa1,0x00,              //    Collection Physical
    0x09,0x30,                  //    Usage X
    0x09,0x31,                  //    Usage Y
    0x16,0x00,0xf0,             //    Logical Minimum (-4096d)
    0x26,0xff,0x0f,             //    Logical Maximum (4095d)
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
    0x85,0x01,                  //    Report ID 1
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
    0x09,0x42,                      //    Usage ET Inertia
    0x09,0x43,                      //    Usage ET Friction
    0x15,0x01,                      //    Logical Minimum 1
    0x25,0x0b,                      //    Logical Maximum Bh (11d)
    0x35,0x01,                      //    Physical Minimum 1
    0x45,0x0b,                      //    Physical Maximum Bh (11d)
    0x75,0x08,                      //    Report Size 8
    0x95,0x01,                      //    Report Count 1
    0x91,0x00,                      //    Output
    0xc0,                       //    End Collection
    0x09,0x50,                  //    Usage Duration
    0x09,0x54,                  //    Usage Trigger Repeat Interval
    0x09,0x51,                  //    Usage Sample Period
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x7f,             //    Logical Maximum 7FFFh (32767d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0xff,0x7f,             //    Physical Maximum 7FFFh (32767d)
    0x66,0x03,0x10,             //    Unit (Seconds)
    0x55,0xfd,                  //    Unit Exponent -3
    0x75,0x10,                  //    Report Size 16
    0x95,0x03,                  //    Report Count 3
    0x91,0x02,                      //    Output (Variable)
    0x55,0x00,                  //    Unit Exponent 0
    0x65,0x00,                  //    Unit None
    0x09,0x52,                  //    Usage Gain
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0x09,0x53,                  //    Usage Trigger Button
    0x15,0x01,                  //    Logical Minimum 1
    0x25,0x08,                  //    Logical Maximum 8
    0x35,0x01,                  //    Physical Minimum 1
    0x45,0x08,                  //    Physical Maximum 8
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
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
    0x26,0xff,0x00,                     //    Logical Maximum FFh (255d)
    0x35,0x00,                          //    Physical Minimum 0
    0x46,0x68,0x01,                     //    Physical Maximum 168h (360d)
    0x66,0x14,0x00,                     //    Unit (Degrees)
    0x75,0x08,                          //    Report Size 8
    0x95,0x02,                          //    Report Count 2
    0x91,0x02,                              //    Output (Variable)
    0x65,0x00,                          //    Unit 0
    0x45,0x00,                          //    Unit Exponent 0
    0xc0,                           //    End Collection
    0xc0,                       //    End Collection
    0xc0,                   //    End Collection

    /* OUT 2: SET ENVELOPE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x5a,              //    Usage Set Envelope Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x02,                //    Report ID 2
    0x09,0x22,                //    Usage Effect Block Index
    0x15,0x01,                //    Logical Minimum 1
    0x25,0x28,                //    Logical Maximum 28h (40d)
    0x35,0x01,                //    Physical Minimum 1
    0x45,0x28,                //    Physical Maximum 28h (40d)
    0x75,0x08,                //    Report Size 8
    0x95,0x01,                //    Report Count 1
    0x91,0x02,                    //    Output (Variable)
    0x09,0x5b,                //    Usage Attack Level
    0x09,0x5d,                //    Usage Fade Level
    0x15,0x00,                //    Logical Minimum 0
    0x26,0x10,0x27,           //    Logical Maximum 2710h (10000d)
    0x35,0x00,                //    Physical Minimum 0
    0x46,0x10,0x27,           //    Physical Maximum 2710h (10000d)
    0x75,0x10,                //    Report Size 10h (16d)
    0x95,0x02,                //    Report Count 2
    0x91,0x02,                    //    Output (Variable)
    0x09,0x5c,                //    Usage Attack Time
    0x09,0x5e,                //    Usage Fade Time
    0x66,0x03,0x10,           //    Unit (Seconds)
    0x55,0xfd,                //    Unit Exponent -3
    0x26,0xff,0x7f,           //    Logical Maximum 7FFFh (32767d)
    0x46,0xff,0x7f,           //    Physical Maximum 7FFFh (32767d)
    0x75,0x10,                //    Report Size 16
    0x91,0x02,                    //    Output (Variable)
    0x45,0x00,                //    Physical Maximum 0
    0x65,0x00,                //    Unit 0
    0x55,0x00,                //    Unit Exponent 0
    0xc0,                   //    End Collection

    /* OUT 3: SET CONDITION REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x5f,              //    Usage Set Condition Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x03,                //    Report ID 3
    0x09,0x22,                //    Usage Effect Block Index
    0x15,0x01,                //    Logical Minimum 1
    0x25,0x28,                //    Logical Maximum 28h (40d)
    0x35,0x01,                //    Physical Minimum 1
    0x45,0x28,                //    Physical Maximum 28h (40d)
    0x75,0x08,                //    Report Size 8
    0x95,0x01,                //    Report Count 1
    0x91,0x02,                    //    Output (Variable)
    0x09,0x60,                //    Usage CP Offset
    0x09,0x65,                //    Usage Dead Band
    0x09,0x61,                //    Usage Positive Coefficient
    0x09,0x62,                //    Usage Negative Coefficient
    0x16,0xf0,0xd8,           //    Logical Minimum D8F0h (-10000d)
    0x26,0x10,0x27,           //    Logical Maximum 2710h (10000d)
    0x36,0xf0,0xd8,           //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,           //    Physical Maximum 2710h (10000d)
    0x75,0x10,                //    Report Size 10h (16)
    0x95,0x04,                //    Report Count 4
    0x91,0x02,                    //    Output (Variable)
    0x15,0x00,                //    Logical Minimum 0
    0x26,0x10,0x27,           //    Logical Maximum 2710h (10000d)
    0x35,0x00,                //    Physical Minimum 0
    0x46,0x10,0x27,           //    Physical Maximum 2710h (10000d)
    0x09,0x63,                //    Usage Positive Saturation
    0x09,0x64,                //    Usage Negative Saturation
    0x75,0x10,                //    Report Size 10h (16d)
    0x95,0x02,                //    Report Count 2
    0x91,0x02,                    //    Output (Variable)
    0xc0,                   //    End Collection
    
    /* OUT 4: SET PERIODIC REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x6e,              //    Usage Set Effect Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x04,              //    Report ID 4
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
    0x26,0x10,0x27,             //    Logical Maximum 2710h (10000d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x75,0x10,                  //    Report Size 10h (16d)
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                     //    Output (Variable)
    0x09,0x6f,                  //    Usage Offset
    0x16,0xf0,0xd8,             //    Logical Minimum D8F0h (-10000d)
    0x26,0x10,0x27,             //    Logical Maximum 2710h (10000d)
    0x36,0xf0,0xd8,             //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,             //    Physical Maximum 2710h (10000d)
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                     //    Output (Variable)
    0x09,0x71,                  //    Usage Phase
    0x65,0x14,                  //    Unit (Degrees)
    0x55,0xfe,                  //    Unit Exponent -2
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xb4,0x00,             //    Logical Maximum B4h (180d)
    0x35,0x00,                  //    Physical Minimum 0
    0x47,0xa0,0x8c,0x00,0x00,   //    Physical Maximum 8CA0h (36000d)
    0x75,0x08,                  //    Report Size 8
    0x91,0x02,                     //    Output (Variable)
    0x09,0x72,                  //    Usage Period
    0x26,0xff,0x7f,             //    Logical Maximum 7FFFh (32767d)
    0x46,0xff,0x7f,             //    Physical Maximum 7FFFh (32767d)
    0x66,0x03,0x10,             //    Unit (Seconds)
    0x55,0xfd,                  //    Unit Exponent -3
    0x75,0x10,                  //    Report Size 10h (16d)
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                     //    Output (Variable)
    0x65,0x00,                  //    Unit 0
    0x55,0x00,                  //    Unit Exponent 0
    0xc0,                   //    End Collection

    /* OUT 5: SET CONSTANT FORCE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x73,              //    Usage Set Constant Force Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x05,                  //    Report ID 5
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

    /* OUT 7: SET RAMP FORCE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x74,              //    Usage Set Ramp Force Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x06,                  //    Report ID 6
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
    0x15,0x80,                  //    Logical Minimum 80h (-128d)
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
    0x85,0x0a,                  //    Report ID Ah (10d)
    0x09,0x22,                  //    Usage Effect Block Index
    0x15,0x00,                  //    Logical Minimum 0
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
    0x09,0x7c,                  //    Usage Loop Count
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0x7f,0x00,             //    Logical Maximum FFh (255d)
    0x35,0x00,                  //    Physical Minimum 0h (0d)
    0x46,0xff,0x00,             //    Physical Maximum FFh (255d)
    0x75,0x08,                  //    Report Size 8
    0x95,0x01,                  //    Report Count 1
    0x91,0x02,                      //    Output (Variable)
    0xc0,                   //    End Collection

    /* OUT 11: PID BLOCK FREE REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x90,              //    Usage PID Block Free Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x0b,                  //    Report ID Bh (11d)
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
    0x85,0x0c,                  //    Report ID Ch (12d)
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
    0x85,0x0d,                  //    Report ID Dh (13d)
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
    0x85,0x01,                  //    Report ID 1
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
    0x09,0x42,                      //    Usage ET Inertia
    0x09,0x43,                      //    Usage ET Friction
    0x15,0x01,                      //    Logical Minimum 1
    0x25,0x0b,                      //    Logical Maximum Bh (11d)
    0x35,0x01,                      //    Physical Minimum 1
    0x45,0x0b,                      //    Physical Maximum Bh (11d)
    0x75,0x08,                      //    Report Size 8
    0x95,0x01,                      //    Report Count 1
    0xb1,0x00,                          //    Feature
    0xc0,                       // End Collection
    0x05,0x01,                  //    Usage Page Generic Desktop
    0x09,0x3b,                  //    Usage Byte Count
    0x15,0x00,                  //    Logical Minimum 0
    0x26,0xff,0x01,             //    Logical Maximum 1FFh (511d)
    0x35,0x00,                  //    Physical Minimum 0
    0x46,0xff,0x01,             //    Physical Maximum 1FFh (511d)
    0x75,0x0a,                  //    Report Size Ah (10d)
    0x95,0x01,                  //    Report Count 1
    0xb1,0x02,                      //    Feature (Variable)
    0x75,0x06,                  //    Report Size 6
    0xb1,0x01,                      //    Feature (Constant)
    0xc0,                   //    End Collection

    /* FEATURE 2: BLOCK LOAD REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x89,              //    Usage Block Load Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x02,                  //    Report ID 2
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
    0x27,0xff,0xff,0x00,0x00,   //    Logical Maximum FFFFh (65535d)
    0x35,0x00,                  //    Physical Minimum 0
    0x47,0xff,0xff,0x00,0x00,   //    Physical Maximum FFFFh (65535d)
    0x75,0x10,                  //    Report Size 10h (16d)
    0x95,0x01,                  //    Report Count 1
    0xb1,0x02,                      //    Feature (Variable)
    0xc0,                   //    End Collection

    /* FEATURE 3: PID POOL REPORT */
    0x05,0x0f,              //    Usage Page Physical Interface
    0x09,0x7f,              //    Usage PID Pool Report
    0xa1,0x02,              //    Collection Datalink
    0x85,0x03,                  //    Report ID 3
    0x09,0x80,                  //    Usage RAM Pool Size
    0x75,0x10,                  //    Report Size 10h (16d)
    0x95,0x01,                  //    Report Count 1
    0x15,0x00,                  //    Logical Minimum 0
    0x35,0x00,                  //    Physical Minimum 0
    0x27,0xff,0xff,0x00,0x00,   //    Logical Maximum FFFFh (65535d)
    0x47,0xff,0xff,0x00,0x00,   //    Physical Maximum FFFFh (65535d)
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

typedef struct {
    ConfigurationDescriptor_t    configuration;
    InterfaceDescriptor_t        interface;
    HIDDescriptor_t              hid;
    EndpointDescriptor_t         ep1in;
    EndpointDescriptor_t         ep1out;
} __attribute__((packed, aligned(1))) Configuration_t;

Configuration_t configurationDescriptor = {
	.configuration = {
		.bLength                =   9,
	    .bDescriptorType        =   0x02,
	    .wTotalLength           =   9+9+9+7+7,
	    .bNumInterfaces         =   1,
	    .bConfigurationValue    =   1,
	    .iConfiguration         =   0,
	    .bmAttributes           =   (1 << 7),   // Bus powered
	    .bMaxPower              =   50,         // 100 mA
	},
	.interface = {
		.bLength                =   9,
	    .bDescriptorType        =   0x04,
	    .bInterfaceNumber       =   HID_INTERFACE_NUM,
	    .bAlternateSetting      =   0,
	    .bNumEndpoints          =   2,
	    .bInterfaceClass        =   0x03,       // HID Device
	    .bInterfaceSubClass     =   0,
	    .bInterfaceProtocol     =   0,
	    .iInterface             =   0,
	},
	.hid = {
		.bLength                =   9,
	    .bDescriptorType        =   0x21,
	    .bcdHID                 =   0x0111,
	    .bCountryCode           =   0,
	    .bNumHIDDescriptors     =   1,
	    .bReportDescriptorType  =   0x22,
	    .wReportDescriptorLength=   sizeof(hidReportDescriptor),
	},
	.ep1in = {
		.bLength                =   7,
	    .bDescriptorType        =   0x05,
	    .bEndpointAdress        =   HID_EP_IN_ADDR,    // IN, addr = 1
	    .bmAttributes           =   0b11,              // Interrupt endpoint
	    .wMaxPacketSize         =   HID_EP_IN_SIZE,    // 16 bytes
	    .bInterval              =   0x01,              // 1 ms
	},
	.ep1out = {
		.bLength                =   7,
	    .bDescriptorType        =   0x05,
	    .bEndpointAdress        =   HID_EP_OUT_ADDR,   // OUT, addr = 1
	    .bmAttributes           =   0b11,              // Interrupt endpoint
	    .wMaxPacketSize         =   HID_EP_OUT_SIZE,   // 16 bytes
	    .bInterval              =   0x01,              // 1 ms
	},
};

const StringDescriptor_t string0Descriptor = {
    .bLength                =   4,
    .bDescriptorType        =   0x03,
    .bString                =   {0x0409},       // English 0x0409
};

const StringDescriptor_t manufacturerStringDescriptor = {
    .bLength                =   32,
    .bDescriptorType        =   0x03,
    .bString                =   {'D', 'e', 'p', 't', ' ', 'I', 'n', 'd', 
                                 'u', 's', 't', 'r', 'i', 'e', 's',},
};

const StringDescriptor_t productStringDescriptor = {
    .bLength                =   12,
    .bDescriptorType        =   0x03,
    .bString                =   {'T', 'o', 'r', 'q', 'i',},
};

const StringDescriptor_t serialStringDescriptor = {
    .bLength                =   10,
    .bDescriptorType        =   0x03,
    .bString                =   {'0', '0', '0', '1',},
};

const RequestedDesc_t reqDescList[] = {
    {0x0100, 0x0000, (uint8_t *)(&deviceDescriptor),
        sizeof(deviceDescriptor)},

    {0x0200, 0x0000, (uint8_t *)(&configurationDescriptor),
        sizeof(configurationDescriptor)},

    {0x0300, 0x0000, (uint8_t *)(&string0Descriptor), 4},

    {0x0301, 0x0409, (uint8_t *)(&manufacturerStringDescriptor), 32},

    {0x0302, 0x0409, (uint8_t *)(&productStringDescriptor), 12},

    {0x0303, 0x0409, (uint8_t *)(&serialStringDescriptor), 10},

    {0x2200, 0x0000, (uint8_t *)(&hidReportDescriptor), sizeof(hidReportDescriptor)},
};

const int8_t DESCRIPTORS_COUNT =
    (sizeof(reqDescList)/sizeof(RequestedDesc_t));

const int16_t HID_REPORT_DESCRIPTOR_SIZE = sizeof(hidReportDescriptor);
