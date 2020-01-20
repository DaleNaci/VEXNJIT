#include "main.h"

using namespace std;

/**
 * These are the variable constants for the ports.
*/
int8_t LEFT_ROLLER_PORT = 14;
int8_t RIGHT_ROLLER_PORT = 10;
int8_t TILTER_1_PORT = 12;
int8_t TILTER_2_PORT = 18;
int8_t LEFT_FRONT_DRIVE_PORT = 17;
int8_t LEFT_BACK_DRIVE_PORT = 13;
int8_t RIGHT_FRONT_DRIVE_PORT = 15;
int8_t RIGHT_BACK_DRIVE_PORT = 16;


/**
 * These are the different motor variables that are used to move
 * different parts of the robot.
*/
Motor rollerL(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rollerR(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor tilter1(TILTER_1_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor tilter2(TILTER_2_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor driveFL(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBL(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveFR(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBR(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);


/**
 * These are the variables that are used to configure controller inputs
 * to perform various tasks for the robot.
*/
Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);
ControllerButton trayDown(ControllerDigital::R2);
ControllerButton trayUp(ControllerDigital::R1);

ControllerButton presetA(ControllerDigital::Y);


/**
 * These are the chassis variables that are used for the driver control
 * period and the autonomous period.
*/
auto chassis = ChassisControllerBuilder()
	.withMotors(
		{driveFL, driveBL},
		{driveFR, driveBR}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{4.08_in, 8.1_in},
			static_cast<int32_t>(imev5GreenTPR * 2.0)
		}
	).build();

auto profileController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.3,
			1.4,
			6.0
		}
	).withOutput(
		chassis
	).buildMotionProfileController();


/**
 * This code is used at the beginning, when the program starts.
 * This function is mainly used to set up motor brakemodes and encoders,
 * and then generate paths (2D motion profiling) for auton.
*/
void initialize() {
	rollerL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilter1.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilter2.setBrakeMode(AbstractMotor::brakeMode::hold);

	tilter1.tarePosition();
	tilter2.tarePosition();

	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.8_ft, 0_ft, 0_deg}
		},
		"A"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1_ft, 0_ft, 0_deg}
		},
		"B"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2_ft, 1.1_ft, 15_deg}
		},
		"C"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.6_ft, 0_ft, 0_deg}
		},
		"D"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.5_ft, -1_ft, 0_deg}
		},
		"E"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{3.8_ft, 0_ft, 0_deg}
		},
		"F"
	);

	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.9_ft, 0_ft, 0_deg}
		},
		"1"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2_ft, 0_ft, 0_deg}
		},
		"2"
	);
}


/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollers(int speed) {
	rollerL.moveVelocity(speed);
	rollerR.moveVelocity(-speed);
}


/**
 * Moves the roller lift to a specific relative position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void rollersPosition(int pos, int speed) {
	rollerL.moveAbsolute(pos, speed);
	rollerR.moveAbsolute(-pos, speed);
}


/**
 * Moves the tilter. Speed will depend on the speed parameter. The range
 * is -100 to 100. If speed is 0, the motor will stop with a brakeType
 * of "hold."
*/
void tilter(int speed) {
	tilter1.moveVelocity(speed);
	tilter2.moveVelocity(-speed);
}


/**
 * Moves the tilter to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void tilterPosition(int pos, int speed) {
	tilter1.moveAbsolute(pos, speed);
	tilter2.moveAbsolute(-pos, speed);
}


/**
 * Moves the rollers to intake and outtake depending on the state of
 * the up and down buttons. If the up button is pressed, the
 * manipulator will intake, and if the down button is pressed, the
 * manipulator will outtake. The up button has priority.
*/
void rollersControl() {
	if (intakeIn.isPressed()) {
		rollers(100);
	} else if (intakeOut.isPressed()) {
		rollers(-100);
	} else {
		rollers(0);
	}
}


/**
 * Move the tray up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority. When the tray is moving up, the closer it is to
 * its resting position, the faster it will be. Once it reaches a
 * certain point in its range of motion, it is capped at a maximum
 * velocity.
*/
void tilterControl() {
	if (trayUp.changedToPressed()) {
		tilter(80);
	} else if (trayDown.changedToPressed()) {
		tilter(-65);
	}
	if (trayUp.changedToReleased() || trayDown.changedToReleased()) {
		if (tilter2.getTargetVelocity() != 100) {
			tilter(0);
		}
	}
}


/**
 * This runs the drive on an arcade control setup. The left vertical
 * axis moves the drive up and down, while the right horizontal axis
 * point the drive left and right.
*/
void driveControl() {
	chassis->getModel()->arcade(
		joystick.getAnalog(ControllerAnalog::leftY),
		joystick.getAnalog(ControllerAnalog::rightX) * .6
	);
}


/**
 * Moves different mechanisms to certain positions based on the
 * parameter, preset. Each preset will call different functions.
*/
void presets(string preset) {
	if (preset == "A") {
		tilterPosition(0, 100);
	}
}

/**
 * Moves the lift to a specific height, depending on the button pressed.
*/
void presetControl() {
	if (presetA.isPressed()) {
		presets("A");
	}
}


/**
 * Runs the path (pathName). It can take in the "reversed" and
 * "mirrored," but by default it treats both of them as false. This
 * function will hold the code in place until it successfully reaches
 * its target.
*/
void runPath(string pathName, bool reversed=false, bool mirrored=false) {
	profileController->setTarget(pathName, reversed, mirrored);
	profileController->waitUntilSettled();
}


/**
 * Turns the robot clockwise to a certain angle (angle) with a certain
 * velocity (speed).
*/
void turn(QAngle angle, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->turnAngle(angle / 2.2);
	chassis->setMaxVelocity(200);
}


/**
 * Runs the autonomous function for the auton period.
*/
void autonomous() {
	runPath("1", true);
	runPath("2");
}

/**
 * This function is called by the competition switch, and will call all
 * of the control functions that are used during the driver control
 * period.
*/
void opcontrol() {
	while (true) {
		rollersControl();
		tilterControl();
		driveControl();

		pros::delay(20);
	}
}


/**
 * Unused methods that are required by PROS. They might be used later.
*/
void disabled() {}
void competition_initialize() {}
void on_center_button() {}
