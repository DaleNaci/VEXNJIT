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

//Deploy tray and push one cube into the goal zone
void autonPush();

#endif
