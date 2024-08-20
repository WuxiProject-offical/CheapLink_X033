/*
 *  Main function file for firmware of CheapLink_X033
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

#include "debug.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ch32x035_usbfs_device.h"
#include "usbqueue.h" // 这个版本的USBQueue已经修改过。This version of USBQueue library has been modified.

#include "DAP_config.h"
#include "DAP.h"

__attribute__((aligned(4)))
TaskHandle_t taskHandleLED = NULL;
void task_LED(void *pvParameters)
{
	// vTaskSuspend(NULL);
	volatile uint8_t i = 0, LED_State = 0;
	volatile uint32_t notify = 0xffffffff, flashDelayTicks = 0;
	while (1)
	{
		switch (LED_State)
		{
			case 0x11:
			// Red Still
			GPIOC->BCR = GPIO_Pin_3;
			GPIOA->BSHR = GPIO_Pin_0 | GPIO_Pin_1;
			break;

			case 0x12:
			// Red Flash(1Hz)
			case 0x13:
			// Red Flash(5Hz)
			if (i)
			{
				i = 0;
				GPIOC->BSHR = GPIO_Pin_3;
			}
			else
			{
				i = 1;
				GPIOC->BCR = GPIO_Pin_3;
			}
			GPIOA->BSHR = GPIO_Pin_0 | GPIO_Pin_1;
			break;

			case 0x21:
			// Green Still
			GPIOA->BCR = GPIO_Pin_1;
			GPIOA->BSHR = GPIO_Pin_0;
			GPIOC->BSHR = GPIO_Pin_3;
			break;

			case 0x22:
			// Green Flash(1Hz)
			case 0x23:
			// Green Flash(5Hz)
			if (i)
			{
				i = 0;
				GPIOA->BSHR = GPIO_Pin_1;
			}
			else
			{
				i = 1;
				GPIOA->BCR = GPIO_Pin_1;
			}
			GPIOA->BSHR = GPIO_Pin_0;
			GPIOC->BSHR = GPIO_Pin_3;
			break;

			case 0x31:
			// Yellow Still
			GPIOA->BCR = GPIO_Pin_1;
			GPIOC->BCR = GPIO_Pin_3;
			GPIOA->BSHR = GPIO_Pin_0;
			break;

			case 0x32:
			// Yellow Flash(1Hz)
			case 0x33:
			// Yellow Flash(5Hz)
			if (i)
			{
				i = 0;
				GPIOA->BSHR = GPIO_Pin_1;
				GPIOC->BSHR = GPIO_Pin_3;
			}
			else
			{
				i = 1;
				GPIOA->BCR = GPIO_Pin_1;
				GPIOC->BCR = GPIO_Pin_3;
			}
			GPIOA->BSHR = GPIO_Pin_0;
			break;

			case 0x41:
			// Blue Still
			GPIOA->BCR = GPIO_Pin_0;
			GPIOA->BSHR = GPIO_Pin_1;
			GPIOC->BSHR = GPIO_Pin_3;
			break;

			case 0x42:
			// Blue Flash(1Hz)
			case 0x43:
			// Blue Flash(5Hz)
			if (i)
			{
				i = 0;
				GPIOA->BSHR = GPIO_Pin_0;
			}
			else
			{
				i = 1;
				GPIOA->BCR = GPIO_Pin_0;
			}
			GPIOA->BSHR = GPIO_Pin_1;
			GPIOC->BSHR = GPIO_Pin_3;
			break;

			case 0x51:
			// Magenta Still
			GPIOA->BCR = GPIO_Pin_0;
			GPIOC->BCR = GPIO_Pin_3;
			GPIOA->BSHR = GPIO_Pin_1;
			break;

			case 0x52:
			// Magenta Flash(1Hz)
			case 0x53:
			// Magenta Flash(5Hz)
			if (i)
			{
				i = 0;
				GPIOA->BSHR = GPIO_Pin_0;
				GPIOC->BSHR = GPIO_Pin_3;
			}
			else
			{
				i = 1;
				GPIOA->BCR = GPIO_Pin_0;
				GPIOC->BCR = GPIO_Pin_3;
			}
			GPIOA->BSHR = GPIO_Pin_1;
			break;

			case 0x61:
			// Cyan Still
			GPIOA->BCR = GPIO_Pin_0 | GPIO_Pin_1;
			GPIOC->BSHR = GPIO_Pin_3;
			break;

			case 0x62:
			// Cyan Flash(1Hz)
			case 0x63:
			// Cyan Flash(5Hz)
			if (i)
			{
				i = 0;
				GPIOA->BSHR = GPIO_Pin_0 | GPIO_Pin_1;
			}
			else
			{
				i = 1;
				GPIOA->BCR = GPIO_Pin_0 | GPIO_Pin_1;
			}
			GPIOC->BSHR = GPIO_Pin_3;
			break;

			case 0x71:
			// White Still
			GPIOA->BCR = GPIO_Pin_0 | GPIO_Pin_1;
			GPIOC->BCR = GPIO_Pin_3;
			break;

			case 0x72:
			// White Flash(1Hz)
			case 0x73:
			// White Flash(5Hz)
			if (i)
			{
				i = 0;
				GPIOA->BSHR = GPIO_Pin_0 | GPIO_Pin_1;
				GPIOC->BSHR = GPIO_Pin_3;
			}
			else
			{
				i = 1;
				GPIOA->BCR = GPIO_Pin_0 | GPIO_Pin_1;
				GPIOC->BCR = GPIO_Pin_3;
			}
			break;

			default:
			// Off
			GPIOA->BSHR = GPIO_Pin_0 | GPIO_Pin_1;
			GPIOC->BSHR = GPIO_Pin_3;
			break;
		}
		if (xTaskNotifyWait(0x0, 0xffffffffUL, &notify, flashDelayTicks) == pdTRUE)
		{
			i = 0;
			LED_State = notify & 0x000000ff;
			if ((LED_State & 0x0f) == 0x02)
			flashDelayTicks = pdMS_TO_TICKS(500);
			else if ((LED_State & 0x0f) == 0x03)
			flashDelayTicks = pdMS_TO_TICKS(200);
			else
			flashDelayTicks = 0;
			//printf("led %02x\r\n",LED_State);
		}
	}
	vTaskDelete(NULL);
}

__attribute__((aligned(4)))
TaskHandle_t taskHandleDAP = NULL;
void task_DAP(void *pvParameters)
{
	xTaskNotify(taskHandleLED, 0x32, eSetValueWithOverwrite); // LED: Yellow 1Hz
	volatile int32_t waitFlag;
	while (1)
	{
		waitFlag = xTaskNotifyWait(0x0, 0xffffffffUL, NULL, pdMS_TO_TICKS(5000));
		if(waitFlag == pdFALSE)
		{
			if (USBFS_DevEnumStatus && !(USBFSD->MIS_ST & USBFS_UMS_SUSPEND))
			{
				xTaskNotify(taskHandleLED, 0x31, eSetValueWithOverwrite); // LED: Yellow Still
			}
		}
		else
		{
			//xTaskNotify(taskHandleLED, 0x71, eSetValueWithOverwrite); // LED: White Still
			USBQueue_DoProcess();
		}
	}
	vTaskDelete(NULL);
}

extern void USBFS_IRQHandler(void) __attribute__((interrupt())) __attribute__((section(".highcode")));

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(921600);
	// SDI_Printf_Enable();
	printf("SystemClk:%d\r\n", SystemCoreClock);
	printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());

	char snbuf[9];
	snprintf(snbuf, 9, "%08X", (X035CHIPSN1 ^ ~X035CHIPSN2));
	for (uint8_t i = 0; i < 8; i++)
	{
		MySerNumInfo[12 + 2 * i] = snbuf[i];
	}
	USBFS_RCC_Init();
	USBFS_Device_Init(ENABLE, PWR_VDD_3V3);
	SetVTFIRQ((u32) USBFS_IRQHandler, USBFS_IRQn, 0, ENABLE);
	NVIC_EnableIRQ(USBFS_IRQn);
	DAP_Setup();

	xTaskCreate((TaskFunction_t) task_LED, (const char *) "LED", (uint16_t) 128,
			(void *) NULL, (UBaseType_t) 1, (TaskHandle_t *) &taskHandleLED);

	xTaskCreate((TaskFunction_t) task_DAP, (const char *) "DAP", (uint16_t) 256,
			(void *) NULL, (UBaseType_t) 3, (TaskHandle_t *) &taskHandleDAP);

	vTaskStartScheduler();

	while (1)
	{
		;
	}
}
