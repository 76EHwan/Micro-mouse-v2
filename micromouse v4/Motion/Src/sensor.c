/*
 * sensor.c
 *
 *  Created on: Nov 10, 2025
 *      Author: kth59
 */

#include "sensor.h"
#include "lcd.h"
#include "gpio.h"
#include "tim.h"
#include "53l4a2_ranging_sensor.h"

#define SENSOR_MUX_XSHUT ((SNR_MUX_GPIO_Port)->BRR = (uint32_t) (SNR_MUX_Pin))
#define SENSOR_MUX_GPIO0 ((SNR_MUX_GPIO_Port)->BSRR = (uint32_t) (SNR_MUX_Pin))

#define SENSOR_XSHUT0_ON ((SNR0_GPIO_Port)->BSRR = (uint32_t)(SNR0_Pin))
#define SENSOR_XSHUT1_ON ((SNR1_GPIO_Port)->BSRR = (uint32_t)(SNR1_Pin))
#define SENSOR_XSHUT2_ON ((SNR2_GPIO_Port)->BSRR = (uint32_t)(SNR2_Pin))
#define SENSOR_XSHUT3_ON ((SNR3_GPIO_Port)->BSRR = (uint32_t)(SNR3_Pin))

#define SENSOR_XSHUT0_OFF ((SNR0_GPIO_Port)->BRR = (uint32_t)(SNR0_Pin))
#define SENSOR_XSHUT1_OFF ((SNR1_GPIO_Port)->BRR = (uint32_t)(SNR1_Pin))
#define SENSOR_XSHUT2_OFF ((SNR2_GPIO_Port)->BRR = (uint32_t)(SNR2_Pin))
#define SENSOR_XSHUT3_OFF ((SNR3_GPIO_Port)->BRR = (uint32_t)(SNR3_Pin))

VL53L4CX_Object_t TOF_Sensor[4];
VL53L4CX_IO_t TOF_IO;
VL53L4CX_Result_t TOF_Result[4];
VL53L4CX_ITConfig_t TOF_ITConfig[4];
bool isTofReady[4];

const uint16_t sensor_address[4] = { 0x54, 0x56, 0x58, 0x5A };

VL53LX_Error Sensor_Init() {
	SENSOR_MUX_XSHUT;
	SENSOR_XSHUT0_OFF;
	SENSOR_XSHUT1_OFF;
	SENSOR_XSHUT2_OFF;
	SENSOR_XSHUT3_OFF;

	for (uint8_t i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			SENSOR_XSHUT0_ON;
			break;
		case 1:
			SENSOR_XSHUT1_ON;
			break;
		case 2:
			SENSOR_XSHUT2_ON;
			break;
		case 3:
			SENSOR_XSHUT3_ON;
			break;
		}
		HAL_Delay(10);
		if (VL53L4A2_RANGING_SENSOR_Init(i) != BSP_ERROR_NONE) {
			Custom_LCD_Printf(0, i, "Init Fail %d", i);
			continue;
		}
		if (VL53L4A2_RANGING_SENSOR_SetAddress(i, sensor_address[i]) != BSP_ERROR_NONE) {
			Custom_LCD_Printf(80, i, "Addr Fail %d", i);
		}
	}

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = SNR0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SNR0_GPIO_Port, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI11_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI11_IRQn);

	GPIO_InitStruct.Pin = SNR1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SNR1_GPIO_Port, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI11_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI11_IRQn);

	GPIO_InitStruct.Pin = SNR2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SNR2_GPIO_Port, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI11_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI11_IRQn);

	GPIO_InitStruct.Pin = SNR3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SNR3_GPIO_Port, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI11_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI11_IRQn);

	return VL53L4CX_OK;
}

void Sensor_Start() {
	VL53L4A2_RANGING_SENSOR_Start(0, RS_MODE_ASYNC_CONTINUOUS);
	VL53L4A2_RANGING_SENSOR_Start(1, RS_MODE_ASYNC_CONTINUOUS);
	VL53L4A2_RANGING_SENSOR_Start(2, RS_MODE_ASYNC_CONTINUOUS);
	VL53L4A2_RANGING_SENSOR_Start(3, RS_MODE_ASYNC_CONTINUOUS);
}
