#include "main.h"

#ifndef VARIABLES_HPP
#define VARIABLES_HPP


extern Controller joystick;

extern ControllerButton intakeIn;
extern ControllerButton intakeOut;

extern ControllerButton liftUp;
extern ControllerButton liftDown;

extern ControllerButton trayUp;
extern ControllerButton trayDown;

extern ControllerButton presetX;
extern ControllerButton presetA;
extern ControllerButton presetB;
extern ControllerButton presetY;
extern ControllerButton presetLeft;

extern std::shared_ptr<okapi::ChassisController> chassis;
extern std::shared_ptr<okapi::AsyncMotionProfileController> profileController;
extern std::shared_ptr<okapi::AsyncMotionProfileController> slowController;


#endif
