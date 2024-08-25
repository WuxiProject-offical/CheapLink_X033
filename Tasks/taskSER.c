/*
 *  Task_SER source file for firmware of CheapLink_X033
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

#if DAP_WITH_CDC
#include "FreeRTOS.h"
#include "task.h"
#include <ch32x035.h>
#include "ch32x035_usbfs_device.h"

volatile uint8_t CDC_linecoding[8];

#define CDCSER_QUEUEUP_LEN 4
#define CDCSER_QUEUEDOWN_LEN 4

__attribute__((aligned(4)))     volatile uint8_t serialBufUp[CDCSER_QUEUEUP_LEN][64];
__attribute__((aligned(4)))    volatile uint8_t serialBufDown[CDCSER_QUEUEDOWN_LEN][64];
static volatile uint8_t CDCSerial_UpLen[4], CDCSerial_DownLen[4];
static volatile uint8_t CDCSerial_UpPtrIn = 0, CDCSerial_UpPtrOut = 0,
		CDCSerial_UpCntIn = 0, CDCSerial_UpCntOut = 0;
static volatile uint8_t CDCSerial_DownPtrIn = 0, CDCSerial_DownPtrOut = 0,
		CDCSerial_DownCntIn = 0, CDCSerial_DownCntOut = 0;
static volatile uint8_t CDCSerial_UpIdle = 1, CDCSerial_DownIdle = 1;

extern volatile uint8_t USBFS_Endp_Busy[];

// Prepare data for IN endpoint upload.
uint8_t CDCSerial_EPUpload(uint8_t *buf, uint16_t len)
{
	while (USBFS_Endp_Busy[DEF_UEP3])
	{
		;
	}
	return USBFS_Endp_DataUp(DEF_UEP3, buf, len, DEF_UEP_DMA_LOAD);
}

// Prepare buffer for OUT transaction.
void CDCSerial_SetEPDNAddr(uint8_t *buffer)
{
	USBFSD->UEP5_DMA = (uint32_t) buffer;
}

// Control the OUT(downstream) endpoint ACK status.
void CDCSerial_SetEPDNAck(FunctionalState state)
{
	if (DISABLE == state)
	{
		// NAK
		USBFSD->UEP5_CTRL_H &= ~USBFS_UEP_R_RES_MASK;
		USBFSD->UEP5_CTRL_H |= USBFS_UEP_R_RES_NAK;
	}
	else
	{
		// ACK
		USBFSD->UEP5_CTRL_H &= ~USBFS_UEP_R_RES_MASK;
		USBFSD->UEP5_CTRL_H |= USBFS_UEP_R_RES_ACK;
	}
}

void CDCSerial_EpOUT_Handler(uint8_t len)
{
	//				Uart.Tx_PackLen[Uart.Tx_LoadNum] = USBFSD->RX_LEN;
	//				Uart.Tx_LoadNum++;
	//				USBFSD->UEP5_DMA =
	//						(uint32_t) (uint8_t *) &UART2_Tx_Buf[(Uart.Tx_LoadNum
	//								* DEF_USB_FS_PACK_LEN)];
	//				if (Uart.Tx_LoadNum >= DEF_UARTx_TX_BUF_NUM_MAX)
	//				{
	//					Uart.Tx_LoadNum = 0x00;
	//					USBFSD->UEP5_DMA = (uint32_t) (uint8_t *) &UART2_Tx_Buf[0];
	//				}
	//				Uart.Tx_RemainNum++;
	//
	//				if (Uart.Tx_RemainNum >= (DEF_UARTx_TX_BUF_NUM_MAX - 2))
	//				{
	//					USBFSD->UEP5_CTRL_H &= ~USBFS_UEP_R_RES_MASK;
	//					USBFSD->UEP5_CTRL_H |= USBFS_UEP_R_RES_NAK;
	//					Uart.USB_Down_StopFlag = 0x01;
	//				}

//	UQ_InLen[UQ_InPtrIn] = len;
//	UQ_InPtrIn++;
//	if (UQ_InPtrIn >= UQ_QUEUELEN) // loopback
//	{
//		UQ_InPtrIn = 0;
//	}
//	UQ_InCntIn++;
//	xTaskNotifyFromISR(taskHandleDAP, 0x01, eSetBits, NULL);
//	if ((uint8_t) (UQ_InCntIn - UQ_InCntOut) != UQ_QUEUELEN)
//	{
//		USBQueue_SetEPDNAddr(UQ_InQueue[UQ_InPtrIn]);
//		USBQueue_SetEPDNAck(ENABLE);
//	}
//	else
//	{
//		USBQueue_SetEPDNAck(DISABLE);
//		UQ_InIdle = 1;
//	}
}

void CDCSerial_EpIN_Handler()
{
	if (CDCSerial_UpCntIn != CDCSerial_UpCntOut)
	{
		// left packets in queue
		CDCSerial_EPUpload(serialBufUp[CDCSerial_UpPtrOut],
				CDCSerial_UpLen[CDCSerial_UpPtrOut]);
		CDCSerial_UpPtrOut++;
		if (CDCSerial_UpPtrOut >= CDCSER_QUEUEUP_LEN) // loopback
		{
			CDCSerial_UpPtrOut = 0U;
		}
		CDCSerial_UpCntOut++;
	}
	else
	{
		CDCSerial_UpIdle = 1;
	}
}

#define MEMCLEAR(x) (memset((x), 0x00, sizeof((x))))

void CDCSerial_QueueReset()
{
	MEMCLEAR(serialBufUp);
	MEMCLEAR(serialBufDown);
	MEMCLEAR(CDCSerial_UpLen);
	MEMCLEAR(CDCSerial_DownLen);
	CDCSerial_UpPtrIn = CDCSerial_UpPtrOut = 0;
	CDCSerial_UpCntIn = CDCSerial_UpCntOut = 0;
	CDCSerial_DownPtrIn = CDCSerial_DownPtrOut = 0;
	CDCSerial_DownCntIn = CDCSerial_DownCntOut = 0;
	CDCSerial_UpIdle = CDCSerial_DownIdle = 1;
	CDCSerial_SetEPDNAddr(serialBufUp[CDCSerial_UpPtrIn]);
	CDCSerial_SetEPDNAck(ENABLE);
	// If other reset operation required, process below.
}

/*
 * baudrate = USBFS_EP0_4Buf[0];
 baudrate += ((uint32_t) USBFS_EP0_4Buf[1] << 8);
 baudrate += ((uint32_t) USBFS_EP0_4Buf[2] << 16);
 baudrate += ((uint32_t) USBFS_EP0_4Buf[3] << 24);
 Uart.Com_Cfg[7] = Uart.Rx_TimeOutMax;
 */

__attribute__((aligned(4)))
     TaskHandle_t taskHandleSER = NULL;
void task_SER(void *pvParameters)
{
	vTaskSuspend(NULL);
	while (1)
	{
		;
	}
	vTaskDelete(NULL);
}
#endif

