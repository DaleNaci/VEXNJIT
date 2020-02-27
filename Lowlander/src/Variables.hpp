#include "main.h"

#ifndef VARIABLES_HPP
#define VARIABLES_HPP

//pros controller
extern pros::Controller master;

/**
 * These are the variables that are used to configure controller inputs
 * to perform various tasks for the robot.
*/
extern Controller joystick;

extern ControllerButton intakeIn;
extern ControllerButton intakeOut;
extern ControllerButton trayDown;
extern ControllerButton trayUp;

extern ControllerButton liftUp;
extern ControllerButton liftDown;
extern ControllerButton toggleAuto;
extern ControllerButton presetRight;

extern ControllerButton presetA;
extern ControllerButton presetX;
extern ControllerButton presetB;
extern ControllerButton presetY;

//Structures
struct rollers{
	double outerRMean;
	double outerLMean;
	double outerRSD;
	double outerLSD;
	double innerRMean;
	double innerLMean;
	double innerRSD;
	double innerLSD;
};

//Global boolean variables
extern bool deployed; //Tells the deploy function if the robot has deployed yet. This helps ensure that it is not accidently used a second time during a match
extern bool armUp; //Is the arm up (used to determine whether or not to activate auto tower function)
extern bool toggleAssist; //Auto Tower Assist feature starts on

extern int bPresetPos;//number of degrees / 5 to set the lift arm above the zero position when the bPreset is pressed

/*
 These are the chassis variables that are used for the driver control
 period and the autonomous period.
*/
extern std::shared_ptr<okapi::ChassisController> chassis;
extern std::shared_ptr<okapi::AsyncMotionProfileController> profileController;
extern std::shared_ptr<okapi::AsyncMotionProfileController> slowController;


#endif
