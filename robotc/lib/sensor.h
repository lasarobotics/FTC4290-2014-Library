/**********************************************************

Gyroscopic Drive
gyro.h

Implements the HiTechnic Gyroscope.

**********************************************************/

#include "gyro.h"
#include "ir.h"
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

task readSensors{
    while (true){
        wait1Msec(20);
        if (gyro_enabled){
            updateGyro();
        }
        wait1Msec(20);
        if (ir_enabled){
            updateIR();
        }
    }
}
