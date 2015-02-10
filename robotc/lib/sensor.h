/**********************************************************

Sensors
sensor.h

Basic Sensor task

**********************************************************/

#ifndef SENSOR_H
#define SENSOR_H

/***** Includes *****/
#include "gyro.h" //gyroscope
#include "ir.h" //ir sensor
#include "touch.h" //touch sensor
/***** LOCAL VARIABLES *****/
static bool gyro_enabled = false;
static bool ir_enabled = false;
static bool touch_enabled = false;
/***** METHODS *****/

/**
* Gyro initialization function
* @param The gyro sensor.
*/
void gyro_init(tSensors gyro)
{
	gyro_init_internal(gyro);
	gyro_enabled = true;
}
/**
* IR initialization function
* @param The ir sensor.
*/
void ir_init(tSensors ir)
{
	ir_init_internal(ir);
	ir_enabled = true;
}
void ir_init(tMUXSensor ir)
{
	ir_init_internal(ir);
	ir_enabled = true;
}
void touch_init(tSensors touch)
{
	touch_init_internal(touch);
	touch_enabled = true;
}
void touch_init(tMUXSensor touch)
{
	touch_init_internal(touch);
	touch_enabled = true;
}
task readSensors {
	while (true){
		wait1Msec(20);
		if (gyro_enabled){
			updateGyro();
		}
		wait1Msec(20);
		if (ir_enabled){
			updateIR();
		}
		wait1Msec(20);
		if (touch_enabled){
		  updateTouch();
	  }
	}
}

#endif
