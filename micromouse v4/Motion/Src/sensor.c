/*
 * sensor.c
 *
 *  Created on: Nov 10, 2025
 *      Author: kth59
 */

#include "sensor.h"
#include "gpio.h"
#include "tim.h"

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

const uint16_t sensor_address[4] = { 0x54, 0x56, 0x58, 0x5A };

VL53LX_Error Sensor_Init() {
	SENSOR_MUX_XSHUT;
	SENSOR_XSHUT0_OFF;
	SENSOR_XSHUT1_OFF;
	SENSOR_XSHUT2_OFF;
	SENSOR_XSHUT3_OFF;

	SENSOR_XSHUT0_ON;
	TOF_IO.Address = VL53L4CX_DEVICE_ADDRESS; // 0x52
	TOF_IO.Init = NULL;
	TOF_IO.DeInit = NULL;
	TOF_IO.WriteReg = TOF_WriteReg;
	TOF_IO.ReadReg = TOF_ReadReg;
	TOF_IO.GetTick = TOF_GetTick;
	if (VL53L4CX_RegisterBusIO(TOF_Sensor, &TOF_IO) != VL53L4CX_OK)
		while (1)
			;
	if (VL53L4CX_Init(TOF_Sensor) != VL53L4CX_OK)
		while (1)
			;

	SENSOR_XSHUT1_ON;
	TOF_IO.Address = VL53L4CX_DEVICE_ADDRESS + 2; // 0x54
	TOF_IO.Init = NULL;
	TOF_IO.DeInit = NULL;
	TOF_IO.WriteReg = TOF_WriteReg;
	TOF_IO.ReadReg = TOF_ReadReg;
	TOF_IO.GetTick = TOF_GetTick;
	if (VL53L4CX_RegisterBusIO(TOF_Sensor + 1, &TOF_IO) != VL53L4CX_OK)
		while (1)
			;
	if (VL53L4CX_Init(TOF_Sensor + 1) != VL53L4CX_OK)
		while (1)
			;

	SENSOR_XSHUT2_ON;
	TOF_IO.Address = VL53L4CX_DEVICE_ADDRESS + 4; // 0x56
	TOF_IO.Init = NULL;
	TOF_IO.DeInit = NULL;
	TOF_IO.WriteReg = TOF_WriteReg;
	TOF_IO.ReadReg = TOF_ReadReg;
	TOF_IO.GetTick = TOF_GetTick;
	if (VL53L4CX_RegisterBusIO(TOF_Sensor + 2, &TOF_IO) != VL53L4CX_OK)
		while (1)
			;
	if (VL53L4CX_Init(TOF_Sensor + 2) != VL53L4CX_OK)
		while (1)
			;

	SENSOR_XSHUT3_ON;
	TOF_IO.Address = VL53L4CX_DEVICE_ADDRESS + 6; // 0x58
	TOF_IO.Init = NULL;
	TOF_IO.DeInit = NULL;
	TOF_IO.WriteReg = TOF_WriteReg;
	TOF_IO.ReadReg = TOF_ReadReg;
	TOF_IO.GetTick = TOF_GetTick;
	if (VL53L4CX_RegisterBusIO(TOF_Sensor + 3, &TOF_IO) != VL53L4CX_OK)
		while (1)
			;
	if (VL53L4CX_Init(TOF_Sensor + 3) != VL53L4CX_OK)
		while (1)
			;

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
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

	return VL53L4CX_OK;
}

void Sensor_Start() {
	VL53LX_ClearInterruptAndStartMeasurement((TOF_Sensor + 0)->Dev);
	VL53LX_ClearInterruptAndStartMeasurement((TOF_Sensor + 1)->Dev);
	VL53LX_ClearInterruptAndStartMeasurement((TOF_Sensor + 2)->Dev);
	VL53LX_ClearInterruptAndStartMeasurement((TOF_Sensor + 3)->Dev);

	VL53L4CX_Start(TOF_Sensor + 0, VL53L4CX_MODE_ASYNC_CONTINUOUS);
	VL53L4CX_Start(TOF_Sensor + 1, VL53L4CX_MODE_ASYNC_CONTINUOUS);
	VL53L4CX_Start(TOF_Sensor + 2, VL53L4CX_MODE_ASYNC_CONTINUOUS);
	VL53L4CX_Start(TOF_Sensor + 3, VL53L4CX_MODE_ASYNC_CONTINUOUS);

	HAL_TIM_Base_Start_IT(SENSOR_TIM);
}

void Sensor_TIM_IRQ() {
	static uint8_t i = 0;
	VL53L4CX_GetDistance(&sensor[i], &result[i]);
	distance[i] = result[i].ZoneResult[0].Distance[0];
	i = (i + 1) & 0x3;
}
