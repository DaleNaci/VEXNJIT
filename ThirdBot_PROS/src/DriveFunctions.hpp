#include "main.h"

#ifndef DRIVEFUNCTIONS_HPP
#define DRIVEFUNCTIONS_HPP
/*
 Sets the left motors velocity to speedL and the right motors velocity to speedR
 to turn the chassis
*/
void turn2(int speedL, int speedR);

/*
 Turns the robot to a specified angle, positive angle means teh robot turns ccw
 and a negatoive angle means it turns c, speed is the velocity of the motors
*/
void turnAngle2(int angle, int speed);

/*
 Sets all drive motors velocity to speed, range -100 - 100
*/
void move(int speed);

#endif
