#include "main.h"
#include "Motors.hpp"
#include "Sensors.hpp"
#include "Variables.hpp"
#include "DriveFunctions.hpp"
#include "LiftFunctions.hpp"
#include "RollerFunctions.hpp"
#include "TilterFunctions.hpp"
#include "AssistFunctions.hpp"
#include "Autonomous.hpp"
#include "ControlFunctions.hpp"

using namespace std;


/*
 This code is used at the beginning, when the program starts.
 This function is mainly used to set up motor brakemodes and encoders,
 and then generate paths (2D motion profiling) for auton.
*/
void initialize() {

	pros::c::lcd_initialize();
	rollertrayL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollertrayR.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterL.setBrakeMode(AbstractMotor::brakeMode::hold);

	tilterR.tarePosition();
	tilterL.tarePosition();


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

/*
 Runs the autonomous function for the auton period.
*/
void autonomous() {
	autonPush();
}

/*
 This function is called by the competition switch, and will call all of the
 control functions that are used during the driver control period.
*/
void opcontrol() {
	toggleAssist = true;//turns on tower assist
	//deployed = true;
	//presets("right");
	//Multi threaded driveControl so that the robot can be driven while other code is executed
	pros::Task my_task(driveControl1,(void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "drive");

	//Loop through all Control Function groups to use the robots functionality
	while (true) {
		liftControl();
		rollersControl();
		tilterControl();
		presetControl();

		pros::delay(20);//delay by good convention
	}
}

/*
 Unused methods that are required by PROS. They might be used later.
*/
void disabled() {}
void competition_initialize() {}
void on_center_button() {}
