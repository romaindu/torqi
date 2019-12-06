#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <stdint.h>

#define USB_EP0_SIZE        64

typedef struct
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    bcdUSB;
    uint8_t     bDeviceClass;
    uint8_t     bDeviceSubClass;
    uint8_t     bDeviceProtocol;
    uint8_t     bMaxPacketSize;
    uint16_t    idVendor;
    uint16_t    idProduct;
    uint16_t    bcdDevice;
    uint8_t     iManufacturer;
    uint8_t     iProduct;
    uint8_t     iSerialNumber;
    uint8_t     bNumConfigurations;
} __attribute__((packed, aligned(1))) DeviceDescriptor_t;

typedef struct
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    wTotalLength;
    uint8_t     bNumInterfaces;
    uint8_t     bConfigurationValue;
    uint8_t     iConfiguration;
    uint8_t     bmAttributes;
    uint8_t     bMaxPower;
} __attribute__((packed, aligned(1))) ConfigurationDescriptor_t;

typedef struct
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint8_t     bInterfaceNumber;
    uint8_t     bAlternateSetting;
    uint8_t     bNumEndpoints;
    uint8_t     bInterfaceClass;
    uint8_t     bInterfaceSubClass;
    uint8_t     bInterfaceProtocol;
    uint8_t     iInterface;
} __attribute__((packed, aligned(1))) InterfaceDescriptor_t;

typedef struct
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    bcdHID;
    uint8_t     bCountryCode;
    uint8_t     bNumHIDDescriptors;
    uint8_t     bReportDescriptorType;
    uint16_t    wReportDescriptorLength;
} __attribute__((packed, aligned(1))) HIDDescriptor_t;

typedef struct
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint8_t     bEndpointAdress;
    uint8_t     bmAttributes;
    uint16_t    wMaxPacketSize;
    uint8_t     bInterval;
} __attribute__((packed, aligned(1))) EndpointDescriptor_t;

typedef struct
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    int16_t     bString[];
} __attribute__((packed, aligned(1))) StringDescriptor_t;

typedef struct {
    uint16_t    wValue;
    uint16_t    wIndex;
    uint8_t     *addr;
    int16_t     len;
} RequestedDesc_t;

extern const RequestedDesc_t reqDescList[];
extern const int8_t  DESCRIPTORS_COUNT;

extern const uint8_t hidReportDescriptor[];
extern const int16_t HID_REPORT_DESCRIPTOR_SIZE;

#endif