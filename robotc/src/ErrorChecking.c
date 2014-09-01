#pragma config(Sensor, S4,     HTSPB,                sensorI2CCustom9V)
/***** DEFINES *****/
#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../drivers/hitechnic-superpro.h" //SuperPro Prototype Board Driver
#include "../lib/i2c.h" //I2C errorchecking
#include "../lib/display.h" //splash screens

task main()
{
    bSmartDiagnostics = false; //true to enable smart diagnostic screen
    bCompetitionMode = false; //true to enable competition mode

    displaySplash("Error Checking", "LED Test", true);

    /*Setup PrototypeBoard for output on B0 of NXT Port 4
    (0x01 represents the pin B0 and Port 4 is defined above in pragma config)*/
    HTSPBsetupIO(HTSPB, 0x1);
    while (true){
    	eraseDisplay();
        bool noerror = errorcheck(1,0,1,MOTORCON);
        if (noerror){
            //LED, we are good
            HTSPBwriteIO(HTSPB, 0x01);
            nxtDisplayCenteredBigTextLine(3, "A OK!");
            ClearSounds();
        }
        else {
            //No LED, because we have error
            HTSPBwriteIO(HTSPB, 0x00);
            nxtDisplayCenteredBigTextLine(3, "ERROR!");
            PlayTone(440, 100);
        }
        wait1Msec(1000);
    }
}
