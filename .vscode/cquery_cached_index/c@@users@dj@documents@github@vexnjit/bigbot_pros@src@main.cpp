#include "main.h"

using namespace std;

/**
 * These are the variable constants for the ports.
*/
int8_t LEFT_ROLLER_PORT = 11;
int8_t RIGHT_ROLLER_PORT = 3;
int8_t LEFT_LIFT_PORT = 16;
int8_t RIGHT_LIFT_PORT = 5;
int8_t TILTER_PORT = 15;
int8_t LEFT_DRIVE_1_PORT = 18;
int8_t LEFT_DRIVE_2_PORT = 13;
int8_t LEFT_DRIVE_3_PORT = 17;
int8_t LEFT_DRIVE_4_PORT = 14;
int8_t RIGHT_DRIVE_1_PORT = 2;
int8_t RIGHT_DRIVE_2_PORT = 6;
int8_t RIGHT_DRIVE_3_PORT = 4;
int8_t RIGHT_DRIVE_4_PORT = 7;
int SWITCH_PORT = 8;
int CONFIRM_BUTTON_PORT = 1;


/**
 * These are the different motor variables that are used to move
 * different parts of the robot.
*/
Motor rollerL(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor rollerR(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

Motor liftL(LEFT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor liftR(RIGHT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor tilter1(TILTER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);


/**
 * These are the variables that are used to configure controller inputs
 * to perform various tasks for the robot.
*/
Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);

ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);

ControllerButton trayUp(ControllerDigital::R1);
ControllerButton trayDown(ControllerDigital::R2);

ControllerButton presetX(ControllerDigital::X);
ControllerButton presetA(ControllerDigital::A);
ControllerButton presetB(ControllerDigital::B);
ControllerButton presetY(ControllerDigital::Y);
ControllerButton presetLeft(ControllerDigital::left);


/**
 * These are the chassis variables that are used for the driver control
 * period and the autonomous period.
*/
auto chassis = ChassisControllerBuilder()
	.withMotors(
		{LEFT_DRIVE_1_PORT, static_cast<int8_t>(-LEFT_DRIVE_2_PORT), static_cast<int8_t>(-LEFT_DRIVE_3_PORT), LEFT_DRIVE_4_PORT},
		{static_cast<int8_t>(-RIGHT_DRIVE_1_PORT), RIGHT_DRIVE_2_PORT, RIGHT_DRIVE_3_PORT, static_cast<int8_t>(-RIGHT_DRIVE_4_PORT)}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{6.34_in, 14.1_in},
			static_cast<int32_t>(imev5GreenTPR * 2.0)
		}
	).build();

auto profileController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.265,
			0.9,
			5.21
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
			{3.0_ft, 0_ft, 0_deg}
		},
		"A"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.8_ft, -2.1_ft, 0_deg}
		},
		"B_blue"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.8_ft, -2.3_ft, 0_deg}
		},
		"B_red"
	);
	// profileController->generatePath(
	// 	{
	// 		{0_ft, 0_ft, 0_deg},
	// 		{3.8_ft, 0_ft, 0_deg}
	// 	},
	// 	"C"
	// );
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.0_ft, 0_ft, 0_deg}
		},
		"C"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.5_ft, 0_ft, 0_deg}
		},
		"Wall"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.5_ft, 0_ft, 0_deg}
		},
		"1"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{5.0_ft, 0_ft, 0_deg}
		},
		"2"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.24_ft, 0_ft, 0_deg}
		},
		"3"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.0_ft, 0_ft, 0_deg}
		},
		"I"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.0_ft, 0_ft, 0_deg}
		},
		"J"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.3_ft, 0_ft, 0_deg}
		},
		"K"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.0_ft, 0_ft, 0_deg}
		},
		"L"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.25_ft, 0_ft, 0_deg}
		},
		"M"
	);
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
 * Moves the roller lift to a specific relative position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void rollersPosition(int pos, int speed) {
	rollerL.tarePosition();
	rollerR.tarePosition();
	rollerL.moveAbsolute(-pos, speed);
	rollerR.moveAbsolute(pos, speed);
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
 * manipulator will outtake. The up button has priority. Also, if the
 * lift is up, the rollers will move at a slower speed in both
 * directions.
*/
void rollersControl() {
	if (liftL.getPosition() > 300) {
		if (intakeIn.isPressed()) {
			rollers(56);
		} else if (intakeOut.isPressed()) {
			rollers(-40);
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
		if (liftL.getPosition() < 0) {
			liftL.tarePosition();
			liftR.tarePosition();
		}
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
		tilterPosition(-670, -100);
		if (tilter1.getPosition() < -240) {
			liftPosition(730, 100);
		}
	}
	if (preset == "A") {
		tilterPosition(-670, -100);
		if (tilter1.getPosition() < -240) {
			liftPosition(930, 100);
		}
	}
	if (preset == "B") {
		liftPosition(150, 100);
		tilterPosition(0, 100);
	}
	if (preset == "Y") {
		tilterPosition(0, 80);
	}
	if (preset == "Left") {
		liftPosition(0, 100);
	}
}


/**
 * Moves the lift to a specific height, depending on the button pressed.
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
	if (presetLeft.isPressed()) {
		presets("Left");
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
	if (trayDown.isPressed() && tilter1.getTargetVelocity() != 40) {
		tilter(90);
		if (tilter1.getPosition() > 0) {
			tilter1.tarePosition();
		}
	} else if (trayUp.isPressed()) {
		int vel = 40 + (350 + tilter1.getPosition()) * 0.171;
		if (vel < 40) {
			vel = 40;
		}
		tilter(-vel);
	}
	if (trayDown.changedToReleased() || trayUp.changedToReleased()) {
		tilter(0);
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
		joystick.getAnalog(ControllerAnalog::rightX)
	);
}


/**
 * Turns the robot clockwise to a certain angle (angle) with a certain
 * velocity (speed).
*/
void turn(QAngle angle, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->turnAngle(angle / 2);
	chassis->setMaxVelocity(200);
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
 * Calling this function runs the red auton.
*/
void red() {
	// rollers(-100);
	// runPath("A");
	// runPath("B_red", true, true);
	// runPath("Wall", true);
	// runPath("C");
	//
	// pros::delay(1200);
	//
	// rollers(0);
	//
	// turn(130_deg, 9);
	// rollers(80);
	// pros::delay(100);
	// rollers(0);
	// runPath("2");
	// runPath("3", true);

	// tilterPosition(-1000, -50);
	// while (tilter1.getPosition() > -950) {
	// 	continue;
	// }
	// pros::delay(800);
	//
	// runPath("I");
	// runPath("I");
	// runPath("J", true);
	// pros::delay(500);
	// turn(45_deg, 20);

	rollers(-100);
	runPath("A");
	runPath("B_red", true, true);
	runPath("Wall", true);
	runPath("C");

	pros::delay(1200);

	rollers(0);

	turn(90_deg, 9);

	runPath("M");

	turn(45_deg, 9);

	rollers(80);
	pros::delay(100);
	rollers(0);
	runPath("2");
	runPath("3", true);
	//
	// tilterPosition(-1000, -50);
	// while (tilter1.getPosition() > -950) {
	// 	continue;
	// }
	// pros::delay(800);
	//
	// runPath("I");
	// runPath("I");
	// runPath("J", true);
	// pros::delay(500);
	// turn(43_deg, 20);
}


/**
 * Calling this function runs the blue auton.
*/
void blue() {
	rollers(-100);
	runPath("A");
	runPath("B_blue", true);
	runPath("Wall", true);
	runPath("C");

	pros::delay(1200);

	rollers(0);

	turn(-90_deg, 9);

	runPath("M");

	turn(-45_deg, 9);

	rollers(80);
	pros::delay(100);
	rollers(0);
	runPath("2");
	runPath("3", true);
	//
	// tilterPosition(-1000, -50);
	// while (tilter1.getPosition() > -950) {
	// 	continue;
	// }
	// pros::delay(800);
	//
	// runPath("I");
	// runPath("I");
	// runPath("J", true);
	// pros::delay(500);
	// turn(-45_deg, 20);
}


/**
 * Calling this function runs programming skills.
*/
void progSkills() {
	rollers(-100);
	runPath("A");
	runPath("B_red", true);
	runPath("Wall", true);
	runPath("C");

	pros::delay(1200);

	rollers(0);

	turn(-131_deg, 9);
	rollers(80);
	pros::delay(100);
	rollers(0);
	runPath("2");
	runPath("3", true);

	tilterPosition(-1000, -50);
	while (tilter1.getPosition() > -950) {
		continue;
	}
	pros::delay(800);

	runPath("I");
	runPath("I");
	runPath("J", true);
	turn(-131_deg, 12);
	presets("B");
	rollers(-100);
	runPath("K");
	runPath("L", true);
	rollers(100);
	pros::delay(300);
	rollers(0);
	presets("A");
	pros::delay(1500);
	presets("A");
	pros::delay(1200);
	runPath("M");
	rollers(50);
	pros::delay(1000);
	presets("B");
}


/**
 * This is a test auton that is not intended to be used during the
 * competition. This is only for testing purposes.
*/
void testAuton() {

}


/**
 * This is the "control" function that picks the auton depending on the
 * given parameter (selected).
*/
void autonSelect(string selected) {
	pros::lcd::set_text(1, selected);

	if (selected == "red") {
		red();
	} else if (selected == "blue") {
		blue();
	} else if (selected == "prog") {
		progSkills();
	} else if (selected == "test") {
		testAuton();
	}
}


/**
 * This function is called by the competition switch, and will call the
 * autonSelect() function to have that select the correct auton.
*/
void autonomous() {
	string SELECTED_AUTON = "blue";
	autonSelect(SELECTED_AUTON);
}


/**
 * This function is called by the competition switch, and will call all
 * of the control functions that are used during the driver control
 * period.
*/
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
