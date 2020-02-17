#include "main.h"
#include "Motors.hpp"
#include "LiftFunctions.hpp"
/*
 Moves the roller lift. Speed will depend on the speed parameter. Positive
 speed moves the lift up, negative speed moves the lift down. The range is -100
 to 100.
*/
void lift(int speed) {
	armL.moveVelocity(speed * 2);
	armR.moveVelocity(speed * 2);
}

/*
 Moves the roller lift to a specified position. Position is based on the pos
 parameter, positive pos will move the lift pos degrees up from the last tare
 angle and negative pos will move it pos degrees down from the last tare angle
 the range is all int numbers; Speed will depend on the speed parameter. The
 range is -100 to 100.
*/
void liftPosition(int pos, int speed) {
    armL.moveAbsolute(pos, speed);
    armR.moveAbsolute(pos, speed);
}
