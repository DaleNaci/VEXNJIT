#include "main.h"

using namespace std;

int LEFT_FRONT_DRIVE_PORT = 20;
int LEFT_BACK_DRIVE_PORT = 10;
int RIGHT_FRONT_DRIVE_PORT = 11;
int RIGHT_BACK_DRIVE_PORT = 1;
int LEFT_CLAMP_PORT = 18;
int RIGHT_CLAMP_PORT = 15;
int TOP_LEFT_LIFT_PORT = 9;
int BOTTOM_LEFT_LIFT_PORT = 7;
int TOP_RIGHT_LIFT_PORT = 2;
int BOTTOM_RIGHT_LIFT_PORT = 3;

Controller joystick;

Motor leftClampMotor(LEFT_CLAMP_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor rightClampMotor(RIGHT_CLAMP_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor topLeftLiftMotor(TOP_LEFT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor bottomLeftLiftMotor(BOTTOM_LEFT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor topRightLiftMotor(TOP_RIGHT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor bottomRightLiftMotor(BOTTOM_RIGHT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor leftFrontDriveMotor(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor leftBackDriveMotor(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rightFrontDriveMotor(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rightFrontBackMotor(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);


ControllerButton clampInBtn(ControllerDigital::L1);
ControllerButton clampOutBtn(ControllerDigital::L2);
ControllerButton liftUpBtn(ControllerDigital::R1);
ControllerButton liftDownBtn(ControllerDigital::R2);

ControllerButton presetX(ControllerDigital::X);
ControllerButton presetA(ControllerDigital::A);
ControllerButton presetB(ControllerDigital::B);
ControllerButton presetY(ControllerDigital::Y);


auto chassis = ChassisModelFactory::create(
	leftFrontDriveMotor,
	rightFrontDriveMotor,
	rightFrontBackMotor,
	leftBackDriveMotor,
	200.0
);



void on_center_button() {}



void initialize() {
	leftClampMotor.tarePosition();
	rightClampMotor.tarePosition();
	topLeftLiftMotor.tarePosition();

	topLeftLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	topRightLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	bottomLeftLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	bottomRightLiftMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	leftClampMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	rightClampMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	pros::lcd::initialize();

}



void disabled() {}
void competition_initialize() {}
void autonomous() {}


void clamp(int speed) {
	leftClampMotor.moveVelocity(speed);
	rightClampMotor.moveVelocity(-speed);
}


/**
 * Moves the clamp to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void clampPosition(int pos, int speed) {
	leftClampMotor.moveAbsolute(pos, speed);
	rightClampMotor.moveAbsolute(-pos, speed);
}


/**
 *
*/
void clampControl() {
	if (clampInBtn.isPressed()) {
		clamp(60);
	} else if (clampOutBtn.isPressed()) {
		clamp(-60);
	} else {
		clamp(0);
	}
}


/**
 * Moves all 4 lift motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void lift(int speed) {
	topLeftLiftMotor.moveVelocity(-speed);
	topRightLiftMotor.moveVelocity(speed);
	bottomLeftLiftMotor.moveVelocity(speed);
	bottomRightLiftMotor.moveVelocity(-speed);
}


/**
 * Moves the lift to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void liftPosition(int pos, int speed) {
	topLeftLiftMotor.moveAbsolute(-pos, speed);
	bottomLeftLiftMotor.moveAbsolute(pos, speed);
	topRightLiftMotor.moveAbsolute(pos, speed);
	bottomRightLiftMotor.moveAbsolute(-pos, speed);
}

/**
 *
*/
void liftControl() {
	if (liftUpBtn.isPressed()) {
		lift(80);
	} else if (liftDownBtn.isPressed() && topLeftLiftMotor.getPosition() < -30) {
		lift(-50);
	}

	if (liftUpBtn.changedToReleased() || liftDownBtn.changedToReleased()) {
		lift(0);
	}
}


void liftPresets() {
	if (presetX.isPressed()) { //middle tower height preset
		liftPosition(350, 50);//was350
	}
	if (presetA.isPressed()) { //preset for lifting a cube before moving
		liftPosition(100, 50);
	}
	if (presetB.isPressed()) { //full down preset
		liftPosition(0, 40);
	}
	if (presetY.isPressed()) { // lower tower height preset
		liftPosition(290, 50); //was 290
	}
}


void opcontrol() {
	while (true) {
		chassis.xArcade(
			joystick.getAnalog(ControllerAnalog::leftX) * 0.9,
			joystick.getAnalog(ControllerAnalog::leftY),
			joystick.getAnalog(ControllerAnalog::rightX) * 0.7
		);

		clampControl();
		liftControl();
		liftPresets();


		pros::delay(20);
	}
}
