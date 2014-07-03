/**********************************************************

Drive Train Operations
drive.h

Multiple drivetrain implementations for
your coding pleasure.

**********************************************************/

#include "drivemath.h"

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
