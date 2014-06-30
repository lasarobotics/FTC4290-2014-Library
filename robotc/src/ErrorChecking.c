#pragma config(Sensor, S4,     HTSPB,                sensorI2CCustom9V)
/***** DEFINES *****/
//#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../drivers/hitechnic-superpro.h" //SuperPro Prototype Board Driver
#include "../lib/i2c.h"
task main()
{
	 //Setup PrototypeBoard for output on B0 of NXT Port 4 (0x01 represents the pin B0 and Port 4 is defined above in pragma config)
   HTSPBsetupIO(HTSPB, 0x1);
	while (true){
		//port of specifc device
		int port = 1;
		//Place in daisy chain. 1st in the chain  is 2, 2nd is 4, 3rd is 6 ect...
		int address = 2;
		//Specific to sensor, located in sensor manuals
		string expected = "ServoCon";
		bool noerror = errorcheck(port,address,expected);
	if (noerror){
		 //LED, we are good
		HTSPBwriteIO(HTSPB, 0x01);
	}
	else {
   //No LED, because we have error
 		HTSPBwriteIO(HTSPB, 0x00);
	}
	 wait1Msec(1000);
	}
}
