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
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = true; //true to enable competition mode

    displaySplash("Error Checking", "Multi Test", true);

    StartTask(displaySmartDiags, 255);

    /*Setup PrototypeBoard for output on B0 of NXT Port 4
    (0x01 represents the pin B0 and Port 4 is defined above in pragma config)*/
    HTSPBsetupIO(HTSPB, 0x1);
    while (true){
    	eraseDisplay();
        bool noerror = errorcheck(1,0,1,MOTORCON) &&
        							 errorcheck(1,0,2,MOTORCON);
        if (noerror){
            //LED, we are good
            nxtDisplayCenteredBigTextLine(3, "A OK!");
            ClearSounds();
        }
        else {
            //No LED, because we have error
            nxtDisplayCenteredBigTextLine(3, "ERROR!");
            PlayTone(440, 100);
        }
        wait1Msec(1000);
    }
}
