#include "main.h"

#ifndef CONTROLFUNCTIONS_HPP
#define CONTROLFUNCTIONS_HPP
/*
 This runs the drive on an arcade control setup. The left vertical
 axis moves the drive up and down, while the right horizontal axis
 point the drive left and right.
*/
void driveControl();

/*
 Task runnable version of driveControl
*/
void driveControl1(void* param);

/*
 Moves the lift up or down, depending on the state of the up and down
 buttons(D pad). If the up button is pressed, the lift will move upwards. If
 the down button is pressed, the lift will move downwards. The up
 button has priority.
*/
void liftControl();

/*
 Moves the rollers to intake and outtake depending on the state of
 the up and down buttons. If the up button is pressed, the
 manipulator will intake, and if the down button is pressed, the
 manipulator will outtake. The up button has priority.
*/
void rollersControl();

/*
 Move the tray up or down, depending on the state of the up and down
 buttons. If the up button is pressed, the tray will move upwards. If
 the down button is pressed, the tray will move downwards. The up
 button has priority. When the tray is moving up, the closer it is to
 its resting position, the faster it will be. Once it reaches a
 certain point in its range of motion, it is capped at a maximum
 velocity.
*/
void tilterControl();

/*
 Moves the lift to a specific height, depending on the button pressed.
*/
void presetControl();

#endif
