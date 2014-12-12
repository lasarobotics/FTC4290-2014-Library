/**********************************************************

Gyroscopic Drive
gyro.h

Implements the HiTechnic Gyroscope.

**********************************************************/

#ifndef SENSOR_H
#define SENSOR_H

/***** LOCAL VARIABLES *****/
static bool gyro_enabled = false;
static bool ir_enabled = false;

/***** METHODS *****/

/**
* Gyro initialization function
* @param The gyro sensor.
*/
#ifdef _GYROSCOPE_ENABLED
void gyro_init(tSensors gyro)
{
	gyro_init_internal(gyro);
	gyro_enabled = true;
}
#endif
/**
* IR initialization function
* @param The ir sensor.
*/
#ifdef IR_H
void ir_init(tSensors ir)
{
	ir_init_internal(ir);
	ir_enabled = true;
}
#endif
task readSensors {
	while (true){
		wait1Msec(20);
		#ifdef _GYROSCOPE_ENABLED
		if (gyro_enabled){
			updateGyro();
		}
		#endif
		wait1Msec(20);
		#ifdef IR_H
		if (ir_enabled){
			updateIR();
		}
		#endif
	}
}

#endif
