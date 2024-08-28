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

TaskHandle_t taskHandleSER __attribute__((aligned(4)));

#define CDCSER_QUEUEUP_LEN 4
#define CDCSER_QUEUEDOWN_LEN 4

volatile uint8_t serialBufUp[CDCSER_QUEUEUP_LEN][64] __attribute__((aligned(4)));
volatile uint8_t serialBufDown[CDCSER_QUEUEDOWN_LEN][64] __attribute__((aligned(4)));
static volatile uint8_t CDCSerial_UpLen[4], CDCSerial_DownLen[4];
static volatile uint8_t CDCSerial_UpPtrIn = 0, CDCSerial_UpPtrOut = 0;
static volatile uint8_t CDCSerial_UpCntIn = 0, CDCSerial_UpCntOut = 0;
static volatile uint8_t CDCSerial_DownPtrIn = 0, CDCSerial_DownPtrOut = 0;
static volatile uint8_t CDCSerial_DownCntIn = 0, CDCSerial_DownCntOut = 0;
static volatile uint8_t CDCSerial_UpIdleIn = 1, CDCSerial_UpIdleOut = 1;
static volatile uint8_t CDCSerial_DownIdleIn = 1, CDCSerial_DownIdleOut = 1;

static volatile uint32_t CDCSerial_LastUpLen = 0;

extern volatile uint8_t USBFS_Endp_Busy[];

// Prepare data for IN endpoint upload.
uint8_t CDCSerial_EPUpload(uint8_t *buf, uint16_t len)
{
	CDCSerial_LastUpLen = len;
	return USBFS_Endp_DataUp(DEF_UEP3, buf, len, DEF_UEP_DMA_LOAD);
}

// Prepare buffer for OUT transaction.
void CDCSerial_SetEPDNAddr(uint8_t *buffer)
{
	USBFSD->UEP5_DMA = (uint32_t)buffer;
}

// Control the OUT(downstream) endpoint ACK status.
void CDCSerial_SetEPDNAck(FunctionalState state)
{
	if(DISABLE == state)
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
	if(CDCSerial_UpCntIn != CDCSerial_UpCntOut)
	{
		// left packets in queue
		CDCSerial_EPUpload(serialBufUp[CDCSerial_UpPtrOut], CDCSerial_UpLen[CDCSerial_UpPtrOut]);
		CDCSerial_UpPtrOut++;
		if(CDCSerial_UpPtrOut >= CDCSER_QUEUEUP_LEN) // loopback
		{
			CDCSerial_UpPtrOut = 0U;
		}
		CDCSerial_UpCntOut++;
	}
	else
	{
		// empty queue
		if(CDCSerial_LastUpLen == 64)
		{
			// need another zero-len packet
			CDCSerial_EPUpload(serialBufUp[CDCSerial_UpPtrOut - 1], 0);
		}
		CDCSerial_UpIdleOut = 1;
	}
	if(CDCSerial_UpIdleIn) // if UART paused
	{
		if((uint8_t)(CDCSerial_UpCntIn - CDCSerial_UpCntOut) != CDCSER_QUEUEUP_LEN) // if Queue not full
		{
			// start UART recv
			CDCSerial_UpIdleIn = 0U;
			DMA_SetCurrDataCounter(DMA1_Channel6, 64U);
			DMA1_Channel6->MADDR = (uint32_t)(serialBufUp[CDCSerial_UpPtrIn]);
			DMA_Cmd(DMA1_Channel6, ENABLE);
		}
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
	CDCSerial_UpIdleIn = CDCSerial_UpIdleOut = 1;
	CDCSerial_DownIdleIn = CDCSerial_DownIdleOut = 1;
	CDCSerial_SetEPDNAddr(serialBufUp[CDCSerial_DownPtrIn]);
	CDCSerial_SetEPDNAck(ENABLE);
	// If other reset operation required, process below.
}

void USART2_IRQHandler(void) __attribute__((interrupt())) __attribute__((section(".highcode")));
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		// Idle
		if(CDCSerial_UpIdleIn)
		{
			;
		}
		else
		{
			if(DMA1_Channel6->CNTR > 0)
			{
				// only handle unfinished DMA
				DMA1_Channel6->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
				DMA1->INTFCR = DMA1_IT_TC6;
				uint16_t rxCnt = (64U - DMA1_Channel6->CNTR);
				CDCSerial_UpLen[CDCSerial_UpPtrIn] = rxCnt;
				CDCSerial_UpPtrIn++;
				if(CDCSerial_UpPtrIn >= CDCSER_QUEUEUP_LEN) // loopback
				{
					CDCSerial_UpPtrIn = 0;
				}
				CDCSerial_UpCntIn++;
				if((uint8_t)(CDCSerial_UpCntIn - CDCSerial_UpCntOut) != CDCSER_QUEUEUP_LEN)
				{
					// Queue not full
					DMA_SetCurrDataCounter(DMA1_Channel6, 64U);
					DMA1_Channel6->MADDR = (uint32_t)(serialBufUp[CDCSerial_UpPtrIn]);
					DMA_Cmd(DMA1_Channel6, ENABLE);
				}
				else
				{
					CDCSerial_UpIdleIn = 1;
				}
			}
		}
		// Clear flag
		(void)USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_PE) != RESET)
	{
		// Parity Err

		// Clear flag
		(void)USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_ORE_ER))
	{
		// Overrun

		// Clear flag
		(void)USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_NE))
	{
		// Noise

		// Clear flag
		(void)USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_FE))
	{
		// Frame err

		// Clear flag
		(void)USART_ReceiveData(USART2);
	}
}

// DMA RX2
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt())) __attribute__((section(".highcode")));
void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC6) != RESET)
	{
		// TC
		DMA1_Channel6->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
		DMA1->INTFCR = DMA1_IT_TC6;
		CDCSerial_UpCntIn++;
		if((uint8_t)(CDCSerial_UpCntIn - CDCSerial_UpCntOut) != CDCSER_QUEUEUP_LEN)
		{
			// Queue not full
			DMA1_Channel6->CNTR = 64U;
			DMA1_Channel6->MADDR = (uint32_t)(serialBufUp[CDCSerial_UpPtrIn]);
			DMA1_Channel6->CFGR |= DMA_CFGR1_EN;
		}
		else
		{
			CDCSerial_UpIdleIn = 1;
		}
		CDCSerial_UpLen[CDCSerial_UpPtrIn] = 64U;
		CDCSerial_UpPtrIn++;
		if(CDCSerial_UpPtrIn >= CDCSER_QUEUEUP_LEN) // loopback
		{
			CDCSerial_UpPtrIn = 0;
		}
		if(CDCSerial_UpIdleOut) // if USB Idle
		{
			if(CDCSerial_UpCntIn != CDCSerial_UpCntOut) // if Queue not empty
			{
				CDCSerial_UpIdleOut = 0U;
				CDCSerial_EPUpload(serialBufUp[CDCSerial_UpPtrOut], CDCSerial_UpLen[CDCSerial_UpPtrOut]);
				CDCSerial_UpPtrOut++;
				if(CDCSerial_UpPtrOut >= CDCSER_QUEUEUP_LEN) // loopback
				{
					CDCSerial_UpPtrOut = 0U;
				}
				CDCSerial_UpCntOut++;
			}
		}
	}
}

// // DMA_TX2
// void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt())) __attribute__((section(".highcode")));
// void DMA1_Channel7_IRQHandler(void)
// {
//	 if (DMA_GetITStatus(DMA1_IT_TC7) != RESET)
//	 	{
//	 		// TC
//	 		DMA1_Channel7->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
//	 		DMA1->INTFCR = DMA1_IT_TC7;
//
//	 	}
// }

void CDCSerial_InitUART(uint32_t baudrate, uint16_t databit, uint16_t paritybit,
                        uint16_t stopbit)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// Init GPIO
	GPIO_InitTypeDef GPIO_InitStructure =
	            { 0 };
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Init UART
	USART_InitTypeDef USART_InitStructure =
	            { 0 };
	USART_DeInit(USART2);
	(void)(USART2->STATR);
	(void)(USART2->DATAR);
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = databit;
	USART_InitStructure.USART_StopBits = stopbit;
	USART_InitStructure.USART_Parity = paritybit;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART2, USART_IT_PE, ENABLE);
	USART_ITConfig(USART2, USART_IT_ERR, ENABLE);
	NVIC_SetPriority(USART2_IRQn, 6);
	NVIC_EnableIRQ(USART2_IRQn);
	USART_Cmd(USART2, ENABLE);

	// Init DMA
	DMA_InitTypeDef DMA_InitStructure =
	            { 0 };
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DATAR);
	DMA_InitStructure.DMA_MemoryBaseAddr =
	        (uint32_t)serialBufUp[CDCSerial_UpPtrIn];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 64U;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel6, DMA_IT_HT, DISABLE);
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(DMA1_IT_GL6);
	DMA_ClearITPendingBit(DMA1_IT_TC6);
	DMA_ClearITPendingBit(DMA1_IT_HT6);
	NVIC_SetPriority(DMA1_Channel6_IRQn, 5);
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	DMA_Cmd(DMA1_Channel6, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
}

void task_SER(void *pvParameters)
{
	int32_t waitFlag;
	uint32_t notifyFlag;
	// vTaskSuspend(NULL);
	while (1)
	{
		waitFlag = xTaskNotifyWait(0x0, 0xffffffffUL, &notifyFlag, portMAX_DELAY);
		if (notifyFlag & 0x00000001UL)
		{
			;
		}
	}
	vTaskDelete(NULL);
}
#endif
