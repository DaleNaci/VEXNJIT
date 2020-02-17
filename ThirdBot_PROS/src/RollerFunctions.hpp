#include "main.h"
#include "Variables.hpp"
#ifndef ROLLERFUNCTIONS_HPP
#define ROLLERFUNCTIONS_HPP

/*
 Moves both tray rollers. Speed will depend on the speed parameter and positive
 speed specifies out(Spit) while negative speed specifies in(Grab). The range
 is -100 to 100.
*/
void rollersTray(int speed);

/*
 Moves both arm rollers. Speed will depend on the speed parameter and positive
 speed specifies out(Spit) while negative speed specifies in(Grab). The range
 is -100 to 100.
*/
void rollersArms(int speed);

/*
 Moves the tray rollers a specified number of degrees. Degrees is based on
 the degrees parameter, positive degrees will turn the rollers out(spit) the
 specified numeber of degrees, negative degrees will turn the rollers in(Grab)
 the specified numeber of degrees; the range of degrees is all int numbers;
 Speed will depend on the speed parameter. The range is -100 to 100.
*/
void rollersTrayDegrees(int degrees, int speed);
/*
 Moves the arm rollers a specified number of degrees. Degrees is based on
 the degrees parameter, positive degrees will turn the rollers out(spit) the
 specified numeber of degrees, negative degrees will turn the rollers in(Grab)
 the specified numeber of degrees; the range of degrees is all int numbers;
 Speed will depend on the speed parameter. The range is -100 to 100.
*/
void rollersArmsDegrees(int degrees, int speed);

/*
 Function to move both sets of rollers a specified number of degrees(relative),
 speed is absolute and positive degrees specifies out(spit) while negative degrees
 specifies in(grab). Speed range is +100 to -100, degrees range is any int value
*/
void rollersDegrees(int degrees, int speed);


/*
This function calcualtes the statistical variables needed for the autoCubeGrab
function. This function is to be used in autonomous mode and for programming
skills. samples is the number of samples to take when calibrating
torque, tInterval is the time in pros::c::milliseconds between samples.
Ex(Rollers roller = rollersInit(30, 10);)
*/
rollers rollersInit(int samples, int tInterval);

/*
 Uses the torque exerted by the motors to detect when a cube is present in the
 inner rollers(arms). This function is to be used in autonomous mode and for
 programming skills.r1 is the rollers structure contatiing statisics information
 zScore is the z score corresponding to the confidence level(alpha) for the test.
 Returns 0 when done. Recommended declaration autoCubeGrab(roller, -3);
*/
int innerRollerBump(rollers r1, double zScore, int maxTime);

/*
 Uses the torque exerted by the motors to detect when a cube is present in the
 outer rollers(arms). This function is to be used in autonomous mode and for
 programming skills.r1 is the rollers structure contatiing statisics information
 zScore is the z score corresponding to the confidence level(alpha) for the test.
 Returns 0 when done. Recommended declaration autoCubeGrab(roller, 3);
*/
int outerRollerBump(rollers r1, double zScore, int maxTime);

#endif
