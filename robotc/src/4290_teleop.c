#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S3,     IR,             sensorI2CCustom)
#pragma config(Sensor, S4,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     BlowerA,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     BlowerB,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     BlowerC,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    TubeRelease,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    Storage,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    GoalRetainer,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    GoalLift,             tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/***** DEFINES *****/
//#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
//#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
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
    bCompetitionMode = true; //true to enable competition mode

    displaySplash("4290", "Scrimmage Teleop", true);
    eraseDisplay();
    gyro_init(HTGYRO);
    wait1Msec(50);
    nxtbarOn();
    return;
}

task main()
{
    float leftFront, leftBack, rightFront, rightBack; // motors
    float y, x, c;
    bool blowerenabled = false;

    /***** BEGIN Mecanum Field Oriented Drive Test *****/
    init();
    StartTask(gyro_calibrate, 8);
    StartTask(displaySmartDiags, 255);
    if (bCompetitionMode) {waitForStart();}

    while (true)
    {
        /***** Proportional Motor Control *****/
        getJoystickSettings(joystick); //get all joystick statuses
		if (deadband(k_deadband,joystick.joy1_y1) == 0 &&
			deadband(k_deadband,joystick.joy1_x1) == 0 &&
        	deadband(k_deadband,joystick.joy1_x2) == 0 ) {
            motor[Lf] = 0;
        	motor[Rf] = 0;
        	motor[Lb] = 0;
        	motor[Rb] = 0;
        }
        else {
		    //scale to -1 to 1
	        y = (deadband(k_deadband,joystick.joy1_y1)+1)/128; //strafe
	        x = (deadband(k_deadband,joystick.joy1_x1)+1)/128; //forward/rev
	        c = (deadband(k_deadband,joystick.joy1_x2)+1)/128; //spin

	        mecanum_arcadeFOD(y, x, c, gyro_getheading(),
	        leftFront, rightFront, leftBack, rightBack);

	        motor[Lf] = leftFront*100;
	        motor[Rf] = rightFront*100;
	        motor[Lb] = leftBack*100;
	        motor[Rb] = rightBack*100;
	      }

        if(joy1Btn(4) == 1) { gyro_reset(); }
        if(joy2Btn(4)== 1){
            if (blowerenabled{
                motor[BlowerA] = 0;
	            motor[BlowerB] = 0;
	            motor[BlowerC] = 0;
	            blowerenabled = false
            }
            else{
	            motor[BlowerA] = 100;
	            motor[BlowerB] = 100;
	            motor[BlowerC] = 100;
	            blowerenabled = true
            }
        }
        if (joy2Btn(1)==1){
            motor[Intake] = 100;
        }
        else{
            motor[Intake] = 0;
        }
        while(nNxtButtonPressed == kEnterButton) { gyro_reset(); }
    }
}