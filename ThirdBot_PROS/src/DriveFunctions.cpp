#include "main.h"
#include "Variables.hpp"
#include "Motors.hpp"
#include "DriveFunctions.hpp"

/*
 Sets the left motors velocity to speedL and the right motors velocity to speedR
 to turn the chassis
*/
void turn2(int speedL, int speedR){
	driveFL.moveVelocity(speedL);
	driveBL.moveVelocity(speedL);
	driveFR.moveVelocity(speedR);
	driveBR.moveVelocity(speedR);
}

/*
 Turns the robot to a specified angle, positive angle means teh robot turns ccw
 and a negatoive angle means it turns c, speed is the velocity of the motors
*/
void turnAngle2(int angle, int speed){
	turn2(-speed,speed);
	float ctime = 26000 / 10 / 360;//the conversion from milliseconds to degrees
	int time = (int)(angle * ctime) + 100;
	pros::delay(time);
	turn2(0,0);
}

/*
 Turns the robot clockwise to a certain angle (angle) with a certain
 velocity (speed).
*/
void turn(QAngle angle, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->turnAngle(angle / 2.2);
	chassis->setMaxVelocity(200);
}

/*
 Sets all drive motors velocity to speed, range -100 - 100
*/
void move(int speed){
	driveFL.moveVelocity(speed);
	driveBL.moveVelocity(speed);
	driveFR.moveVelocity(speed);
	driveBR.moveVelocity(speed);
}
