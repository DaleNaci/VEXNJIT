#include "main.h"

using namespace std;

/**
 * These are the variable constants for the ports.
*/
//Tray Lift
int8_t TILTER_LEFT_PORT = 15;
int8_t TILTER_RIGHT_PORT = 18; //formerly 16 which is now dead

//Arm Lift
int8_t LEFT_LIFT_PORT = 4;
int8_t RIGHT_LIFT_PORT = 3;

//Rollers
int8_t ARM_LEFT_ROLLER_PORT = 10;
int8_t ARM_RIGHT_ROLLER_PORT = 5;
int8_t TRAY_LEFT_ROLLER_PORT = 8;
int8_t TRAY_RIGHT_ROLLER_PORT = 7;

//Drive Wheels
int8_t LEFT_FRONT_DRIVE_PORT = 9;
int8_t LEFT_BACK_DRIVE_PORT = 14;
int8_t RIGHT_FRONT_DRIVE_PORT = 6;
int8_t RIGHT_BACK_DRIVE_PORT = 13;

int8_t ARM_LEFT_STOP_SWITCH_PORT = 8;
int8_t ARM_RIGHT_STOP_SWITCH_PORT = 7;


/**
 * These are the different motor variables that are used to move
 * different parts of the robot.
*/
Motor tilterL(TILTER_LEFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor tilterR(TILTER_RIGHT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

Motor armL(LEFT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor armR(RIGHT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

Motor rollerarmL(ARM_LEFT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rollerarmR(ARM_RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor rollertrayL(TRAY_LEFT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rollertrayR(TRAY_RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor driveFL(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBL(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveFR(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBR(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

ADIButton armLStop(ARM_LEFT_STOP_SWITCH_PORT);
ADIButton armRStop(ARM_RIGHT_STOP_SWITCH_PORT);

/**
 * These are the variables that are used to configure controller inputs
 * to perform various tasks for the robot.
*/
Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);
ControllerButton trayDown(ControllerDigital::R2);
ControllerButton trayUp(ControllerDigital::R1);

ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);

ControllerButton presetA(ControllerDigital::A);
ControllerButton presetX(ControllerDigital::X);
ControllerButton presetB(ControllerDigital::B);
ControllerButton presetY(ControllerDigital::Y);
ControllerButton presetRight(ControllerDigital::right);
ControllerButton toggleAuto(ControllerDigital::left);

bool areArmsResetting = false;
bool toggleAssist = true;

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
	rollertrayL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollertrayR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterL.setBrakeMode(AbstractMotor::brakeMode::hold);
	// armL.setBrakeMode(AbstractMotor::brakeMode::hold);
	// armR.setBrakeMode(AbstractMotor::brakeMode::hold);

	tilterR.tarePosition();
	tilterL.tarePosition();

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
 * Moves the roller lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void lift(int speed) {
	armL.moveVelocity(speed * 2);
	armR.moveVelocity(-speed * 2);
}


/**
 * Move the lift up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void liftControl() {
	if (liftUp.changedToPressed()) {
		lift(60);
	} else if (liftDown.changedToPressed()) {
		lift(-60);
		if (armL.getPosition() < 0) {
			armL.tarePosition();
			armR.tarePosition();
		}
	}
	if (liftUp.changedToReleased() || liftDown.changedToReleased()) {
		lift(0);
	}
}

void liftPosition(int pos, int speed) {
    armL.moveAbsolute(pos, speed);
    armR.moveAbsolute(-pos, speed);
}


/**
 * Moves both tray roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollersTray(int speed) {
	rollertrayL.moveVelocity(speed);
	rollertrayR.moveVelocity(-speed);
}


/**
 * Moves both arm roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollersArms(int speed) {
	rollerarmL.moveVelocity(-speed);
	rollerarmR.moveVelocity(speed);
}


/**
 * Moves the tilter. Speed will depend on the speed parameter. The range
 * is -100 to 100. If speed is 0, the motor will stop with a brakeType
 * of "hold."
*/
void tilter(int speed) {
	tilterR.moveVelocity(speed);
	tilterL.moveVelocity(-speed);
}


/**
 * Moves the tilter to a specific absolute position. Position will
 * depend on the pos parameter. Speed will depend on the speed
 * parameter.
*/
void tilterPosition(int pos, int speed) {
	tilterR.moveAbsolute(pos, speed);
	tilterL.moveAbsolute(-pos, speed);
}


/**
 * Moves the rollers to intake and outtake depending on the state of
 * the up and down buttons. If the up button is pressed, the
 * manipulator will intake, and if the down button is pressed, the
 * manipulator will outtake. The up button has priority.
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
			rollersArms(100);
			rollersTray(100);
		} else if (intakeOut.isPressed()) {
			rollersArms(-100);
			rollersTray(-100);
		} else {
			rollersArms(0);
			rollersTray(0);
		}
	}
}

void rollersTrayDegrees(int speed, int degrees) {
	int	ticksPerRevoultion = 1800; // ticks per revoultion of a red insert motor
	int ticks = floor(degrees / 360 * ticksPerRevoultion); //compute number of ticks to rotate
	rollertrayL.moveRelative(ticks, speed);
	rollertrayR.moveRelative(ticks, -speed);
}


/**
 * Moves both arm roller motors. Speed will depend on the speed parameter.
 * The range is -100 to 100.
*/
void rollersArmsDegrees(int speed, int degrees) {
	int	ticksPerRevoultion = 900; // ticks per revoultion of a green insert motor
	int ticks = floor(degrees / 360 * ticksPerRevoultion); //compute number of ticks to rotate
	rollerarmL.moveRelative(ticks, -speed);
	rollerarmR.moveRelative(ticks, speed);

}

//Function to move both sets of rollers a specified number of degrees
void rollersDegrees(int speed, int degrees)
{
	rollersTrayDegrees(speed, -degrees);
	rollersArmsDegrees(speed, -degrees);

	while (rollerarmL.getPosition() != rollerarmL.getTargetPosition() - 1)
	{
		// Continue running this loop as long as the motor is not at its goal position
		pros::delay(2);
	}

/*automatically loads cubes into the lift arms for scoring towerAssist*/
void towerAssist(bool toggle)
{
	if(toggleAssist) //if the assist is on then run the following
	{
		// bring up cubes first
		rollersDegrees(100, -360);
		pros::delay(200);
		//push 1 cube down
		rollersDegrees(-100, 270);
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
		if (tilterL.getTargetVelocity() != 100) {
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


void deployTray() {
	rollersArms(-40);
	pros::delay(500);
	liftPosition(280, 30);
	while (armL.getPosition() < 279) {
		continue;
	}
	rollersArms(0);
	lift(-30);
	// pros::delay(300);
	// lift(0);
	while (true) {
		if (armLStop.changedToPressed() || armRStop.changedToPressed()) {
			armL.moveVelocity(0);
			armL.tarePosition();

			armR.moveVelocity(0);
			armR.tarePosition();

			pros::delay(500);

			armL.setBrakeMode(AbstractMotor::brakeMode::hold);
			armR.setBrakeMode(AbstractMotor::brakeMode::hold);

			break;
		}
	}

	tilterPosition(-100, 90);
	while (tilterR.getPosition() > -99) {
		continue;
	}
	tilterPosition(0, 90);
	// waitForArmReset();
	liftPosition(550, 40);
	while (armL.getPosition() < 549) {
		continue;
	}
	liftPosition(0, 40);
}


/**
 * Moves different mechanisms to certain positions based on the
 * parameter, preset. Each preset will call different functions.
*/
void presets(string preset) {
	if (preset == "B") {
		if (tilterR.getPosition() < -10) {
			tilterPosition(0, 100);
		} else {
			liftPosition(0, 100);
		}
	}
	if (preset == "X") {
		towerAssist(toggleAssist);
		liftPosition(445, 100);
	}
	if (preset == "A") {
		towerAssist(toggleAssist);
		liftPosition(350, 100);

	}
	if (preset == "Y") {
		towerAssist(toggleAssist);
		liftPosition(600, 90);
	}
	if (preset == "right") {
		deployTray();
	}
	if (preset == "left") {
		toggleAssist = !toggleAssist;//Toggles the cube tower scoring assist feature
		 /* pros::c::controller_rumble(, ". - . -");
		if(toggleAssist)
		{

		}
		else
		{

		}
*/
}
}

/**
 * Moves the lift to a specific height, depending on the button pressed.
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
	if (toggleAuto.isPressed()) {
	 presets("left");
 }
	// if (presetRight.isPressed()) {
	// 	presets("right");
	// }
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

void waitForArmReset() {
	// while(areArmsResetting) {
	// 	pros::delay(20);
	// }

	while (true) {
		bool l = armLStop.isPressed();
		bool r = armRStop.isPressed();

		if (l) {
			armL.moveVelocity(0);
		}
		if (r) {
			armR.moveVelocity(0);
		}

		if(l && r) {
			break;
		}
	}
}

/*void deployTray() {
	rollersArms(-40);
	liftPos(220, 30);
	while (armL.getPosition() < 200) {
		continue;
	}
	pros::delay(1000);
	rollersArms(0);
	//lift(0);
	pros::delay(5000);
	lift(-30);
	pros::delay(200);
	lift(0);

//	waitForArmReset();`
}
*/
/**
 * Runs the autonomous function for the auton period.
*/
void autonomous() {
	runPath("1", true);
	runPath("2");

	deployTray();
	armL.tarePosition();
	armR.tarePosition();
}

/**
 * This function is called by the competition switch, and will call all
 * of the control functions that are used during the driver control
 * period.
*/
void opcontrol() {
	armL.tarePosition();
	armR.tarePosition();
	armL.setBrakeMode(AbstractMotor::brakeMode::hold);
	armR.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmR.setBrakeMode(AbstractMotor::brakeMode::hold);

	while (true) {
		liftControl();
		rollersControl();
		tilterControl();
		driveControl();
		presetControl();

		pros::delay(20);
	}
}


/**
 * Unused methods that are required by PROS. They might be used later.
*/
void disabled() {}
void competition_initialize() {}
void on_center_button() {}
