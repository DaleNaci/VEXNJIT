#include "main.h"
#include "Variables.hpp"
#include "Motors.hpp"


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

std::shared_ptr<okapi::ChassisController> chassis = ChassisControllerBuilder()
	.withMotors(
		{driveL1, driveL2, driveL3, driveL4},
		{driveR1, driveR2, driveR3, driveR4}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{6.34_in, 14.1_in},
			static_cast<int32_t>(imev5GreenTPR * 2.0)
		}
	).build();

std::shared_ptr<okapi::AsyncMotionProfileController> profileController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			1.268,
			4.5,
			5.155
		}
	).withOutput(
		chassis
	).buildMotionProfileController();

std::shared_ptr<okapi::AsyncMotionProfileController> slowController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.265,
			0.9,
			5.21
		}
	).withOutput(
		chassis
	).buildMotionProfileController();
