/**********************************************************

Drive Train Operations
drive.h

Multiple drivetrain implementations for
your coding pleasure.

**********************************************************/

#include "joystick.h"
#include "motor.h"

/**
* Provide tank steering using the stored robot configuration.
* This function lets you directly provide joystick values from any source.
* @param leftValue The value of the left stick.
* @param rightValue The value of the right stick.
*/
void drive_tank(float leftValue, float rightValue, float& out_left, float& out_right)
{
	// square the inputs (while preserving the sign) to increase fine control while permitting full power
	out_left = coerce(-100, 100, leftValue);
	out_right = coerce(-100, 100, rightValue);
	return;
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
* Provide exponential rampup and rampdown
* @param value The value of the left stick.
* @param time Time since zero or value == outvalue
*/
float exp_drive(float value, float time, bool& max)
{
	float a = .01 * (pow(1.031177275, time));
	if (a >= 1) { a = 1; max=true; } else { max=false; }
	float b = value * a;
	return b;
}
