/*
 * sensor.c
 *
 *  Created on: Nov 10, 2025
 *      Author: kth59
 */

#include "sensor.h"
#include "gpio.h"
#include "tim.h"

#define SENSOR_MUX_XSHUT ((SNR_MUX_GPIO_Port)->BSRR = (uint32_t) (SNR_MUX_Pin) << 16)
#define SENSOR_MUX_GPIO0 ((SNR_MUX_GPIO_Port)->BSRR = (uint32_t) (SNR_MUX_Pin))

#define SENSOR_XSHUT0_ON ((SNR0_GPIO_Port)->BSRR = (uint32_t)(SNR0_Pin))
#define SENSOR_XSHUT1_ON ((SNR1_GPIO_Port)->BSRR = (uint32_t)(SNR1_Pin))
#define SENSOR_XSHUT2_ON ((SNR2_GPIO_Port)->BSRR = (uint32_t)(SNR2_Pin))
#define SENSOR_XSHUT3_ON ((SNR3_GPIO_Port)->BSRR = (uint32_t)(SNR3_Pin))

VL53L4CX_Object_t sensor[4];
VL53L4CX_Result_t result[4];
uint32_t distance[4];

const uint16_t sensor_address[4] = { 0x54, 0x56, 0x58, 0x5A };

VL53LX_Error Sensor_Init() {
	SENSOR_MUX_XSHUT;
	int32_t status;

	SENSOR_XSHUT0_ON;
	HAL_Delay(10);
	VL53L4CX_Init(&sensor[0]);
	status = VL53L4CX_SetAddress(&sensor[0], sensor_address[0]);
	if (status != VL53L4CX_OK)
		return status;

	SENSOR_XSHUT1_ON;
	HAL_Delay(10);
	VL53L4CX_Init(&sensor[1]);
	status = VL53L4CX_SetAddress(&sensor[1], sensor_address[1]);
	if (status != VL53L4CX_OK)
		return status;

	SENSOR_XSHUT2_ON;
	HAL_Delay(10);
	VL53L4CX_Init(&sensor[2]);
	status = VL53L4CX_SetAddress(&sensor[2], sensor_address[2]);
	if (status != VL53L4CX_OK)
		return status;

	SENSOR_XSHUT3_ON;
	HAL_Delay(10);
	VL53L4CX_Init(&sensor[3]);
	status = VL53L4CX_SetAddress(&sensor[3], sensor_address[3]);
	if (status != VL53L4CX_OK)
		return status;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SNR0_Pin; // 예: 5번 핀을 대상으로 함
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;    // 모드를 Input으로 변경
	GPIO_InitStruct.Pull = GPIO_PULLUP;        // 또는 GPIO_NOPULL, GPIO_PULLDOWN
	HAL_GPIO_Init(SNR0_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SNR1_Pin; // 예: 5번 핀을 대상으로 함
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;    // 모드를 Input으로 변경
	GPIO_InitStruct.Pull = GPIO_PULLUP;        // 또는 GPIO_NOPULL, GPIO_PULLDOWN
	HAL_GPIO_Init(SNR1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SNR2_Pin; // 예: 5번 핀을 대상으로 함
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;    // 모드를 Input으로 변경
	GPIO_InitStruct.Pull = GPIO_PULLUP;        // 또는 GPIO_NOPULL, GPIO_PULLDOWN
	HAL_GPIO_Init(SNR2_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SNR3_Pin; // 예: 5번 핀을 대상으로 함
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;    // 모드를 Input으로 변경
	GPIO_InitStruct.Pull = GPIO_PULLUP;        // 또는 GPIO_NOPULL, GPIO_PULLDOWN
	HAL_GPIO_Init(SNR3_GPIO_Port, &GPIO_InitStruct);
	return status;
}

void Sensor_Start(){
	VL53L4CX_Start(&sensor[0], VL53L4CX_MODE_ASYNC_CONTINUOUS);
	VL53L4CX_Start(&sensor[1], VL53L4CX_MODE_ASYNC_CONTINUOUS);
	VL53L4CX_Start(&sensor[2], VL53L4CX_MODE_ASYNC_CONTINUOUS);
	VL53L4CX_Start(&sensor[3], VL53L4CX_MODE_ASYNC_CONTINUOUS);

	HAL_TIM_Base_Start_IT(SENSOR_TIM);
}


void Sensor_TIM_IRQ() {
	static uint8_t i = 0;
	VL53L4CX_GetDistance(&sensor[i], &result[i]);
	distance[i] = result[i].ZoneResult[0].Distance[0];
	i = (i + 1) & 0x3;
}
