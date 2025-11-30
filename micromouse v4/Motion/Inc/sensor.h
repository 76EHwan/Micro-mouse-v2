/*
 * sensor.h
 *
 *  Created on: Nov 10, 2025
 *      Author: kth59
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdbool.h>
#include "vl53l4cx.h"

#define SENSOR_TIM &htim12

extern VL53L4CX_Object_t TOF_Sensor[4];
extern VL53L4CX_IO_t TOF_IO;
extern VL53L4CX_Result_t TOF_Result[4];
extern VL53L4CX_ITConfig_t TOF_ITConfig[4];
extern bool isTofReady[4];

VL53LX_Error Sensor_Init();
void Sensor_Start();

#endif /* SENSOR_H_ */
