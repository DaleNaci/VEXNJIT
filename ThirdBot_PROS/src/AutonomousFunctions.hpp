#include "main.h"
#include "Variables.hpp"

#ifndef AUTONOMOUSFUNCTIONS_HPP
#define AUTONOMOUSFUNCTIONS_HPP


/*
 Uses the torque exerted by the motors to detect when a cube is present in the
 inner rollers(arms). This function is to be used in autonomous mode and for
 programming skills.r1 is the rollers structure contatiing statisics information
 zScore is the z score corresponding to the confidence level(alpha) for the test.
 eject is for if it should pu the cube in place for towering, true for towering,
 false for putting it in the tray. Returns 0 when done. Recommended declaration
 autoCubeGrab(roller, 3, false);
*/
int autoCubeGrab(rollers r1, double zScore,bool eject, int maxTime);

/*
	Used with the ultrasonic sensor to find the center of a pole and turn to it.
	Length is the number of data points to collect, range [5 is a good number] is
	the number of data points to sample on each side of a point for an average,
	tInterval is the time in pros::c::milliseconds between samples minimum is 50 milliseconds, decay [0.0 – 1.0] how
	slowly to decay from raw value.Speed determines the turn speed
*/
void centerDetect(int length, int range, int tInterval, double decay, int speed);//see also low-pass-filter-method now at [https://web.archive.org/web/20180922093343/http://www.robosoup.com/2014/01/cleaning-noisy-time-series-data-low-pass-filter-c.html] (no longer exists)[https://www.robosoup.com/2014/01/cleaning-noisy-time-series-data-low-pass-filter-c.html]

/*
 This functions serchs for and then navigates towards a  tower using a line
 follower like algorithm. lowerBound is the distacne a point must be above to
 trigger the navigation start, upperBound is the value the point must be below,
 tInterval is the time in milliseconds between samples(minimum of 50(approx:
 refresh rate of sensor)), turnSpeed is the speed the robot will turn at where
 positive turnSpeed is clockwise turning and negative turnSpeed is
 counterclockwise turning, moveSpeed is the speed to move the robot where
 positive moveSpeed is forward and negative moveSpeed is backward deployDistance
 is the distance in ultrasonic sensor units to move the arm up at.
*/
void towerDetect(int lowerBound, int upperBound, int tInterval, int turnSpeed, int moveSpeed, int deployDistance);


#endif
