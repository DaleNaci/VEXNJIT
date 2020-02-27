#include "main.h"
#include "Motors.hpp"
#include "Sensors.hpp"
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

/*
 Moves the roller lift to a specified position and blocks code execution until
 it reaches the position. Position is based on the pos parameter, positive pos
 will move the lift pos degrees up from the last tare angle and negative pos
 will move it pos degrees down from the last tare angle the range is all int
 numbers; Speed will depend on the speed parameter. The range is -100 to 100.
*/
void liftPositionDelay(int pos, int speed)
{
	int currentPos = armL.getPosition();//get the current position of the motor(in encoder units)
	liftPosition(pos, speed);//set the motor target postion and position

	if(currentPos < pos)
	{
		while (armL.getPosition() < pos - 1) { //halts code exectuion until the arms reach their intended position with a minus one for error
			pros::delay(1);
		}
	}else{
		while (armL.getPosition() > pos + 1) { //halts code exectuion until the arms reach their intended position with a plus one for error
			pros::delay(1);
		}
	}
}


/*
 Moves the lift arms down util one of the buttons is pressed and then zeros the
 arms
 */
void liftZero(){
	if(armLStop.isPressed() && armRStop.isPressed()){}else{
		lift(-100);//Brings the arms back down
		//Stops the arms when one or both arms triggers the zero button
		while (true) {
			if (armLStop.changedToPressed() || armRStop.changedToPressed()) {
				break;
			}
		}
	}
		//pushes the lift arms into the buttons
		armL.moveVelocity(-10);
		armR.moveVelocity(-10);

		pros::delay(200);

		//stops moving the lift arms
		armL.moveVelocity(0);
		armR.moveVelocity(0);
		pros::delay(200);
		/*Since no brakeMode is set yet, the arms should fall to a natural resting
		position during the delay and will be synced together when they are zeroed*/

		//Zeros the lift arm position
		armL.tarePosition();
		armR.tarePosition();

		//Zeros the shaft encoders on the lift arms
		armLEncoder.reset();
		armREncoder.reset();

		pros::delay(5);

		armL.setBrakeMode(AbstractMotor::brakeMode::hold);
		armR.setBrakeMode(AbstractMotor::brakeMode::hold);

}
