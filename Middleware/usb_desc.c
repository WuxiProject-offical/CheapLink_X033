/*
 *  USB Descriptors for firmware of CheapLink_X033
 *  Copyright (C) 2022-2024  WuxiProject
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "usb_desc.h"

/* Device Descriptor */
const uint8_t MyDevDescr[] =
{ 0x12,       // bLength
		0x01,       // bDescriptorType (Device)
#if MSOS_DESC == 1
		0x00, 0x02, // bcdUSB 2.00
#elif MSOS_DESC == 2
		0x10, 0x02, // bcdUSB 2.10
#endif
		0x00,       // bDeviceClass
		0x00,       // bDeviceSubClass
		0x00,       // bDeviceProtocol
		DEF_USBD_UEP0_SIZE,   // bMaxPacketSize0 64
		(uint8_t) DEF_USB_VID, (uint8_t) (DEF_USB_VID >> 8), // idVendor
		(uint8_t) DEF_USB_PID, (uint8_t) (DEF_USB_PID >> 8), // idProduct
		0x10, 0x01, // bcdDevice
		0x01,       // iManufacturer (String Index)
		0x02,       // iProduct (String Index)
		0x03,       // iSerialNumber (String Index)
		0x01,       // bNumConfigurations 1
		};

/* Configuration Descriptor */
const uint8_t MyCfgDescr[] =
{
// 配置描述符
		0x09,// 描述符大小
		0x02,     // 描述符类型（是配置描述符）
		32, 0x00, // 配置总长度
		0x01,     // 配置接口数
		0x01,     // 配置值
		0x00,     // 配置字符串描述符索引
		0x80,     // 机供
		0xFA,     // 请求500mA

		// 接口描述符1
		0x09,// 描述符大小
		0x04, // 描述符类型（是接口描述符）
		0x00, // 接口编号
		0x00, // 接口替用设置（？）
		0x02, // 扣除端点0后的端点数
		0xFF, // 接口类（是Vendor）
		0x00, // 自定义
		0x00, // 自定义
		0x04, // 接口字符串描述符索引

		// 端点描述符
		0x07,// 描述符大小
		0x05,                // 描述符类型（是端点描述符）
		0x02,                // 是EP2OUT
		0x02,                // 是批量传输
		DEF_USBD_ENDP2_SIZE, 0x00, // 端点大小
		0x00,                 // 无意义
		// 端点描述符
		0x07,// 描述符大小
		0x05,                // 描述符类型（是端点描述符）
		0x81,                // 是EP1IN
		0x02,                // 是批量传输
		DEF_USBD_ENDP1_SIZE, 0x00, // 端点大小
		0x00,                 // 无意义

//		// 接口描述符2
//		0x09,// 描述符大小
//		0x04, // 描述符类型（是接口描述符）
//		0x01, // 接口编号
//		0x00, // 接口替用设置（？）
//		0x02, // 扣除端点0后的端点数
//		0xFF, // 接口类（是Vendor）
//		0x00, // 自定义
//		0x00, // 自定义
//		0x00, // 接口字符串描述符索引
//
//		// 端点描述符
//		0x07,// 描述符大小
//		0x05,                // 描述符类型（是端点描述符）
//		0x05,                // 是EP5OUT
//		0x02,                // 是批量传输
//		DEF_USBD_ENDP4_SIZE, 0x00, // 端点大小
//		0x00,                 // 无意义
//		// 端点描述符
//		0x07,// 描述符大小
//		0x05,                // 描述符类型（是端点描述符）
//		0x83,                // 是EP3IN
//		0x02,                // 是批量传输
//		DEF_USBD_ENDP3_SIZE, 0x00, // 端点大小
//		0x00,                 // 无意义
		};

/* Language Descriptor */
const uint8_t MyLangDescr[] =
{ 0x04, 0x03, 0x09, 0x04 };

/* Manufacturer Descriptor */
const uint8_t MyManuInfo[] =
{
		// Str desc: "WuxiProject"
		24,// Desc length = 24
		0x03,   // Str type desc
		0x57, 0x00, 0x75, 0x00, 0x78, 0x00, 0x69, 0x00, 0x50, 0x00, 0x72, 0x00,
		0x6f, 0x00, 0x6a, 0x00, 0x65, 0x00, 0x63, 0x00, 0x74, 0x00 };

/* Product Information */
const uint8_t MyProdInfo[] =
{
		// Str desc: "CheapLink CMSIS-DAP"
		40,// Desc length = 40
		0x03,   // Str type desc
		0x43, 0x00, 0x68, 0x00, 0x65, 0x00, 0x61, 0x00, 0x70, 0x00, 0x4c, 0x00,
		0x69, 0x00, 0x6e, 0x00, 0x6b, 0x00, 0x20, 0x00, 0x43, 0x00, 0x4d, 0x00,
		0x53, 0x00, 0x49, 0x00, 0x53, 0x00, 0x2d, 0x00, 0x44, 0x00, 0x41, 0x00,
		0x50, 0x00 };

/* Serial Number Information */
uint8_t MySerNumInfo[] =
{
		// Str desc: "TSHE-xxxxxxxx"
		28,// Desc length = 28
		0x03,   // Str type desc
		0x54, 0x00, 0x53, 0x00, 0x48, 0x00, 0x45, 0x00, 0x2d, 0x00, 0x30, 0x00,
		0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00,
		0x30, 0x00 };

const uint8_t StrDescCustom4[] =
{
		// Str desc: "CMSIS-DAP_WINUSB"
		34,// Desc length = 34
		0x03,   // Str type desc
		0x43, 0x00, 0x4d, 0x00, 0x53, 0x00, 0x49, 0x00, 0x53, 0x00, 0x2d, 0x00,
		0x44, 0x00, 0x41, 0x00, 0x50, 0x00, 0x5f, 0x00, 0x57, 0x00, 0x49, 0x00,
		0x4e, 0x00, 0x55, 0x00, 0x53, 0x00, 0x42, 0x00 };

#if MSOS_DESC == 1
const uint8_t MsOs1Desc[] =
{
		// Str desc: "MSFT100"
		18,// Desc length = 18
		0x03,   // Str type desc
		0x4d, 0x00, 0x53, 0x00, 0x46, 0x00, 0x54, 0x00, 0x31, 0x00, 0x30, 0x00,
		0x30, 0x00, 0x11, 0x00 };

const uint8_t WCID1Desc[] =
{ 0x28, 0x00, 0x00,
		0x00, // Length 40
		0x00,
		0x01, // WCID1.0
		0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, // First Interface 0
		0x01, // ?
		'W', 'I', 'N', 'U', 'S', 'B', 0x00,
		0x00, // CompatibleID {'W', 'I', 'N', 'U', 'S', 'B', '\0', 0x00} for WINUSB
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 };

const uint8_t WCID1DescEx[] =
{ 0x8e, 0x00, 0x00,
		0x00, // Length 142
		0x00,
		0x01, // WCID1.0
		0x05, 0x00,
		0x01,0x00,
		132, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		40,
		0x00,   // sizeof(struct MS_DEV_GUID_NAME)
		// MS_DEV_GUID_NAME
		'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I',
		0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00, 'a', 0x00,
		'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 0, 0,
		78,
		0x00,0x00,0x00, // sizeof (struct MS_DEV_INT_GUID)
		// MS_DEV_INT_GUID
		'{', 0, 'C', 0, 'D', 0, 'B', 0, '3', 0, 'B', 0, '5', 0, 'A', 0, 'D', 0,
		'-', 0, '2', 0, '9', 0, '3', 0, 'B', 0, '-', 0, '4', 0, '6', 0, '6', 0,
		'3', 0, '-', 0, 'A', 0, 'A', 0, '3', 0, '6', 0, '-', 0, '1', 0, 'A', 0,
		'A', 0, 'E', 0, '4', 0, '6', 0, '4', 0, '6', 0, '3', 0, '7', 0, '7', 0,
		'6', 0, '}', 0, 0, 0 };
#elif MSOS_DESC == 2
const uint8_t MyBosDesc[] =
{ 0x05,     // bNumPlatDesc
		0x0f,     // bRequestType
		40, 0x00, // sizeof(struct MS_BOS_DESCRIPTOR)
		0x02,     // bNumPlatDesc

		0x07, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00,     // ?

		33 - 5, // bDescLen
		0x10,   // bDescType
		0x05,   // bCapabilityType
		0x00,   // bReserved

		0xDF, 0x60, 0xDD, 0xD8, // MS OS 2.0 Platform Capability ID (D8DD60DF-4589-4CC7-9CD2-659D9E648A9F)
		0x89, 0x45, 0xC7, 0x4C, 0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F,

		0x00, 0x00, 0x03, 0x06, // Windows version (8.1) (0x06030000)
		170, 0x00,              // sizeof(struct MS_OS_DESC_SET_HEADER)
		0x01,                   // bRequest used to get MS_OS_DESC_SET_HEADER
		0x00                    // Doesn't support alternate enumeration
		};

const uint8_t MyWinusbDesc[] =
{ 0x0a,
		0x00,             // wLength 0x0A
		0x00,
		0x00,             // wDescriptorType 0x0000
		0x00, 0x00, 0x03,
		0x06, // dwWindowsVersion 0x06030000
		170,
		0x00, // sizeof(struct MS_OS_DESC_SET_HEADER) including MS_FUNC_SUBSET_HEADER

		// MS_FUNC_SUBSET_HEADER
		0x08,
		0x00, // wLength 0x08
		0x02,
		0x00, // wDescriptorType 0x02
		0x00,       // bFirstInterface
		0x00,       // bReserved
		160,
		0x00, // sizeof(struct MS_FUNC_SUBSET_HEADER) including MS_COMP_ID_FEAT_DESC

		// MS_COMP_ID_FEAT_DESC
		0x14,
		0x00,                                     // wLength 0x14
		0x03,
		0x00,                                     // wDescriptorType  0x03
		'W', 'I', 'N', 'U', 'S', 'B', 0x00,
		0x00, // CompatibleID {'W', 'I', 'N', 'U', 'S', 'B', '\0', 0x00} for WINUSB
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, // SubCompatibleID 0x00

		// MS_REG_PROP_DESC_GUID
		132,
		0x00,  // sizeof(struct MS_REG_PROP_DESC_GUID)
		0x04,
		0x00, // wDescriptorType 0x04
		0x07,
		0x00, // wStringType 0x07 - UTF-16 encoded null terminated strings
		42,
		0x00,   // sizeof(struct MS_DEV_GUID_NAME)
		// MS_DEV_GUID_NAME
		'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I',
		0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00, 'a', 0x00,
		'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's',
		0x00, 0x00, 0x00, 80,
		0x00, // sizeof (struct MS_DEV_INT_GUID)
		// MS_DEV_INT_GUID
		'{', 0, 'C', 0, 'D', 0, 'B', 0, '3', 0, 'B', 0, '5', 0, 'A', 0, 'D', 0,
		'-', 0, '2', 0, '9', 0, '3', 0, 'B', 0, '-', 0, '4', 0, '6', 0, '6', 0,
		'3', 0, '-', 0, 'A', 0, 'A', 0, '3', 0, '6', 0, '-', 0, '1', 0, 'A', 0,
		'A', 0, 'E', 0, '4', 0, '6', 0, '4', 0, '6', 0, '3', 0, '7', 0, '7', 0,
		'6', 0, '}', 0, 0, 0, 0, 0 };
#endif
