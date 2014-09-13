/**********************************************************

Controller Conversions
controller.h

Standardize controller joystick inputs.

**********************************************************/

/**
* Convert from FTC Controller value (-128 to 127)
* @param Joystick input value
*/
void ftcController(float& input)
{
    if (input < 0)
        return input/128;
    return input/127;
}
/**
* Convert from PSP-nx Controller value (0 to 255)
* @param Joystick input value
*/
void pspnxController(float& input)
{   input = input - 128;
    if (input < 0)
        return input/128;
    return input/127;
}
