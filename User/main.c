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

void task_Blink(void *pvParameters)
{
	u8 i = 0;
	GPIO_InitTypeDef GPIO_InitStructure =
	{ 0 };

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(500));
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
	}
	vTaskDelete( NULL);
}

TaskHandle_t taskHandleDAP = NULL;
void task_DAP(void *pvParameters)
{
	while(1)
	{
		xTaskNotifyWait(0x0, 0xffffffffUL, NULL, portMAX_DELAY);
		USBQueue_DoProcess();
	}
	vTaskDelete( NULL);
}

extern void USBFS_IRQHandler(void) __attribute__((interrupt())) __attribute__((section(".highcode")));

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(921600);
	//SDI_Printf_Enable();
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

	xTaskCreate((TaskFunction_t) task_Blink, (const char*) "Blink",
			(uint16_t) 64, (void*) NULL, (UBaseType_t) 1, (TaskHandle_t*) NULL);

	xTaskCreate((TaskFunction_t) task_DAP, (const char*) "DAP", (uint16_t) 256,
			(void*) NULL, (UBaseType_t) 7, (TaskHandle_t*) &taskHandleDAP);

	vTaskStartScheduler();

	while(1)
	{;}
}
