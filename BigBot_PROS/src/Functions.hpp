#include "main.h"
#include <string>

using namespace std;

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollers(int speed);

/**
 * Moves the roller lift to a specific relative position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void rollersPosition(int pos, int speed);

/**
 * Moves the roller lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void lift(int speed);

/**
 * Moves the roller lift to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void liftPosition(int pos, int speed);

/**
 * Moves the tilter. Speed will depend on the speed parameter. The range
 * is -100 to 100. If speed is 0, the motor will stop with a brakeType
 * of "hold."
*/
void tilter(int speed);

/**
 * Moves the tilter to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void tilterPosition(int pos, int speed);

/**
 * Moves the rollers to intake and outtake depending on the state of
 * the up and down buttons. If the up button is pressed, the
 * manipulator will intake, and if the down button is pressed, the
 * manipulator will outtake. The up button has priority. Also, if the
 * lift is up, the rollers will move at a slower speed in both
 * directions. In addition, if the tilter is up, the rollers will move
 * slowly in the outwards direction to make it easier to deposit stacks.
*/
void rollersControl();

/**
 * Move the lift up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void liftControl();

/**
 * Moves different mechanisms to certain positions based on the
 * parameter, preset. Each preset will call different functions.
*/
void presets(string preset);

/**
 * Moves the lift to a specific height, depending on the button pressed.
*/
void presetControl();

/**
 * Move the tray up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority. When the tray is moving up, the closer it is to
 * its resting position, the faster it will be. Once it reaches a
 * certain point in its range of motion, it is capped at a maximum
 * velocity.
*/
void tilterControl();

/**
 * This runs the drive on an arcade control setup. The left vertical
 * axis moves the drive up and down, while the right horizontal axis
 * point the drive left and right.
*/
void driveControl();

/**
 * Turns the robot clockwise to a certain angle (angle) with a certain
 * velocity (speed).
*/
void turn(QAngle angle, int speed);

/**
 * Turns the robot clockwise to a certain angle (input) using the PD
 * part of a PID controller with a high level of precision.
*/
void pidTurn(double input);

/**
 * Runs the path (pathName). It can take in the "reversed" and
 * "mirrored," but by default it treats both of them as false. This
 * function will hold the code in place until it successfully reaches
 * its target.
*/
void runPath(string pathName, bool reversed=false, bool mirrored=false);

/**
 * Runs the path (pathName) at a slower pace than normal. It can take in
 * the "reversed" and "mirrored," but by default it treats both of them
 * as false. This function will hold the code in place until it
 * successfully reaches its target.
*/
void slowPath(string pathName, bool reversed=false, bool mirrored=false);

/**
 * This is the "control" function that picks the auton depending on the
 * given parameter (selected).
*/
void autonSelect(string selected);

#endif
