#include "main.h"

#ifndef LIFTFUNCTIONS_HPP
#define LIFTFUNCTIONS_HPP

/*
 Moves the roller lift. Speed will depend on the speed parameter. Positive
 speed moves the lift up, negative speed moves the lift down. The range is -100
 to 100.
*/
void lift(int speed);

/*
 Moves the roller lift to a specified position. Position is based on the pos
 parameter, positive pos will move the lift pos degrees up from the last tare
 angle and negative pos will move it pos degrees down from the last tare angle
 the range is all int numbers; Speed will depend on the speed parameter. The
 range is -100 to 100.
*/
void liftPosition(int pos, int speed);

/*
 Moves the roller lift to a specified position and blocks code execution until
 it reaches the position. Position is based on the pos parameter, positive pos
 will move the lift pos degrees up from the last tare angle and negative pos
 will move it pos degrees down from the last tare angle the range is all int
 numbers; Speed will depend on the speed parameter. The range is -100 to 100.
*/
void liftPositionDelay(int pos, int speed);

/*
 Moves the lift arms down util one of the buttons is pressed and then zeros the
 arms
 */
void liftZero();

#endif
