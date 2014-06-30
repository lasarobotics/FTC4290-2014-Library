/**********************************************************

Gyroscopic and Field Oriented Drive
gyro.h

Uses HiTechnic Gyroscope and implements
full Field Oriented Drive.

**********************************************************/

#include "../drivers/hitechnic-gyro.h" //gyroscope

/***** LOCAL VARIABLES *****/
static float rotSpeed = 0;
static float heading = 0;
static float firstrot = 0;
static tSensors gyro = 0;
static bool go = true;

/***** QUICK METHODS *****/

//Get current rotational speed (in deg/s)
float gyro_getrotspeed() { return rotSpeed; }
//Get current heading from -360 to 360 degrees
float gyro_getheading() { return heading; }
//Get first derivative offset (in deg)
float gyro_getfirstoffset() { return firstrot; }
//Reset gyro heading to zero
void gyro_reset() { heading = 0; }
//Stop gyro calibration task
void gyro_kill() { go = false; }

/***** METHODS *****/

/**
* Gyro initialization function
* @param The gyro sensor.
*/
float gyro_init(tSensors link)
{
    //Read gyro offset
    gyro = link;
    return HTGYROstartCal(link);
}

/**
* Gyro calibration task
* Call before your while(true) loop with priority 8
*/
task gyro_calibrate()
{
    while (go)
    {
        // Wait until 20ms has passed
        while (time1[T1] < 20)
          wait1Msec(1);

        // Reset the timer
        time1[T1]=0;

        // Read the current rotation speed
        rotSpeed = HTGYROreadRot(gyro);
        if ((rotSpeed != 0) && (firstrot == 0) && (abs(rotSpeed) < 1))
        {
            if (sgn(rotSpeed) == 1) {firstrot = rotSpeed;}
            if (sgn(rotSpeed) == -1) {firstrot = rotSpeed + 1;}
        }

        if (sgn(rotSpeed) == 1) {rotSpeed = rotSpeed - firstrot;}
        if (sgn(rotSpeed) == -1) {rotSpeed = rotSpeed - firstrot + 1;}

        if ((abs(rotSpeed) <= 1) && (firstrot != 0)) { rotSpeed = 0; }
        if (abs(rotSpeed) < 1) { rotSpeed = 0; }

        // Calculate the new heading by adding the amount of degrees
        // we've turned in the last 20ms
        // If our current rate of rotation is 100 degrees/second,
        // then we will have turned 100 * (20/1000) = 2 degrees since
        // the last time we measured.
        heading += rotSpeed * 0.02;
        if (heading > 360) { heading -= 360; }
        if (heading < -360) { heading += 360; }
    }
}
