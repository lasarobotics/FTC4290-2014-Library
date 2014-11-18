#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
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
#pragma config(Servo,  srvo_S2_C1_2,    BallStorage,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    GoalRetainer,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    Kickstand,            tServoStandard)
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
#include "../lib/options.h" //splash screens and display options
#include "../drivers/hitechnic-irseeker-v2.h"
/***** STstorageclosedATICS *****/
static float k_deadband = 15;
/***** VARIABLES *****/
//TJoystick controller; //--declared in JoystickDriver.c, imported by drive.h--

void init()
{
    servo[GoalRetainer] = 255; //ANY SERVO LINE CAUSES THE PROBLEM
    servo[Kickstand] = 135;
    servo[BallStorage] = 100;
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = true; //true to enable competition mode
    displaySplash("High PHidelity", "Teleop", true);
    eraseDisplay();
    gyro_init(HTGYRO);
    wait1Msec(50);
    nxtbarOn();
    return;
}
/*Control Layout:
Controller 1:
Left Joystick x/y - Strafe and forward for robot
Right Joystick x - Turn
Button 3: Goal Hook
Button 4: Gyro Reset
Controller 2:
Button 1: Intake
Button 3: Kickstand
Button 4: Blower
Button 5: Lift release
Button 6: Intake backwards
Button 8: Ball storage
*/
task main()
{
    float leftFront, leftBack, rightFront, rightBack; // motors
    float y, x, c;
    bool blowerenabled = false;
    bool kickstandenabled = false;
    bool goalreatainenabled = false;
    bool storageclosed = false;
    int joy1Btn3last = 0;
    int joy2Btn4last = 0;
    int joy2Btn3last = 0;
    int joy2Btn8last = 0;
    /***** BEGIN Mecanum Field Oriented Drive Test *****/
    init();
    StartTask(gyro_calibrate, 8);
    StartTask(displaySmartDiags, 255);
    if (bCompetitionMode) {waitForStart();}
    while (true)
    {
        /***** Proportional Motor Control *****/
        getJoystickSettings(joystick); //get all joystick statuses
        //Drive Code
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
        //Gyro Reset Code
        if(joy1Btn(4) == 1) { gyro_reset(); }
        if(nNxtButtonPressed == kEnterButton) { gyro_reset(); }

        //Goal Latch Toggle
        if(joy1Btn(3)== 1 && joy1Btn3last != 1){
            if (goalreatainenabled){
                servo[GoalRetainer] = 255;
                goalreatainenabled = false;
            }
            else{
                servo[GoalRetainer] = 0;
                goalreatainenabled = true;
            }
        }

        //Blower Toggle
        if(joy2Btn(4)== 1 && joy2Btn4last != 1){
            if (blowerenabled){
                motor[BlowerA] = 1;
                motor[BlowerB] = 1;
                motor[BlowerC] = 1;
                //Start timer for 1 sec, then set motors to 0
                ClearTimer(T1);
                blowerenabled = false;
            }
            else{
                motor[BlowerA] = 100;
                motor[BlowerB] = 100;
                motor[BlowerC] = 100;
                blowerenabled = true;
            }
        }
        //If 5 seconds since blower shutdown-brake motors
        if (time1[T1] > 5000 && !blowerenabled){
            motor[BlowerA] = 0;
            motor[BlowerB] = 0;
            motor[BlowerC] = 0;
        }
        //Kickstand Toggle
        if(joy2Btn(3)== 1 && joy2Btn3last != 1){
            if (kickstandenabled){
                servo[Kickstand] = 135;
                kickstandenabled = false;
            }
            else{
                servo[Kickstand] = 0;
                kickstandenabled = true;
            }
        }
        //Storage Toggle
        if(joy2Btn(8)== 1 && joy2Btn8last != 1){
            if (storageclosed){
                servo[BallStorage] = 180;
                storageclosed = false;
            }
            else{
                servo[BallStorage] = 100;
                storageclosed = true;
            }
        }
        //Intake Forwards, back, slow forwards
        if (joy2Btn(1) == 1){
            motor[Intake] = 100;
        }
        else if (joy2Btn(5) == 1){
            motor[Intake] = 25;
        }
        else if (joy2Btn(6) == 1){
            motor[Intake] = -100;
        }
        else{
            motor[Intake] = 0;
        }
        joy1Btn3last = joy1Btn(3);
        joy2Btn4last = joy2Btn(4);
        joy2Btn3last = joy2Btn(3);
        joy2Btn8last = joy2Btn(8);
        //DO NOT REMOVE THIS WAIT, See issue #11
        nxtDisplayTextLine(3, "%i", gyro_getheading());
        wait1Msec(5);
    }
}
