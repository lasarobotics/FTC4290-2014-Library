/**********************************************************
Drive Train Operations
drive.h
Multiple drivetrain implementations for
your coding pleasure.
**********************************************************/

#include "drivemath.h"
#include "controller.h"
#include "sensor.h"
/**
* Provide tank steering using the stored robot configuration.
* This function lets you directly provide joystick values from any source.
* @param leftValue The value of the left stick.
* @param rightValue The value of the right stick.
*/
void drive_tank(float leftValue, float rightValue, float& out_left, float& out_right)
{
    // possible to square the inputs (while preserving the sign) to increase fine control while permitting full power
    out_left = coerce(-100, 100, leftValue);
    out_right = coerce(-100, 100, rightValue);
    return;
}

/**
* Mecanum 2-joystick arcade drive.
* Recommended: forward=y1, strafe=x1, spin=x2
* @param forward Forward/reverse (x) direction power (-1 to 1)
* @param strafe strafe (y) direction power (-1 to 1)
* @param spin Clockwise spin (c) rotation power (-1 to 1)
*/
void mecanum_arcade(float forward, float strafe, float spin,
float& leftfront, float& rightfront, float& leftback, float& rightback)
{
    leftfront = forward + strafe + spin;
    rightfront = forward - strafe - spin;
    leftback = forward - strafe + spin;
    rightback = forward + strafe - spin;
    norm_mecanum(leftfront,rightfront,leftback,rightback);
}

/**
* Mecanum 2-joystick arcade drive with field orientation.
* Recommended: forward=y1, strafe=x1, spin=x2
* @param forward Forward/reverse (x) direction power (-1 to 1)
* @param strafe strafe (y) direction power (-1 to 1)
* @param spin Clockwise spin (c) rotation power (-1 to 1)
* @param gyroheading Gyroscope heading in degrees, recommended -360 to 360
*/
void mecanum_arcadeFOD(float forward, float strafe, float spin, float gyroheading,
float& leftfront, float& rightfront, float& leftback, float& rightback)
{
    float x, y;
    y = forward;
    x = strafe;
    float cosA = cos(concGyro(gyroheading) * (3.14159 / 180.0));
    float sinA = sin(concGyro(gyroheading) * (3.14159 / 180.0));
    float xOut = x * cosA - y * sinA;
    float yOut = x * sinA + y * cosA;

    mecanum_arcade(yOut, xOut, spin,
    leftfront, rightfront, leftback, rightback);
}
/**
* Autonomous turn to a specific degree based on gyro.
* @param Degree to turn to.
* @param Speed to turn at.
*/
void turnToDeg_Mecanum(float deg, float speed, tMotor Lf, tMotor Lb, tMotor Rf, tMotor Rb){
    float leftFront, leftBack, rightFront, rightBack;
    //Get position from 0-360
    float absposition = concGyro(gyro_getheading());
    float clockwise = 0;
    float counterclockwise = 0;
    float goclockwise = false;
    //Logic for if target deg is greater or less than current deg
    if (deg < absposition){
        clockwise = (360+deg) - absposition;
        counterclockwise = absposition - deg;
    }
    else {
        clockwise = deg - absposition;
        counterclockwise = absposition + (360 - deg);
    }
    //If clockwise direction to destination is closer than counterclockwise, go clockwise
    if (clockwise < counterclockwise )
    {
        goclockwise = true;
    }
    nxtDisplayCenteredTextLine(3, "%.2f", clockwise);
    nxtDisplayCenteredTextLine(4, "%.2f", counterclockwise);
    if (goclockwise) { nxtDisplayCenteredTextLine(5, "clockwise"); }
    if (!goclockwise) { nxtDisplayCenteredTextLine(5, "NOT clockwise"); }
    nxtDisplayCenteredTextLine(6, "%.2f", concGyro(gyro_getheading()));
    //While we are greater than within mechanum_tolerance, drive
    /** MOVE MOTORS HERE! **/
    if (!goclockwise) { speed = -speed; }
    float localspeed = speedController(speed);
    mecanum_arcadeFOD(0, 0, localspeed, 0,
    leftFront, rightFront, leftBack, rightBack);
    motor[Lf] = leftFront*100;
    motor[Rf] = rightFront*100;
    motor[Lb] = leftBack*100;
    motor[Rb] = rightBack*100;
    float offset = gyro_getrawheading();
    if (goclockwise){
        while (abs(gyro_getrawheading() - offset)  < clockwise ){
            nxtDisplayCenteredTextLine(7, "%.2f", gyro_getrawheading());
            wait1Msec(5);
        }
    }
    else{
        while (abs(gyro_getrawheading()- offset)  < counterclockwise ){
            nxtDisplayCenteredTextLine(7, "%.2f", gyro_getrawheading());
            wait1Msec(5);
        }
    }
    motor[Lf] = 0;
    motor[Rf] = 0;
    motor[Lb] = 0;
    motor[Rb] = 0;
}
/**
* Autonomous go forward for a certain time.
* @param Milliseconds to go forward for.
* @param Forward speed to go foward at.
* @param Directional (strafe) speed to go foward at.
*/
void forward_Mecanum(float millis, float forward, float strafe, tMotor Lf, tMotor Lb, tMotor Rf, tMotor Rb){
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
    wait1Msec(millis);

    //Stop
    motor[Lf] = 0;
    motor[Rf] = 0;
    motor[Lb] = 0;
    motor[Rb] = 0;
}
void forward_encoderMecanum(float encodercount, float forward, float strafe, tMotor Lf, tMotor Lb, tMotor Rf, tMotor Rb){
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
    while (abs(nMotorEncoder[Rb]) <= encodercount){
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
* Autonomous go forward for a certain time.
* @param Milliseconds to go forward for.
* @param Forward speed to go foward at.
*/
void forward_Mecanum(float millis, float forward, tMotor Lf, tMotor Lb, tMotor Rf, tMotor Rb){
    forward_Mecanum(millis, forward, 0, Lf, Lb, Rf, Rb);
}
