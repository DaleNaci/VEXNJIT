#include "main.h"
#include "Motors.hpp"
#include "Variables.hpp"
#include "DriveFunctions.hpp"
#include "LiftFunctions.hpp"
#include "RollerFunctions.hpp"
#include "TilterFunctions.hpp"
#include "AssistFunctions.hpp"
#include "ControlFunctions.hpp"


/*
 This runs the drive on an arcade control setup. The left vertical
 axis moves the drive up and down, while the right horizontal axis
 point the drive left and right.
*/
void driveControl() {
	chassis->getModel()->arcade(
		joystick.getAnalog(ControllerAnalog::leftY),
		joystick.getAnalog(ControllerAnalog::rightX) * .6
	);
}

/*
 Task runnable version of driveControl
*/
void driveControl1(void* param) {
	while(true) {
		driveControl();
		pros::delay(20);
	}
}


/*
 Moves the lift up or down, depending on the state of the up and down
 buttons(D pad). If the up button is pressed, the lift will move upwards. If
 the down button is pressed, the lift will move downwards. The up
 button has priority.
*/
void liftControl() {
	if (liftUp.changedToPressed()) {
		lift(60);
		if(armL.getPosition() > 100){
			armUp = true;
		} else {
			armUp = false;
		}
	} else if (liftDown.changedToPressed()) {
		lift(-60);
		if(armL.getPosition() < 100) {
			armUp = false;
		}
		if (armL.getPosition() < 0) {
			armL.tarePosition();
			armR.tarePosition();
			armUp = false;
		}
	}
	if (liftUp.changedToReleased() || liftDown.changedToReleased()) {
		lift(0);
		if(armL.getPosition() > 100) {
			armUp = true;
		}
	}
}

/*
 Moves the rollers to intake and outtake depending on the state of
 the up and down buttons. If the up button is pressed, the
 manipulator will intake, and if the down button is pressed, the
 manipulator will outtake. The up button has priority.
*/
void rollersControl() {
	if (armL.getPosition() > 600) {
		rollersArms(0);
		if (intakeIn.isPressed()) {
			rollersTray(100);
		} else if (intakeOut.isPressed()) {
			rollersTray(-100);
		} else {
			rollersTray(0);
		}
	} else if (armL.getPosition() > 100) {
		rollersTray(0);
		if (intakeIn.isPressed()) {
			rollersArms(100);
		} else if (intakeOut.isPressed()) {
			rollersArms(-5);
		} else {
			rollersArms(0);
		}
	} else {
		if (intakeIn.isPressed()) {
			rollersArms(70);
			rollersTray(100);
			//pros::c::lcd_print(0, "Current Draw: %f ma",rollerarmL.getEfficiency());
		} else if (intakeOut.isPressed()) {
			rollersArms(-70);
			rollersTray(-100);
		} else {
			rollersArms(0);
			rollersTray(0);
		}
	}
}

/*
 Move the tray up or down, depending on the state of the up and down
 buttons. If the up button is pressed, the tray will move upwards. If
 the down button is pressed, the tray will move downwards. The up
 button has priority. When the tray is moving up, the closer it is to
 its resting position, the faster it will be. Once it reaches a
 certain point in its range of motion, it is capped at a maximum
 velocity.
*/
void tilterControl() {
	if (trayUp.changedToPressed()) {
		//If the the trayUp button is pressed, move the tray up at speed 80
		tilter(80);
	} else if (trayDown.changedToPressed()) {
		/*If the tilter(tray) is not all of the way up, then move it down at speed 100
		while the trayDown button is pressed*/
		if (tilterL.getPosition() < 100) {
			tilter(-100);
		} else {
			int vel = 50 + (250 + (-tilterL.getPosition() + 100)) * 0.2000;
			if (vel < 50) {
				vel = 50;
			}
			tilter(-vel);
		}
	}
	//When the tray control buttons are released, stop moving the tray
	if (trayUp.changedToReleased() || trayDown.changedToReleased()) {
		if (tilterL.getTargetVelocity() != 100) {
			tilter(0);
		}
	}
}

/*
 Moves the lift to a specific height, depending on the button pressed.
*/
void presetControl() {
	if (presetA.isPressed()) {
		presets("A");
	}
	if (presetX.isPressed()) {
		presets("X");
	}
	if (presetB.isPressed()) {
		presets("B");
	}
	if (presetY.isPressed()) {
		presets("Y");
	}
	if (toggleAuto.changedToPressed()) {
	 presets("left");
 	}
	if (presetRight.isPressed()) {
	presets("right");
 	}
}
