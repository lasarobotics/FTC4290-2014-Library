/**********************************************************

Drive Train Operations
drive.h

Multiple drivetrain implementations for
your coding pleasure.

**********************************************************/

#include "drivemath.h"
#include "gyro.h"
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
    leftfront = coerce(-100,100,norm_ftc_motor(forward + strafe + spin));
    rightfront = coerce(-100,100,norm_ftc_motor(forward - strafe - spin));
    leftback = coerce(-100,100,norm_ftc_motor(forward - strafe + spin));
    rightback = coerce(-100,100,norm_ftc_motor(forward + strafe - spin));
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
    float x, y, r, theta, theta2;

    rect2polar(strafe,forward,r,theta);
    theta = radiansToDegrees(theta);
    theta2 = degreesToRadians(theta + gyroheading);
    polar2rect(r,theta2,x,y);

    mecanum_arcade(y, x, spin,
    leftfront, rightfront, leftback, rightback);
}

/**
* Autonomous turn to a specific degree based on gyro.
* @param Degree to turn to.
* @param Speed to turn at.
*/
void turnToDeg_Mecanum(float deg,float speed){
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
    nxtDisplayCenteredTextLine(5, "%.2f", deg);
    nxtDisplayCenteredTextLine(6, "%.2f", concGyro(gyro_getheading());
    //While we are greater than within 5 degs, drive
    while (abs(concGyro(gyro_getheading())- deg) > 2 ){
        if (goclockwise ){
            mecanum_arcadeFOD(0, 0, speed, gyro_getheading(),
            leftFront, rightFront, leftBack, rightBack);
        }
        else {
            mecanum_arcadeFOD(0, 0, -speed, gyro_getheading(),
            leftFront, rightFront, leftBack, rightBack);
        }
        nxtDisplayCenteredTextLine(7, "%.2f", concGyro(gyro_getheading());
        motor[Lf] = leftFront;
        motor[Rf] = rightFront;
        motor[Lb] = leftBack;
        motor[Rb] = rightBack;
    }
}
/**
* Autonomous go forward for a certain time.
* @param Milliseconds to go forward for.
* @param Speed to go foward at.
*/
void forward_Mecanum(float millis, float speed){
        float leftFront, leftBack, rightFront, rightBack;
        mecanum_arcade(speed, 0, 0,
        leftFront, rightFront, leftBack, rightBack);
        motor[Lf] = leftFront;
        motor[Rf] = rightFront;
        motor[Lb] = leftBack;
        motor[Rb] = rightBack;
        wait1Msec(millis);
        //Stop
        motor[Lf] = 0;
        motor[Rf] = 0;
        motor[Lb] = 0;
        motor[Rb] = 0;
}
