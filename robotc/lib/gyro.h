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

float gyro_init(tSensors link)
{
    //Read gyro offset
    return HTGYROstartCal(link);
}
