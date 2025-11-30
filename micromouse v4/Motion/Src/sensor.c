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
	HAL_Delay(50); // 전압이 완전히 떨어질 때까지 충분히 대기

	for (uint8_t i = 0; i < 4; i++) {
		// 2. 해당 센서 켜기
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

		// 3. 센서 부팅 대기 (중요: 데이터시트 상 1.2ms지만 여유 있게)
		HAL_Delay(50);

		// 4. 기본 주소(0x52)로 센서가 살아있는지 확인 (Ping)
		// 여기서 HAL_OK가 안 나오면 하드웨어 연결 문제임
		if (HAL_I2C_IsDeviceReady(&hi2c2, 0x52, 2, 10) != HAL_OK) {
			Custom_LCD_Printf(0, i + 1, "S%d: MISSING(0x52)", i);
			// 다음 센서를 위해 루프를 계속할지, 여기서 멈출지 결정
			// continue;
			return VL53LX_ERROR_CONTROL_INTERFACE; // 디버깅을 위해 멈춤
		}

		// 5. 센서 초기화 (드라이버 로드)
		if (VL53L4A2_RANGING_SENSOR_Init(i) != BSP_ERROR_NONE) {
			Custom_LCD_Printf(0, i + 1, "S%d: Init Fail", i);
			return VL53LX_ERROR_CONTROL_INTERFACE;
		}

		// 6. 주소 변경 (0x52 -> 새로운 주소)
		if (VL53L4A2_RANGING_SENSOR_SetAddress(i,
				sensor_address[i]) != BSP_ERROR_NONE) {
			Custom_LCD_Printf(0, i + 1, "S%d: Addr Fail", i);
			return VL53LX_ERROR_CONTROL_INTERFACE;
		}

		// 7. 변경된 주소로 잘 살아있는지 최종 확인
		if (HAL_I2C_IsDeviceReady(&hi2c2, sensor_address[i], 2, 10) == HAL_OK) {
			Custom_LCD_Printf(0, i + 1, "S%d: OK (0x%02X)", i,
					sensor_address[i]);
		} else {
			Custom_LCD_Printf(0, i + 1, "S%d: Lost (0x%02X)", i,
					sensor_address[i]);
		}
	}
	return VL53L4CX_OK;
}

void Sensor_Start() {
	VL53L4A2_RANGING_SENSOR_Start(0, RS_MODE_ASYNC_CONTINUOUS);
	VL53L4A2_RANGING_SENSOR_Start(1, RS_MODE_ASYNC_CONTINUOUS);
	VL53L4A2_RANGING_SENSOR_Start(2, RS_MODE_ASYNC_CONTINUOUS);
	VL53L4A2_RANGING_SENSOR_Start(3, RS_MODE_ASYNC_CONTINUOUS);
}
