#include "main.h"

using namespace std;

int8_t LEFT_ROLLER_PORT = 19;
int8_t RIGHT_ROLLER_PORT = 1;
int8_t LEFT_LIFT_PORT = 16;
int8_t RIGHT_LIFT_PORT = 5;
int8_t TILTER_PORT = 15;
int8_t LEFT_DRIVE_1_PORT = 18;
int8_t LEFT_DRIVE_2_PORT = 13;
int8_t LEFT_DRIVE_3_PORT = 17;
int8_t LEFT_DRIVE_4_PORT = 20;
int8_t RIGHT_DRIVE_1_PORT = 2;
int8_t RIGHT_DRIVE_2_PORT = 6;
int8_t RIGHT_DRIVE_3_PORT = 4;
int8_t RIGHT_DRIVE_4_PORT = 7;

Motor rollerL(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor rollerR(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

Motor liftL(LEFT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor liftR(RIGHT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor tilter1(TILTER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);


Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);

ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);

ControllerButton trayDown(ControllerDigital::R1);
ControllerButton trayUp(ControllerDigital::R2);

ControllerButton presetX(ControllerDigital::X);
ControllerButton presetA(ControllerDigital::A);
ControllerButton presetB(ControllerDigital::B);
ControllerButton presetY(ControllerDigital::Y);


auto chassis = ChassisControllerBuilder()
	.withMotors(
		{LEFT_DRIVE_1_PORT, static_cast<int8_t>(-LEFT_DRIVE_2_PORT), static_cast<int8_t>(-LEFT_DRIVE_3_PORT), LEFT_DRIVE_4_PORT},
		{static_cast<int8_t>(-RIGHT_DRIVE_1_PORT), RIGHT_DRIVE_2_PORT, RIGHT_DRIVE_3_PORT, static_cast<int8_t>(-RIGHT_DRIVE_4_PORT)}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{6.1_in, 12.5_in},
			static_cast<int32_t>(imev5GreenTPR * 2.0)
		}
	).build();

auto profileController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.42,
			0.8,
			5.2
		}
	).withOutput(
		chassis
	).buildMotionProfileController();

auto cubeIntakeController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.25,
			1.5,
			3.0
		}
	).withOutput(
		chassis
	).buildMotionProfileController();

auto slowController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.2,
			0.8,
			1.4
		}
	).withOutput(
		chassis
	).buildMotionProfileController();


void initialize() {
	liftL.setBrakeMode(AbstractMotor::brakeMode::hold);
	liftR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilter1.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerR.setBrakeMode(AbstractMotor::brakeMode::hold);

	liftL.tarePosition();
	liftR.tarePosition();
	tilter1.tarePosition();

	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{.4_ft, 0_ft, 0_deg}
		},
		"A"
	);
	cubeIntakeController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.8_ft, 0_ft, 0_deg}
		},
		"B"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.8_ft, -2.1_ft, 0_deg}
		},
		"C"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.8_ft, 2.1_ft, 0_deg}
		},
		"C_alt"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.05_ft, 0_ft, 0_deg}
		},
		"D"
	);
	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.8_ft, 0_ft, 0_deg}
		},
		"E"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.4_ft, 0_ft, 0_deg}
		},
		"F"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.25_ft, 0_ft, 0_deg}
		},
		"Small"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.4_ft, 0_ft, 0_deg}
		},
		"Wall_Back"
	);


	pros::lcd::initialize();
}


/**
 * Moves both roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollers(int speed) {
	rollerL.moveVelocity(-speed);
	rollerR.moveVelocity(speed);
}


/**
 * Moves the roller lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void lift(int speed) {
	liftL.moveVelocity(speed * 2);
	liftR.moveVelocity(-speed * 2);
}


/**
 * Moves the roller lift to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void liftPosition(int pos, int speed) {
	liftL.moveAbsolute(pos, speed * 2);
	liftR.moveAbsolute(-pos, speed * 2);
}


/**
 * Moves the tilter. Speed will depend on the speed parameter. The range
 * is -100 to 100. If speed is 0, the motor will stop with a brakeType
 * of "hold."
*/
void tilter(int speed) {
	tilter1.moveVelocity(speed);
}


/**
 * Moves the tilter to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void tilterPosition(int pos, int speed) {
	tilter1.moveAbsolute(pos, speed);
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
 * Move the lift up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void liftControl() {
	if (liftUp.isPressed()) {
		lift(90);
	} else if (liftDown.isPressed()) {
		lift(-90);
	}

	if (liftUp.changedToReleased() || liftDown.changedToReleased()) {
		lift(0);
	}
}


/**
 * Moves different mechanisms to certain positions based on the
 * parameter, preset. Each preset will call different functions.
*/
void presets(string preset) {
	if (preset == "X") {
		liftPosition(690, 80);
	}
	if (preset == "A") {
		liftPosition(905, 80);
	}
	if (preset == "B") {
		liftPosition(5, 80);
	}
	if (preset == "Y") {
		tilterPosition(0, 80);
	}
}


/**
 * Moves the lift to a specific height, depending on the button pressed
 * in the XYAB button mapping. If there is a significant difference
 * between the two arms, they will stop and automatically re-align.
*/
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
}


/**
 * Move the tray up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void tilterControl() {
	if (trayUp.isPressed() && tilter1.getTargetVelocity() != 40) {
		tilter(80);
	} else if (trayDown.isPressed()) {
		tilter(-65);
	}
	if (trayUp.changedToReleased() || trayDown.changedToReleased()) {
		tilter(0);
	}


}


void driveControl() {
	chassis->getModel()->arcade(
		joystick.getAnalog(ControllerAnalog::leftY),
		joystick.getAnalog(ControllerAnalog::rightX)
	);
}


/**
 * Turns the robot clockwise to a certain angle (angle) with a certain
 * velocity (speed).
*/
void turn(QAngle angle, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->turnAngle(angle);
	chassis->setMaxVelocity(200);
}


void red() {
	profileController->setTarget("Small");
	profileController->waitUntilSettled();
	profileController->setTarget("Small", true);
	profileController->waitUntilSettled();

	presets("X");
	pros::delay(2000);
	presets("B");
	pros::delay(1000);

	profileController->setTarget("A");
	profileController->waitUntilSettled();
	rollers(-100);
	cubeIntakeController->setTarget("B");
	cubeIntakeController->waitUntilSettled();
	pros::delay(40);
	rollers(0);

	profileController->setTarget("C_alt", true);
	profileController->waitUntilSettled();

	profileController->setTarget("Wall_Back", true);
	profileController->waitUntilSettled();

	profileController->setTarget("D");
	profileController->waitUntilSettled();
	rollers(-100);
	cubeIntakeController->setTarget("B");
	cubeIntakeController->waitUntilSettled();
	pros::delay(100);
	rollers(0);

	slowController->setTarget("E", true);
	slowController->waitUntilSettled();
}


void blue() {
	profileController->setTarget("Small");
	profileController->waitUntilSettled();
	profileController->setTarget("Small", true);
	profileController->waitUntilSettled();

	presets("X");
	pros::delay(2000);
	presets("B");
	pros::delay(1000);

	profileController->setTarget("A");
	profileController->waitUntilSettled();
	rollers(-100);
	cubeIntakeController->setTarget("B");
	cubeIntakeController->waitUntilSettled();
	pros::delay(40);
	rollers(0);

	profileController->setTarget("C", true);
	profileController->waitUntilSettled();

	profileController->setTarget("Wall_Back", true);
	profileController->waitUntilSettled();

	profileController->setTarget("D");
	profileController->waitUntilSettled();
	rollers(-100);
	cubeIntakeController->setTarget("B");
	cubeIntakeController->waitUntilSettled();
	pros::delay(100);
	rollers(0);

	slowController->setTarget("E", true);
	slowController->waitUntilSettled();

	turn(-93_deg, 20);
}


void testAuton() {
	profileController->setTarget("F");
	profileController->waitUntilSettled();
}


void autonSelect(string selected) {
	if (selected == "red") {
		red();
	} else if (selected == "blue") {
		blue();
	} else if (selected == "test") {
		testAuton();
	}
}


void autonomous() {
	string SELECTED_AUTON = "blue";
	autonSelect(SELECTED_AUTON);
}


void opcontrol() {
	while(true) {
		rollersControl();
		liftControl();
		tilterControl();
		presetControl();
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
