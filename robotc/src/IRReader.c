#pragma config(Sensor, S4,     IR,             sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

#include "../lib/naturalization.h"
#include "../drivers/hitechnic-irseeker-v2.h"

task main()
{
	while (true){
		int irdir = HTIRS2readDCDir(IR);
		nxtDisplayCenteredBigTextLine(3, "%i", irdir);
	}
}
