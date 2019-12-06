#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <stdint.h>

#define USB_EP0_SIZE        64

struct DeviceDescriptor_t
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
};

struct ConfigurationDescriptor_t
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    wTotalLength;
    uint8_t     bNumInterfaces;
    uint8_t     bConfigurationValue;
    uint8_t     iConfiguration;
    uint8_t     bmAttributes;
    uint8_t     bMaxPower;
};

struct InterfaceDescriptor_t
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
};

struct HIDDescriptor_t
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    bcdHID;
    uint8_t     bCountryCode;
    uint8_t     bNumHIDDescriptors;
    uint8_t     bReportDescriptorType;
    uint16_t    wReportDescriptorLength;
};

struct EndpointDescriptor_t
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint8_t     bEndpointAdress;
    uint8_t     bmAttributes;
    uint16_t    wMaxPacketSize;
    uint8_t     bInterval;
};

struct StringDescriptor_t
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    int16_t     bString[];
};

struct DescriptorList_t {
    uint16_t        wValue;
    uint16_t        wIndex;
    uint8_t         *addr;
    int16_t         len;
};

extern const struct  DescriptorList_t descriptorList[];
extern const int8_t DESCRIPTORS_COUNT;

extern const uint8_t hidReportDescriptor[];
extern const int16_t HID_REPORT_DESCRIPTOR_SIZE;

#endif