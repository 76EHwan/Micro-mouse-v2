/*
 * sensor.h
 *
 *  Created on: Nov 10, 2025
 *      Author: kth59
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "vl53l4cx.h"

#define SENSOR_TIM &htim12

extern VL53L4CX_Object_t sensor[4];
extern VL53L4CX_Result_t result[4];
extern uint32_t distance[4];

VL53LX_Error Sensor_Init();
void Sensor_Start();
void Sensor_TIM_IRQ();

#endif /* SENSOR_H_ */
