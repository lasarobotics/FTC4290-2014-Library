/**********************************************************

Joystick Operations
joystick.h

Activates the Joystick driver and includes joystick
IO and manipulation functions.

**********************************************************/

#include "JoystickDriver.c"  //Joystick and Bluetooth driver

/**
* Gives a joystick a "deadzone" where any value
* less than this would return zero.  Useful when joysticks
* may be calibrated slightly off to prevent unwanted
* movements.
*
* @param Maximum value that returns zero
* @param Value to test
* @return Deadbanded value
*/
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
/**
* Forces a numerical value to be between a min
* and a max.
*
* @param If less than min, returns min
* @param If greater than max, returns max
* @param Value to test
* @return Coerced value
*/
float coerce(float min, float max, float value)
{
    if (value > max) { return max; }
    if (value < min) { return min; }
    return value;
}
