/**********************************************************

Drive Train Mathematical Operation
drivemath.h

Mathematics required for drive trains to function.

**********************************************************/

#include "joystick.h"
#include "motor.h"

/**
* Provide exponential rampup and rampdown.
* @param value The value of the left stick.
* @param time Time since zero or value == outvalue
* @param max Output, is the value at maximum power?  Use this to reset timers.
* @param min Output, is the value at minimum power?  Use this to reset timers.
* @return Scaled drive power
*/
float ramp_exp(float value, float time, bool& max, bool& min)
{
    float a = (float)0.01 * (pow(1.031177275, time));
    if (a >= 1) { a = 1; max=true; } else { max=false; }
    if (a <= 0.01) { a = 0; min=true; } else { min=false; }
    float b = value * a;
    return b;
}

/**
* Provides acceleration-based rampup and rampdown
*/
float ramp_accel(float power, float& lastpower, float time, const float maxAccel)
{

}

/**
* Provides PID-based rampup and rampdown
* @param power Target power, the PID setpoint
* @param measuredpower Immediate measured motor power
* @param dt Change in time since last measured reading
* @param error Last received error value... just keep passing the last value in
* @param integral Last integral value... just keep passing the last value in
*/
float ramp_pid(float power, float measuredpower, float dt, float& error, float& integral)
{
	float curerror = power - measuredpower;
	integral += curerror*dt;
	float derivative = (curerror - error)/dt;
	float output = curerror + integral + derivative;
	error = curerror;
	return output;
}

/**
* Provide tank steering using the stored robot configuration.
* This function lets you directly provide joystick values from any source.
* @param leftValue The value of the left stick.
* @param rightValue The value of the right stick.
*/
void tank2arcade(float leftValue, float rightValue, float& out_x, float& out_y)
{
    out_x = coerce(-100, 100, (leftValue-rightValue)/2);
    out_y = coerce(-100, 100, (leftValue+rightValue)/2);
    return;
}

/**
* Provide tank steering using the stored robot configuration.
* This function lets you directly provide joystick values from any source.
* @param leftValue The value of the left stick.
* @param rightValue The value of the right stick.
*/
void arcade2tank(float x, float y, float& out_left, float& out_right)
{
    x = -x;
    float v = (100-abs(x)) * (y/100) + y;
    float w = (100-abs(y)) * (x/100) + x;
    out_right = (v+w)/2;
    out_left = (v-w)/2;
    return;
}

/**
* Converts rectangular to polar coordinates, in radians.
* To convert to degrees, use degreesToRadians()
* @param x X-value
* @param y Y-value
* @param out_r Magnitude, r
* @param out_theta Direction, theta in radians
*/
void rect2polar(float x, float y, float& out_r, float& out_theta)
{
    out_r = sqrt((x*x)+(y*y));
    out_theta = atan2(y,x);
}

/**
* Converts polar to rectangular coordinates, in radians.
* To convert to degrees, use degreesToRadians()
* @param r Magnitude, r
* @param theta Direction, theta in radians
* @param out_x X-value
* @param out_y Y-value
*/
void polar2rect(float r, float theta, float& out_x, float& out_y)
{
    out_x = r*cos(theta);
    out_y = r*sin(theta);
}

/**
* Gyro Absolution
* Force Gyro from -360 to 360 into 0 to 360
*/
float concGyro(float gyro){
   if (gyro < 0){
    	return 360 - abs(gyro);
 	 }
 	 return gyro;
}
