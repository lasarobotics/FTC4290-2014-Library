/**********************************************************

Motor and Servo Operations
motor.h

Controls motors, servos, and encoders.

**********************************************************/


////////////////////////
// ENCODER OPERATIONS //
////////////////////////

/*****************************************************
Filter Encoder Values

Ignores encoder errors

in  value         : Current encoder value
in  lastvalue     : Last encoder value, if none set to zero
out filterencoder : Fixed value
*****************************************************/
float filterencoder(float value, float lastvalue)
{
	if (value == 0) { return lastvalue; }
	else { return value; }
}

//////////////////////////////
// NORMALIZATION OPERATIONS //
//////////////////////////////

/*****************************************************
Normalize Motor from FTC Controller

Converts FTC Controller output (-128 to 128) to
TETRIX or LEGO motor input (-100 to 100)

in  value : Value to normalize
out norm  : Normalized Value
*****************************************************/
float norm_ftc_motor(float value)
{
	return value / 127.5 * 100;
}
/*****************************************************
Normalize Servo from FTC Controller

Converts FTC Controller output (-128 to 128) to
FTC servo input (0 to 255)

in  value : Value to normalize
out norm  : Normalized Value
*****************************************************/
float norm_ftc_servo(float value)
{
	return ((value / 127) + 1) / 2 * 255;
}
