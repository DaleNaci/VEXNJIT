#include "main.h"

/**
 * These are the variable constants for the ports.
*/
//Tray Lift
const int8_t TILTER_LEFT_PORT = 15;
const int8_t TILTER_RIGHT_PORT = 18; //formerly 16 which is now dead

//Arm Lift
const int8_t LEFT_LIFT_PORT = 4;
const int8_t RIGHT_LIFT_PORT = 3;

//Rollers
const int8_t ARM_LEFT_ROLLER_PORT = 21;//formerly 10 which is now dead
const int8_t ARM_RIGHT_ROLLER_PORT = 5;
const int8_t TRAY_LEFT_ROLLER_PORT = 8;
const int8_t TRAY_RIGHT_ROLLER_PORT = 7;

//Drive Wheels
const int8_t LEFT_FRONT_DRIVE_PORT = 9;
const int8_t LEFT_BACK_DRIVE_PORT = 14;
const int8_t RIGHT_FRONT_DRIVE_PORT = 6;
const int8_t RIGHT_BACK_DRIVE_PORT = 13;

//Arm Stop Buttons
const int8_t ARM_LEFT_STOP_SWITCH_PORT = 1;
const int8_t ARM_RIGHT_STOP_SWITCH_PORT = 2;

//Shaft encoders
//Left encoder
const int8_t ARM_LEFT_SHAFT_ENCODER_TOP_PORT = 3;
const int8_t ARM_LEFT_SHAFT_ENCODER_BOTTOM_PORT = ARM_LEFT_SHAFT_ENCODER_TOP_PORT + 1;
//right encoder
const int8_t ARM_RIGHT_SHAFT_ENCODER_TOP_PORT = 5;
const int8_t ARM_RIGHT_SHAFT_ENCODER_BOTTOM_PORT = ARM_RIGHT_SHAFT_ENCODER_TOP_PORT + 1;

//Ultrasonic
const int8_t ULTRASONIC_PING_PORT = 7;
const int8_t ULTRASONIC_ECHO_PORT = ULTRASONIC_PING_PORT + 1;
