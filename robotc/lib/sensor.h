/**********************************************************

Gyroscopic Drive
gyro.h

Implements the HiTechnic Gyroscope.

**********************************************************/

#ifndef SENSOR_H
#define SENSOR_H

/***** Includes *****/
#include "../../lib/gyro.h" //gyroscope
#include "../../lib/ir.h" //gyroscope
/***** LOCAL VARIABLES *****/
static bool gyro_enabled = false;
static bool ir_enabled = false;

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
task readSensors {
	while (true){
		wait1Msec(20);
		if (gyro_enabled){
			updateGyro();
		}
		if (ir_enabled){
			updateIR();
		}
	}
}

#endif
