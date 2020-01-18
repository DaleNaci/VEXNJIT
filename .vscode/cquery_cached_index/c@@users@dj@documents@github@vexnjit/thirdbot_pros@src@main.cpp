#include "main.h"

using namespace std;

int8_t LEFT_ROLLER_PORT = 14;
int8_t RIGHT_ROLLER_PORT = 10;
int8_t TILTER_1_PORT = 12;
int8_t TILTER_2_PORT = 18;
int8_t LEFT_FRONT_DRIVE_PORT = 17;
int8_t LEFT_BACK_DRIVE_PORT = 13;
int8_t RIGHT_FRONT_DRIVE_PORT = 15;
int8_t RIGHT_BACK_DRIVE_PORT = 16;

Motor rollerL(LEFT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor rollerR(RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor tilter1(TILTER_1_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor tilter2(TILTER_2_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor driveFL(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBL(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveFR(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBR(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);
ControllerButton trayDown(ControllerDigital::R2);
ControllerButton trayUp(ControllerDigital::R1);


auto chassis = ChassisControllerBuilder()
	.withMotors(
		{driveFL, driveBL},
		{driveFR, driveBR}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{6.1_in, 12.5_in},
			static_cast<int32_t>(imev5GreenTPR * 2.0)
		}
	).build();

auto timer = TimeUtilFactory::createDefault().getTimer();


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


void initialize() {
	rollerL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilter1.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilter2.setBrakeMode(AbstractMotor::brakeMode::hold);
}


void rollers(int speed) {
	rollerL.moveVelocity(speed);
	rollerR.moveVelocity(-speed);
}


void tilter(int speed) {
	tilter1.moveVelocity(speed);
	tilter2.moveVelocity(-speed);
}


void tilterPosition(int pos, int speed) {
	tilter1.moveAbsolute(-pos, speed);
	tilter2.moveAbsolute(pos, speed);
}


void rollersControl() {
	if (intakeIn.isPressed()) {
		rollers(100);
	} else if (intakeOut.isPressed()) {
		rollers(-100);
	} else {
		rollers(0);
	}
}


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

	if (trayDown.changedToPressed()) {
		auto timeChanged = timer->getDtFromMark();

		if (0_ms < timeChanged && timeChanged < 1000_ms) {
			tilterPosition(0, 100);
		} else {
			timer->placeMark();
		}
	}
}


void driveControl() {
	chassis->getModel()->arcade(
		joystick.getAnalog(ControllerAnalog::leftY),
		joystick.getAnalog(ControllerAnalog::rightX)
	);
}


void autonomous() {}


void opcontrol() {
	while (true) {
		rollersControl();
		tilterControl();
		driveControl();

		pros::delay(20);
	}
}



void disabled() {}
void competition_initialize() {}
void on_center_button() {}
