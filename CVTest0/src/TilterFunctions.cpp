#include "main.h"
#include "Motors.hpp"
#include "TilterFunctions.hpp"

/*
 Moves the tilter. Speed will depend on the speed parameter. Positive speed
 means the tilter(tray) will move up(stacking position) and negative speed means
 the tilter(tray) will move down(intake position). The range is -100 to 100. If
 speed is 0, the motor will stop with a brakeType of "hold."
*/
void tilter(int speed) {
	tilterR.moveVelocity(speed);
	tilterL.moveVelocity(speed);
}

/*
 Moves the tilter(tray) a specified number of degrees(absoulte from starting
 position), speed is absolute and positive degrees specifies up(stacking
 position) while negative degrees specifies the tilter(tray) will move down
 (intake position). Speed range is +100 to -100, degrees range is any int value
*/
void tilterPosition(int degrees, int speed) {
	tilterR.moveAbsolute(degrees, speed);
	tilterL.moveAbsolute(degrees, speed);
}
