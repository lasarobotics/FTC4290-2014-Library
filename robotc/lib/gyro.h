/**********************************************************

Gyroscopic and Field Oriented Drive
gyro.h

Uses HiTechnic Gyroscope and implements
full Field Oriented Drive.

**********************************************************/

#include "../drivers/hitechnic-gyro.h" //gyroscope

/***** VARIABLES *****/
static float rotSpeed = 0;
static float heading = 0;
static float firstrot = 0;
static tSensors gyro = 0;

float gyro_getrotspeed() { return rotSpeed; }
float gyro_getheading() { return heading; }
float gyro_getfirstrot() { return firstrot; }

float gyro_init(tSensors link)
{
    //Read gyro offset
    gyro = link;
    return HTGYROstartCal(link);
}

void gyro_reset() { heading = 0; }

task gyro_calibrate()
{
    while (true)
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
        if (heading < 0) { heading += 360; }
    }
}
