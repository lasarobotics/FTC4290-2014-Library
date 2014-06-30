/**********************************************************

Motor and Servo Operations
motor.h

Controls motors, servos, and encoders.

**********************************************************/

////////////////////////
// ENCODER OPERATIONS //
////////////////////////

/**
* Ignores encoder errors.
*
* @param Current encoder value
* @param Last encoder value, if none set to zero
* @return Fixed value
*/
float filterencoder(float value, float lastvalue)
{
    if (value == 0) { return lastvalue; }
    else { return value; }
}

//////////////////////////////
// NORMALIZATION OPERATIONS //
//////////////////////////////

/**
* Converts FTC Controller output (-128 to 128) to TETRIX or LEGO motor input (-100 to 100)
*
* @param Value to normalize
* @return Normalized Value
*/
float norm_ftc_motor(float value)
{
    return value / 127 * 100;
}

/**
* Converts FTC Controller output (-128 to 128) to FTC servo input (0 to 255)
*
* @param Value to normalize
* @return Normalized Value
*/
float norm_ftc_servo(float value)
{
    return ((value / 127) + 1) / 2 * 255;
}
