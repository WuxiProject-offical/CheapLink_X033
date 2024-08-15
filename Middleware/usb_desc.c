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
// ����������
		0x09,// ��������С
		0x02,     // ���������ͣ���������������
		32, 0x00, // �����ܳ���
		0x01,     // ���ýӿ���
		0x01,     // ����ֵ
		0x00,     // �����ַ�������������
		0x80,     // ����
		0xFA,     // ����500mA

		// �ӿ�������1
		0x09,// ��������С
		0x04, // ���������ͣ��ǽӿ���������
		0x00, // �ӿڱ��
		0x00, // �ӿ��������ã�����
		0x02, // �۳��˵�0��Ķ˵���
		0xFF, // �ӿ��ࣨ��Vendor��
		0x00, // �Զ���
		0x00, // �Զ���
		0x04, // �ӿ��ַ�������������

		// �˵�������
		0x07,// ��������С
		0x05,                // ���������ͣ��Ƕ˵���������
		0x01,                // ��EP2OUT
		0x02,                // ����������
		DEF_USBD_ENDP2_SIZE, 0x00, // �˵��С
		0x00,                 // ������
		// �˵�������
		0x07,// ��������С
		0x05,                // ���������ͣ��Ƕ˵���������
		0x82,                // ��EP2IN
		0x02,                // ����������
		DEF_USBD_ENDP1_SIZE, 0x00, // �˵��С
		0x00,                 // ������

//		// �ӿ�������2
//		0x09,// ��������С
//		0x04, // ���������ͣ��ǽӿ���������
//		0x01, // �ӿڱ��
//		0x00, // �ӿ��������ã�����
//		0x02, // �۳��˵�0��Ķ˵���
//		0xFF, // �ӿ��ࣨ��Vendor��
//		0x00, // �Զ���
//		0x00, // �Զ���
//		0x00, // �ӿ��ַ�������������
//
//		// �˵�������
//		0x07,// ��������С
//		0x05,                // ���������ͣ��Ƕ˵���������
//		0x05,                // ��EP5OUT
//		0x02,                // ����������
//		DEF_USBD_ENDP4_SIZE, 0x00, // �˵��С
//		0x00,                 // ������
//		// �˵�������
//		0x07,// ��������С
//		0x05,                // ���������ͣ��Ƕ˵���������
//		0x83,                // ��EP3IN
//		0x02,                // ����������
//		DEF_USBD_ENDP3_SIZE, 0x00, // �˵��С
//		0x00,                 // ������
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
	0x03,// Str type desc
	0x4d, 0x00, 0x53, 0x00, 0x46, 0x00, 0x54, 0x00, 0x31, 0x00, 0x30, 0x00,
	0x30, 0x00, 0x11, 0x00};

const uint8_t WCID1Desc[] =
{	0x28, 0x00, 0x00,
	0x00, // Length 40
	0x00,
	0x01,// WCID1.0
	0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,// First Interface 0
	0x01,// ?
	'W', 'I', 'N', 'U', 'S', 'B', 0x00,
	0x00,// CompatibleID {'W', 'I', 'N', 'U', 'S', 'B', '\0', 0x00} for WINUSB
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00};

const uint8_t WCID1DescEx[] =
{	0x8e, 0x00, 0x00,
	0x00, // Length 142
	0x00,
	0x01,// WCID1.0
	0x05, 0x00,
	0x01,0x00,
	132, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00,
	40,
	0x00,// sizeof(struct MS_DEV_GUID_NAME)
	// MS_DEV_GUID_NAME
	'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I',
	0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00, 'a', 0x00,
	'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 0, 0,
	78,
	0x00,0x00,0x00,// sizeof (struct MS_DEV_INT_GUID)
	// MS_DEV_INT_GUID
	'{', 0, 'C', 0, 'D', 0, 'B', 0, '3', 0, 'B', 0, '5', 0, 'A', 0, 'D', 0,
	'-', 0, '2', 0, '9', 0, '3', 0, 'B', 0, '-', 0, '4', 0, '6', 0, '6', 0,
	'3', 0, '-', 0, 'A', 0, 'A', 0, '3', 0, '6', 0, '-', 0, '1', 0, 'A', 0,
	'A', 0, 'E', 0, '4', 0, '6', 0, '4', 0, '6', 0, '3', 0, '7', 0, '7', 0,
	'6', 0, '}', 0, 0, 0};
#elif MSOS_DESC == 2
const uint8_t MyBosDesc[] =
{
///////////////////////////////////////
/// WCID20 BOS descriptor
///////////////////////////////////////
		0x05, /* bLength */
		0x0f, /* bDescriptorType */
		0x21, 0x00, /* wTotalLength */
		0x01, /* bNumDeviceCaps */

		///////////////////////////////////////
		/// WCID20 device capability descriptor
		///////////////////////////////////////
		0x1c, /* bLength */
		0x10, /* bDescriptorType */
		0x05, /* bDevCapabilityType */
		0x00, /* bReserved */
		0xdf, 0x60, 0xdd, 0xd8, 0x89, 0x45, 0xc7, 0x4c, /* bPlatformCapabilityUUID_16 */
		0x9c, 0xd2, 0x65, 0x9d, 0x9e, 0x64, 0x8a, 0x9f, /* bPlatformCapabilityUUID_16 */
		0x00, 0x00, 0x03, 0x06, /* dwWindowsVersion */
		162, 0,/* wDescriptorSetTotalLength */
		0x01, /* bVendorCode */
		0x00, /* bAltEnumCode */
};

const uint8_t MyWinusbDesc[] =
{
///////////////////////////////////////
/// WCID20 descriptor set descriptor
///////////////////////////////////////
		0x0a, 0x00, /* wLength */
		0x00, 0x00, /* wDescriptorType */
		0x00, 0x00, 0x03, 0x06, /* dwWindowsVersion */
		0xa2, 0x00, /* wDescriptorSetTotalLength */

		///////////////////////////////////////
		/// WCID20 compatible ID descriptor
		///////////////////////////////////////
		0x14, 0x00, /* wLength */
		0x03, 0x00, /* wDescriptorType */
		/* WINUSB */
		'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00, /* cCID_8 */
		/*  */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* cSubCID_8 */

		///////////////////////////////////////
		/// WCID20 registry property descriptor
		///////////////////////////////////////
		0x84, 0x00, /* wLength */
		0x04, 0x00, /* wDescriptorType */
		0x07, 0x00, /* wPropertyDataType */
		0x2a, 0x00, /* wPropertyNameLength */
		/* DeviceInterfaceGUIDs */
		'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, /* wcPropertyName_21 */
		'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, /* wcPropertyName_21 */
		't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00, /* wcPropertyName_21 */
		'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, /* wcPropertyName_21 */
		'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, /* wcPropertyName_21 */
		0x00, 0x00, /* wcPropertyName_21 */
		0x50, 0x00, /* wPropertyDataLength */
		/* {CDB3B5AD-293B-4663-AA36-1AAE46463776} */
		'{', 0x00, 'C', 0x00, 'D', 0x00, 'B', 0x00, /* wcPropertyData_40 */
		'3', 0x00, 'B', 0x00, '5', 0x00, 'A', 0x00, /* wcPropertyData_40 */
		'D', 0x00, '-', 0x00, '2', 0x00, '9', 0x00, /* wcPropertyData_40 */
		'3', 0x00, 'B', 0x00, '-', 0x00, '4', 0x00, /* wcPropertyData_40 */
		'6', 0x00, '6', 0x00, '3', 0x00, '-', 0x00, /* wcPropertyData_40 */
		'A', 0x00, 'A', 0x00, '3', 0x00, '6', 0x00, /* wcPropertyData_40 */
		'-', 0x00, '1', 0x00, 'A', 0x00, 'A', 0x00, /* wcPropertyData_40 */
		'E', 0x00, '4', 0x00, '6', 0x00, '4', 0x00, /* wcPropertyData_40 */
		'6', 0x00, '3', 0x00, '7', 0x00, '7', 0x00, /* wcPropertyData_40 */
		'6', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00, /* wcPropertyData_40 */
};
#endif
