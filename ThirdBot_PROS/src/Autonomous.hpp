#include "main.h"

#ifndef AUTONOMOUS_HPP
#define AUTONOMOUS_HPP

/*
Master Auton fucntion
*/
void auton(bool blue);
/*
Runs the blue side autonomous code
*/
void autonBlue();

/*
Runs the red side autonomous code
*/
void autonRed();

//backup auton path for one cube push, antitip wheel will push
/*runPath("1", true);
runPath("2");*/

#endif
