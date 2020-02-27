#include "main.h"
#include "Ports.cpp"
#include "Sensors.hpp"

//Sensor Declarartion
//Button stops
ADIButton armLStop(ARM_LEFT_STOP_SWITCH_PORT);
ADIButton armRStop(ARM_RIGHT_STOP_SWITCH_PORT);
//Shaft Encoders
ADIEncoder armLEncoder(ARM_LEFT_SHAFT_ENCODER_TOP_PORT, ARM_LEFT_SHAFT_ENCODER_BOTTOM_PORT, false);
ADIEncoder armREncoder(ARM_RIGHT_SHAFT_ENCODER_TOP_PORT, ARM_RIGHT_SHAFT_ENCODER_BOTTOM_PORT, true);//reversed, might have to switch which one is reversed
//Ultrasonic
pros::ADIUltrasonic UltraSensor(ULTRASONIC_PING_PORT, ULTRASONIC_ECHO_PORT);
