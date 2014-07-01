#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S3,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     PSPNXV4,        sensorI2CCustomFastSkipStates)
#pragma config(Motor,  mtr_S1_C1_1,     drive_right,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     drive_left,    tmotorTetrix, openLoop, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/***** DEFINES *****/
#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/drive.h" //drive trains
#include "../lib/gyro.h" //gyroscope and FOD
#include "../lib/i2c.h" //I2C error checking

#include "../drivers/mindsensors-ps2ctrl-v4.h" //mindsensors stuffs

/***** STATICS *****/
static bool competitionmode = false; //set to true to wavoid waiting for FCS
static float k_deadband = 15;

/***** VARIABLES *****/
bool ready = false;

void qconv(bool ok, string& isok) {
    if (ok) { isok = "OK!"; }
    else { isok = "ERROR!"; }
}

void init()
{
	// Place code here to initialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

    eraseDisplay();
    wait1Msec(1000);

    //I2C error checking
    const int itemcount = 4;
    bool ok[itemcount];
    string desc[itemcount];
    bool error = true;
    while(error)
    {
        //Test and description
        ok[0] = errorcheck(1,1,MOTORCON);
        desc[0] = "MC1";
        ok[1] = errorcheck(2,1,MOTORCON);
        desc[1] = "MC2";
        ok[2] = errorcheck(3,1,MOTORCON);
        desc[2] = "MC3";
        ok[3] = errorcheck(4,1,MOTORCON);
        desc[3] = "MC4";

        error = true;
        for (int i=0; i<itemcount; i++)
        {
            if(ok[i]==false){error = true;}
        }

        if (error)
        {
            string isok;
            for (int j=0; j<itemcount; j++) {
                qconv(ok[j], isok);
                nxtDisplayTextLine(j, "%s", desc[j]);
                int y = 63-(8*j);
                nxtDisplayStringAt(62, y, "%s", isok);
            }

            PlayTone(440, 100);
        }
    }
    ClearSounds();

}

task main()
{
    init();

    // This is the struct that holds all the info on all buttons and joypads/sticks
    tPSP controller;
    float left, right;

    StartTask(gyro_calibrate, 8);

    if (competitionmode) {waitForStart();}

    while (true)
    {
        // Read the state of the buttons
        PSPV4readButtons(PSPNXV4, controller);

        //y-axis is inverted
        drive_tank(deadband(k_deadband,-controller.joystickLeft_y), deadband(k_deadband,-controller.joystickRight_y), left, right);
        motor[drive_left] = left;
        motor[drive_right] = right;

        nxtDisplayCenteredBigTextLine(6, "%.2f", gyro_getheading());
        nxtDisplayCenteredTextLine(5, "%.2f", gyro_getrotspeed());
        nxtDisplayCenteredTextLine(4, "%.2f", gyro_getfirstoffset());

        while(nNxtButtonPressed == kEnterButton)
        {
            //Reset heading to zero
            gyro_reset();
        }

    }
}
