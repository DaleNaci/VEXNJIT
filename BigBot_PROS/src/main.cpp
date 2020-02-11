#include "main.h"
#include "Motors.hpp"
#include "Sensors.hpp"
#include "Variables.hpp"
#include "Functions.hpp"

using namespace std;


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
	driveR1.tarePosition();
	driveR2.tarePosition();
	driveR3.tarePosition();
	driveR4.tarePosition();
	driveL1.tarePosition();
	driveL2.tarePosition();
	driveL3.tarePosition();
	driveL4.tarePosition();

	encoderL.reset();
	encoderR.reset();

	pros::lcd::initialize();

	pros::delay(300);

	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{4.25_ft, 0_ft, 0_deg}
		},
		"1"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.63_ft, 0_ft, 0_deg}
		},
		"2"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.0_ft, 0_ft, 0_deg}
		},
		"3"
	);
	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.7_ft, 0_ft, 0_deg}
		},
		"4"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.8_ft, 0.7_ft, 43.5_deg}
		},
		"5"
	);
	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.48_ft, 0_ft, 0_deg}
		},
		"6"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.257_ft, 0_ft, 0_deg}
		},
		"7"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.63_ft, 0_ft, 0_deg}
		},
		"8"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.8_ft, 0_ft, 0_deg}
		},
		"9"
	);
	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.5_ft, 0_ft, 0_deg}
		},
		"10"
	);
	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.3_ft, 0_ft, 0_deg}
		},
		"11"
	);
	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.3_ft, 0_ft, 0_deg}
		},
		"12"
	);
	slowController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1.5_ft, 0_ft, 0_deg}
		},
		"13"
	);
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
