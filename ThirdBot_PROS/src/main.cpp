#include "main.h"

using namespace std;

/**
 * These are the variable constants for the ports.
*/
//Tray Lift
int8_t TILTER_LEFT_PORT = 15;
int8_t TILTER_RIGHT_PORT = 16;

//Arm Lift
int8_t LEFT_LIFT_PORT = 2;
int8_t RIGHT_LIFT_PORT = 3;

//Rollers
int8_t ARM_LEFT_ROLLER_PORT = 10;
int8_t ARM_RIGHT_ROLLER_PORT = 1;
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

Motor rollerarmL(ARM_LEFT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rollerarmR(ARM_RIGHT_LIFT_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor rollertrayL(TRAY_LEFT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rollertrayR(TRAY_RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor driveFL(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBL(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveFR(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBR(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

ADIDigitalIn armLStop(ARM_LEFT_STOP_SWITCH_PORT);
ADIDigitalIn armRStop(ARM_RIGHT_STOP_SWITCH_PORT);

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
ControllerButton liftOverride(ControllerDigital::A);

ControllerButton presetA(ControllerDigital::Y);

bool areArmsResetting = true;

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

bool areArmsAtStops() {
	return armLStop.getValue() || armRStop.getValue();
}

bool isLiftOverrideActive(bool isAuton) {
	if(isAuton) {
		return false;
	} else {
		return liftOverride.isPressed();
	}
}

/**
 * Move the lift up or down, depending on the state of the up and down
 * buttons. If the up button is pressed, the tray will move upwards. If
 * the down button is pressed, the tray will move downwards. The up
 * button has priority.
*/
void liftControl() {
	int speed = 0;	
	if (liftUp.isPressed()) {
		speed = 90;
	} else if (liftDown.isPressed()) {
		speed = -90;
		if (liftL.getPosition() < 0) {
			liftL.tarePosition();
			liftR.tarePosition();
		}
	}
	
	lift(speed, false);
}

void lift(int speed, bool isAuton) {
	bool overrideCtrl = isLiftOverrideActive(isAuton);
	
	if(areArmsAtStops() && !overrideCtrl) {
		speed = 0;
		areArmsResetting = false;
	}
	
	if(overrideCtrl) {
		areArmsResetting = false;
	}
	
	if(areArmsResetting && !overrideCtrl) {
		speed = -30;
	}
	
	liftRaw(speed);
}

/**
 * Moves the roller lift. Speed will depend on the speed parameter. The
 * range is -100 to 100.
*/
void liftRaw(int speed) {
	armL.moveVelocity(speed * 2);
	armR.moveVelocity(-speed * 2);
}

void liftPos(int speed, int pos) {
    armL.moveAbsolute(pos, speed);
    armR.moveAbsolute(-pos, speed);
}

void liftPreset(int pos) {
	liftPos(80, pos);
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
	rollerarmL.moveVelocity(speed);
	rollerarmR.moveVelocity(-speed);
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

int getLeftArmPos() {
	return rollerarmL.getPosition();
}

/**
 * Moves the rollers to intake and outtake depending on the state of
 * the up and down buttons. If the up button is pressed, the
 * manipulator will intake, and if the down button is pressed, the
 * manipulator will outtake. The up button has priority.
*/
void rollersControl() {
	int speed = 0;
	if(intakeIn.isPressed()) {
		speed = 100;
	} else if(intakeOut.isPressed()) {
		speed = -100;
	}
	
	int armPos = getLeftArmPos();
	/*bool down = armPos < (ARM_POS_DOWN + ARM_MARGIN_OF_ERROR);
	bool up = armPos > (ARM_POS_UP - ARM_MARGIN_OF_ERROR) && armPos < (ARM_POS_UP + ARM_MARGIN_OF_ERROR);
	bool back = armPos > (ARM_POS_BACK - ARM_MARGIN_OF_ERROR) && armPos < (ARM_POS_BACK + ARM_MARGIN_OF_ERROR);
	bool betweenUpAndBack = armPos > (ARM_POS_BACK + ARM_MARGIN_OF_ERROR * 2) && armPos < (ARM_POS_BACK - ARM_MARGIN_OF_ERROR * 2);*/
	bool down = false, up = true, back = false, betweenUpAndBack = false;
	
	int speedArms = 0;
	int speedTray = 0;
	
	if(down || up) {
		speedArms = speed;
	}
	
	if(betweenUpAndBack) {
		speedArms = armL.getVelocity();
	}
	
	if(down || back) {
		speedTray = speed;
	}
	
	rollersArms(speedArms);
	rollersTray(speedTray);
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

void waitForArmReset() {
	while(areArmsResetting) {
		pros::delay(20);
	}
}

void deployTray(bool isAuton) {
	if(areArmsAtStops()) {
		
	}
}

/**
 * Runs the autonomous function for the auton period.
*/
void autonomous() {
	waitForArmReset();
	deployTray(true);
	
	
	/*runPath("1", true);
	runPath("2");*/
}

/**
 * This function is called by the competition switch, and will call all
 * of the control functions that are used during the driver control
 * period.
*/
void opcontrol() {
	while (true) {
		liftControl();
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
