#include "main.h"
#include "Functions.hpp"
#include "Motors.hpp"
#include "Sensors.hpp"
#include "Variables.hpp"
#include "Motors.hpp"
#include <string>


void rollers(int speed) {
	rollerL.moveVelocity(-speed);
	rollerR.moveVelocity(speed);
}

void rollersPosition(int pos, int speed) {
	rollerL.tarePosition();
	rollerR.tarePosition();
	rollerL.moveAbsolute(-pos, speed);
	rollerR.moveAbsolute(pos, speed);
}

void lift(int speed) {
	liftL.moveVelocity(speed * 2);
	liftR.moveVelocity(-speed * 2);
}

void liftPosition(int pos, int speed) {
	liftL.moveAbsolute(pos, speed * 2);
	liftR.moveAbsolute(-pos, speed * 2);
}

void tilter(int speed) {
	tilter1.moveVelocity(speed);
}

void tilterPosition(int pos, int speed) {
	tilter1.moveAbsolute(pos, speed);
}

void rollersControl() {
	if (liftL.getPosition() > 400) {
		if (intakeIn.isPressed()) {
			rollers(56);
		} else if (intakeOut.isPressed()) {
			rollers(-40);
		} else {
			rollers(0);
		}
	} else if (tilter1.getPosition() < -500) {
		if (intakeIn.isPressed()) {
			rollers(100);
		} else if (intakeOut.isPressed()) {
			rollers(-20);
		} else {
			rollers(0);
		}
	} else {
		if (intakeIn.isPressed()) {
			rollers(100);
		} else if (intakeOut.isPressed()) {
			rollers(-100);
		} else {
			rollers(0);
		}
	}
}

void liftControl() {
	if (liftUp.isPressed()) {
		lift(90);
	} else if (liftDown.isPressed()) {
		lift(-90);
		if (liftL.getPosition() < 0) {
			liftL.tarePosition();
			liftR.tarePosition();
		}
	}

	if (liftUp.changedToReleased() || liftDown.changedToReleased()) {
		lift(0);
	}
	if (trayUp.changedToPressed()) {
		liftPosition(0, 100);
	}
}

void presets(string preset) {
	if (preset == "X") {
		tilterPosition(-750, -100);
		if (tilter1.getPosition() < -240) {
			liftPosition(730, 100);
		}
	}
	if (preset == "A") {
		tilterPosition(-750, -100);
		if (tilter1.getPosition() < -240) {
			liftPosition(930, 100);
		}
	}
	if (preset == "B") {
		liftPosition(140, 100);
		tilterPosition(0, 100);
	}
	if (preset == "Left") {
		liftPosition(0, 100);
	}
	if (preset == "Y") {
		liftPosition(270, 100);
		tilterPosition(-300, 100);
	}
}

void presetControl() {
	if (presetX.isPressed()) {
		presets("X");
	}
	if (presetA.isPressed()) {
		presets("A");
	}
	if (presetB.isPressed()) {
		presets("B");
	}
	if (presetY.isPressed()) {
		presets("Y");
	}
	if (presetLeft.isPressed()) {
		presets("Left");
	}
}

void tilterControl() {
	if (trayDown.isPressed() && tilter1.getTargetVelocity() != 40) {
		tilter(100);
		if (tilter1.getPosition() > 0) {
			tilter1.tarePosition();
		}
	} else if (trayUp.isPressed()) {
		if (tilter1.getPosition() > -250) {
			tilter(-100);
		} else {
			int vel = 50 + (450 + (tilter1.getPosition() + 250)) * 0.1111;
			if (vel < 50) {
				vel = 50;
			}
			tilter(-vel);
		}
	}
	if (trayDown.changedToReleased() || trayUp.changedToReleased()) {
		tilter(0);
	}
}


void driveControl() {
	chassis->getModel()->arcade(
		joystick.getAnalog(ControllerAnalog::leftY),
		joystick.getAnalog(ControllerAnalog::rightX)
	);
}

void turn(QAngle angle, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->turnAngle(angle / 2);
	chassis->setMaxVelocity(200);
}


void pidTurn(double input) {
	double angle = input * 2.97;

	encoderL.reset();
	encoderR.reset();

	double TARGET = encoderL.get_value() + angle;
	double HALFWAY = encoderL.get_value() + angle / 4;
	double currentValue = encoderL.get_value();
	double currentError = TARGET - currentValue;
	double previousError = 0;
	double difference = encoderL.get_value() - encoderR.get_value();
	double accel = true;

	double kP = 6.000;
	double kI = 0.000;
	double kD = 11.500;
	double kDr = 0.000;

	double maxRate = 21;

	while (fabs(currentError) > 5) {
		if (angle > 0 && currentValue > HALFWAY) {
			accel = false;
		} else if (angle < 0 && currentValue < HALFWAY) {
			accel = false;
		}

		double p = kP * currentError;
		double i = kI;
		double d = kD * (currentError - previousError);
		double dr = kDr * difference;

		double command = p + i + d;

		if (fabs(command) > maxRate) {
			if (command > 0) {
				command = maxRate;
			} else {
				command = -maxRate;
			}
		}

		double left = command;
		double right = -command;

		driveL1.moveVelocity(left);
		driveL2.moveVelocity(left);
		driveL3.moveVelocity(left);
		driveL4.moveVelocity(left);
		driveR1.moveVelocity(right);
		driveR2.moveVelocity(right);
		driveR3.moveVelocity(right);
		driveR4.moveVelocity(right);

		pros::delay(20);

		if (accel) {
			if (maxRate < 45) {
				maxRate += 1;
			}
		}

		currentValue = encoderL.get_value();
		previousError = currentError;
		currentError = TARGET - currentValue;
	}

	driveL1.moveVelocity(0);
	driveL2.moveVelocity(0);
	driveL3.moveVelocity(0);
	driveL4.moveVelocity(0);
	driveR1.moveVelocity(0);
	driveR2.moveVelocity(0);
	driveR3.moveVelocity(0);
	driveR4.moveVelocity(0);
}
