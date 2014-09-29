#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/***** DEFINES *****/
#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
//#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/drive.h" //drive trains
#include "../lib/gyro.h" //gyroscope and FOD
#include "../lib/i2c.h" //I2C error checking
#include "../lib/display.h" //splash screens

/***** STATICS *****/
static float k_deadband = 15;

/***** VARIABLES *****/
//TJoystick controller; //--declared in JoystickDriver.c, imported by drive.h--

void init()
{
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = true; //true to enable competition mode

    displaySplash("NoStep", "Auto IR Test", true);

    bool ok = false;
    while(!ok)
    {
        const int testcount = 2;
	    bool test[testcount] = {
	        errorcheck(1,0,1,MOTORCON),
	        errorcheck(1,0,2,MOTORCON)};
	    string desc[testcount] = {"MC1-1","MC1-2"};
	    ok = error_display(test,desc,testcount);
	    if (!ok) {
	        PlayTone(440, 50);
	        if (test[0] == false && test[1] == false){
	            nxtDisplayCenteredTextLine(7, "Reboot MC!");
	        }
	    }
	    else { ClearSounds(); }
    }

    eraseDisplay();
    gyro_init(HTGYRO);
    wait1Msec(50);
    nxtbarOn();
    return;
}

task main()
{

    /***** BEGIN Mecanum Field Oriented Drive Test *****/
    init();
    StartTask(gyro_calibrate, 8);
    StartTask(displaySmartDiags, 255);

    //Movement Sequence
    forward_Mecanum(1700, 100, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(270,100, Lf, Lb, Rf, Rb);
    forward_Mecanum(3500, 100, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(270, 100, Lf, Lb, Rf, Rb);
    forward_Mecanum(2000, 100, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(180, 100, Lf, Lb, Rf, Rb);

}