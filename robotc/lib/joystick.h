/**********************************************************

Joystick Operations
joystick.h

Activates the Joystick driver and includes joystick
IO and manipulation functions.

**********************************************************/

#include "JoystickDriver.c"  //Joystick and Bluetooth driver

/*****************************************************
deadband

Gives a joystick a "deadzone" where any value
less than this would return zero.  Useful when joysticks
may be calibrated slightly off to prevent unwanted
movements.

in  deadband : Maximum value that returns zero
in  value    : Value to test
out deadband : Deadbanded value
*****************************************************/
float deadband(float deadband, float value)
{
	if (abs(value) > deadband)
	{
		return value;
	}
	else
	{
		return 0;
	}
}
/*****************************************************
coerce

Forces a numerical value to be between a min
and a max.

in  min    : If less than min, returns min
in  max    : If greater than max, returns max
in  value  : Value to test
out coerce : Coerced value
*****************************************************/
float coerce(float min, float max, float value)
{
	if (value > max) { return max; }
	if (value < min) { return min; }
	return value;
}
