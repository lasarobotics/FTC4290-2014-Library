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
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     BlowerA,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     BlowerB,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     BlowerC,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_2,    BallStorage,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    GoalRetainer,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    Kickstand,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
/**
* Autonomous Program Headers for 4290 GiraPHPHe
* Contains methods for 4290 autonomous
**/

const int BallStorage_Closed = 140;
const int BallStorage_OpenSmall = 95;
const int BallStorage_Open = 85;
const int GoalRetainer_Open = 0;
const int GoalRetainer_Closed = 255;

tMUXSensor touchSensorOne = msensor_S4_2;
tMUXSensor touchSensorTwo = msensor_S4_3;
void auto_init()
{
  if (log_enabled)
  {
    log_init("auto.txt", false, logid);
    log_write("LOG  : Started", logid);
  }
}

float getZone(float avgS2,float avgS3,bool newIR){
  float zone = 1;
  if ((avgS2 > 20) && (avgS3 > 20)) {
    zone = 2;
  }
  else if ((avgS2 < 10) && (avgS3 > 20)) {
    zone = 3;
  }
  nxtDisplayCenteredTextLine(3, "%i", zone);
  return zone;
}

void move_encoderortouch(float encodercount, float forward, float strafe, tMotor Lf, tMotor Lb, tMotor Rf, tMotor Rb){
  nMotorEncoder[Rb] = 0;
  float leftFront, leftBack, rightFront, rightBack;
  forward = speedController(forward);
  strafe = speedController(strafe);
  mecanum_arcade(forward, strafe, 0,
  leftFront, rightFront, leftBack, rightBack);

  //Move
  motor[Lf] = leftFront*100;
  motor[Rf] = rightFront*100;
  motor[Lb] = leftBack*100;
  motor[Rb] = rightBack*100;

  //Pause
  while ((abs(nMotorEncoder[Rb])) <= encodercount /*&& !TSreadState(touchSensorOne)*/){
    nxtDisplayCenteredTextLine(7, "%.2f", nMotorEncoder[Rb]);
  }

  //Stop
  nMotorEncoder[Rb] = 0;
  motor[Lf] = 0;
  motor[Rf] = 0;
  motor[Lb] = 0;
  motor[Rb] = 0;
}
/**
* Move down ramp
* Move the robot down the ramp
*/
void auto_moveDownRamp(){
  forward_encoderMecanum(4000, 0, 50, Lf, Lb, Rf, Rb);
  forward_encoderMecanum(4000, 0, 40, Lf, Lb, Rf, Rb);
}

/**
* Move from bottom of ramp to parking zone
*/
void auto_rampToParking(){
  forward_encoderMecanum(3000, 0, -100, Lf, Lb, Rf, Rb);
  forward_encoderMecanum(4250, -100, 0, Lf, Lb, Rf, Rb);
  forward_encoderMecanum(2500, 0, -100, Lf, Lb, Rf, Rb);
}
/**
* Center IR Right
* Move until the robot's gyro sensor is aligned to the goal.
*/
void centerIRRight(int zone){
  float leftFront, leftBack, rightFront, rightBack;
  //move until IR
  mecanum_arcade(0, -.5, 0, leftFront, leftBack, rightFront, rightBack);
  motor[Lf] = leftFront*100;
  motor[Rf] = rightFront*100;
  motor[Lb] = leftBack*100;
  motor[Rb] = rightBack*100;
  while (ir_getavg(2) < 50 )
  {
    nxtDisplayCenteredTextLine(4, "IR2: %i", ir_getraw(2));
    nxtDisplayCenteredTextLine(5, "IR3: %i", ir_getraw(3));
    nxtDisplayCenteredTextLine(6, "Avg IR2: %i", ir_getavg(2));
    nxtDisplayCenteredTextLine(7, "Avg IR3: %i", ir_getavg(3));
  }
  motor[Lf] = 0;
  motor[Rf] = 0;
  motor[Lb] = 0;
  motor[Rb] = 0;
  forward_encoderMecanum(600,0,-25,Lf,Lb,Rf,Rb);
  wait1Msec(1000);
  //Place ball sequence
  forward_encoderMecanum(800, -25, 0, Lf, Lb, Rf, Rb);
  forward_encoderMecanum(400, -15, 0, Lf, Lb, Rf, Rb);
  forward_encoderMecanum(500, -5, 0, Lf, Lb, Rf, Rb);
  wait1Msec(1000);
}
/**
* Center IR
* Move until the robot's gyro sensor is aligned to the goal.
*/
void centerIRLeft(int zone){
  float leftFront, leftBack, rightFront, rightBack;
  //move until IR
  mecanum_arcade(0, 1, 0, leftFront, leftBack, rightFront, rightBack);
  motor[Lf] = leftFront*100;
  motor[Rf] = rightFront*100;
  motor[Lb] = leftBack*100;
  motor[Rb] = rightBack*100;
  while (ir_getavg(3) < 50 )
  {
    nxtDisplayCenteredTextLine(4, "IR2: %i", ir_getraw(2));
    nxtDisplayCenteredTextLine(5, "IR3: %i", ir_getraw(3));
    nxtDisplayCenteredTextLine(6, "Avg IR2: %i", ir_getavg(2));
    nxtDisplayCenteredTextLine(7, "Avg IR3: %i", ir_getavg(3));
  }
  //2.5in delay 250 for low bat
  forward_encoderMecanum(150, 0, -100, Lf, Lb, Rf, Rb);
  motor[Lf] = 0;
  motor[Rf] = 0;
  motor[Lb] = 0;
  motor[Rb] = 0;
  //Place ball sequence
  //~1200 for low bat
  forward_encoderMecanum(950, 25, 0, Lf, Lb, Rf, Rb);
}
//TODO enum irAction

/***** PLACE IN CENTER GOAL *****/
// returns current zone (1,2,3)
float auto_placeCenterGoal(bool newIR)
{
  servo[GoalRetainer] = GoalRetainer_Open;
  wait1Msec(1000);
  motor[Intake] = 30;
  wait1Msec(2000);
  motor[Intake] = 1;
  forward_encoderMecanum(2100, -15, 0, Lf, Lb, Rf, Rb);
  //Wait for a little bit
  wait1Msec(1000);
  gyro_reset();
  //wait10Msec(30);
  //forward_encoderMecanum(400, 0, 100, Lf, Lb, Rf, Rb);
  //ir_wait(50); //wait 50 samples

  float avgS2 = ir_getavg(2);
  float avgS3 = ir_getavg(3);
  float zone = getZone(avgS2,avgS3,newIR);
  if (zone == 3){
    forward_encoderMecanum(1100, 0, -90, Lf, Lb, Rf, Rb);
    wait1Msec(1000);
    forward_encoderMecanum(400, -35, 0, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(267, -25, 0, Lf, Lb, Rf, Rb);
    forward_Mecanum(1000,-15,0,Lf,Lb,Rf,Rb);
    servo[BallStorage] = BallStorage_Open;
  }
  if (zone == 2){
    //Nav to zone 2
    forward_encoderMecanum(367,0,-50, Lf, Lb, Rf, Rb);
    wait1Msec(1000);
    //Place ball sequence
    forward_encoderMecanum(700, -25, 0, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(733, -15, 0, Lf, Lb, Rf, Rb);
    //turnToDeg_Mecanum(10,25,Lf,Lb,Rf,Rb);
    wait1Msec(1000);
    forward_Mecanum(1000,-15,0,Lf,Lb,Rf,Rb);
    servo[BallStorage] = BallStorage_Open;
  }
  if (zone == 1){
    //Nav to zone 1 (farthest)
    forward_encoderMecanum(2500, 0, 100, Lf, Lb, Rf, Rb);
    wait1Msec(250);
    forward_encoderMecanum(2600, -25, 0, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(1000, -15, 0, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(75, 50, Lf, Lb, Rf, Rb);
    wait1Msec(100);
    forward_encoderMecanum(700, -15, 0, Lf, Lb, Rf, Rb);
    forward_Mecanum(1000,-25,0,Lf,Lb,Rf,Rb);
    servo[BallStorage] = BallStorage_Open;
  }
  motor[Intake] = 0;
  wait1Msec(250);
  return zone;
}
void auto_centerGoalToLarge(float zone){
  if (zone == 3){
    turnToDeg_Mecanum(0,25,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(667,15,0, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(2667, 0, -100, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(5250, -50, 0, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(42,25,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(3500, -25, 0, Lf, Lb, Rf, Rb);
    forward_Mecanum(1000,-50,0,Lf,Lb,Rf,Rb);
    servo[GoalRetainer] = GoalRetainer_Closed;
    forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
    wait1Msec(2000);
    servo[BallStorage] = BallStorage_Open;
    wait1Msec(1000);
  }
  if (zone == 2){
    forward_encoderMecanum(500, 15, 0, Lf, Lb, Rf, Rb);
    //turnToDeg_Mecanum(0,25,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(2133, 0, 100, Lf, Lb, Rf, Rb);
    forward_encoderMecanum(5273, -50, 0, Lf, Lb, Rf, Rb);
    wait1Msec(250);
    turnToDeg_Mecanum(75,25,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(6335, -50, 0, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(42,25,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(750, -25, 0, Lf, Lb, Rf, Rb);
    forward_Mecanum(1000,-25,0,Lf,Lb,Rf,Rb);
    servo[GoalRetainer] = GoalRetainer_Closed;
    wait1Msec(500);
    forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
    wait1Msec(500);
    servo[BallStorage] = BallStorage_Open;
    wait1Msec(500);
  }
  if (zone == 1){
    forward_encoderMecanum(333, 15, 0, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(0,35,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(3000, -50, 0, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(75,35,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(6667, -50, 0, Lf, Lb, Rf, Rb);
    turnToDeg_Mecanum(42,35,Lf,Lb,Rf,Rb);
    forward_encoderMecanum(600, -25, 0, Lf, Lb, Rf, Rb);
    forward_Mecanum(1000,-25,0,Lf,Lb,Rf,Rb);
    servo[GoalRetainer] = GoalRetainer_Closed;
    forward_encoderMecanum(200, 25, 0, Lf, Lb, Rf, Rb);
    wait1Msec(1000);
    servo[BallStorage] = BallStorage_Open;
    wait1Msec(500);
  }

}
/**** PLACE IN KICKSTAND
(from already placing a ball in center goal) ****/
void auto_kickstandFromCenterGoal(int zone)
{
  //hook the goal
  wait1Msec(250);
  forward_encoderMecanum(1300, 0, 100, Lf, Lb, Rf, Rb);
  wait1Msec(250);
  forward_encoderMecanum(600, 100, 0, Lf, Lb, Rf, Rb);
  wait1Msec(250);
  forward_encoderMecanum(1000, 0, -100, Lf, Lb, Rf, Rb); //hit the goal
  wait1Msec(250);
  forward_encoderMecanum(2200, 100, 0, Lf, Lb, Rf, Rb); //continue moving
}
