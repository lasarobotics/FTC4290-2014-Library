#pragma once //only include this file once

// Flag to control whether "test mode" (i.e. autonomous only) or "competition" mode
// 1. For testing user control mode only leave the code as follows.
// 2. For real competition, remove the "#define bUserControlTestModeOnly" or comment it out.
// This will allow you to use the mini-joystick application builtin the ROBOTC application for testing
// user control.
#ifdef _DEBUG_
#define bUserControlTestModeOnly
#endif

#include "JoystickDriver.c"  //Bluetooth and Joystick
#include "../drivers/hitechnic-irseeker-v2.h" //IR Seeker


///////////////
// FUNCTIONS //
///////////////

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

float norm_motor(float value)
{
	return value / 127.5 * 100;
}

float coerce(float min, float max, float value)
{
	if (value > max) { return max; }
	if (value < min) { return min; }
	return value;
}
