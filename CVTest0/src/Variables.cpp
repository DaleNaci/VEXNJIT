#include "main.h"
#include "Variables.hpp"
#include "Motors.hpp"


//pros controller
pros::Controller master(pros::E_CONTROLLER_MASTER);
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
ControllerButton toggleAuto(ControllerDigital::left);
ControllerButton presetRight(ControllerDigital::right);

ControllerButton presetA(ControllerDigital::A);
ControllerButton presetX(ControllerDigital::X);
ControllerButton presetB(ControllerDigital::B);
ControllerButton presetY(ControllerDigital::Y);



//Global boolean variables
bool deployed = false; //Tells the deploy function if the robot has deployed yet. This helps ensure that it is not accidently used a second time during a match
bool armUp = false; //Is the arm up (used to determine whether or not to activate auto tower function)
bool toggleAssist = true; //Auto Tower Assist feature starts on

int bPresetPos = 75;//number of degrees / 5 to set the lift arm above the zero position when the bPreset is pressed

/*
 These are the chassis variables that are used for the driver control
 period and the autonomous period.
*/
std::shared_ptr<okapi::ChassisController> chassis = ChassisControllerBuilder()
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

std::shared_ptr<okapi::AsyncMotionProfileController> profileController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.3,
			1.4,
			6.0
		}
	).withOutput(
		chassis
	).buildMotionProfileController();
