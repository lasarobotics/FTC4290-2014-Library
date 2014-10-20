#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     HTIRS2,             sensorI2CCustom)
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
#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/drive.h" //drive trains
#include "../lib/gyro.h" //gyroscope and FOD
#include "../lib/i2c.h" //I2C error checking
#include "../lib/display.h" //splash screens
#include "../drivers/hitechnic-irseeker-v2.h"
/***** STATICS *****/
static float k_deadband = 15;

/***** VARIABLES *****/
//TJoystick controller; //--declared in JoystickDriver.c, imported by drive.h--

void init()
{
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = false; //true to enable competition mode

    displaySplash("NoStep", "IR Seek Test", true);

    bool ok = true;
    while(!ok)
    {
        ok = errorcheck(1,0,1,MOTORCON) &&
        		 errorcheck(1,0,2,MOTORCON);
		    if (!ok) {
		        PlayTone(440, 50);
		        nxtDisplayCenteredBigTextLine(3, "ERROR!");
		    }
		    else { ClearSounds(); nxtDisplayCenteredBigTextLine(3, "A OK!"); }
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

    forward_Mecanum(1700, 100, 0, Lf, Lb, Rf, Rb);
    //forward_Mecanum(1200, 0, -100, Lf, Lb, Rf, Rb);

    float leftFront, leftBack, rightFront, rightBack;

    static float ir_tolerance = 15;

    //move until ir
    mecanum_arcade(0, -1, 0, leftFront, leftBack, rightFront, rightBack);
    motor[Lf] = leftFront*100;
    motor[Rf] = rightFront*100;
    motor[Lb] = leftBack*100;
   	motor[Rb] = rightBack*100;
		int irS1, irS2,irS3,irS4, irS5 = 0;
    while ((abs(irS3 - irS4) > ir_tolerance) || (irS3 < 70) || (irS4 < 70)) {
   		HTIRS2readAllACStrength(HTIRS2, irS1, irS2, irS3, irS4, irS5);
    	nxtDisplayCenteredTextLine(6, "IR3: %i", irS3);
    	nxtDisplayCenteredTextLine(7, "IR4: %i", irS4);
   	} //goal: the difference between sensors < a tolerance
    //while (!((irS3 >= 100) && (irS4 >= 100))) { } //goal: 3&4 > 100
    motor[Lf] = 0;
    motor[Rf] = 0;
    motor[Lb] = 0;
   	motor[Rb] = 0;
    wait1Msec(20);
   	forward_Mecanum(500, 100, 0, Lf, Lb, Rf, Rb);

    //Movement Sequence
    //forward_Mecanum(3500, 100, Lf, Lb, Rf, Rb);
   	//turnToDeg_Mecanum(180,100, Lf, Lb, Rf, Rb);
    //forward_Mecanum(3500, 100, Lf, Lb, Rf, Rb);
    //turnToDeg_Mecanum(270, 100, Lf, Lb, Rf, Rb);
    //forward_Mecanum(2000, 100, Lf, Lb, Rf, Rb);
    //turnToDeg_Mecanum(180, 100, Lf, Lb, Rf, Rb);

    //kill everything
		StopAllTasks();

}
