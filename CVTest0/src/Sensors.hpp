#include "main.h"

#ifndef SENSORS_HPP
#define SENSORS_HPP

//Sensor Declarartion
//Button stops
extern ADIButton armLStop;
extern ADIButton armRStop;
//Shaft Encoders
extern ADIEncoder armLEncoder;
extern ADIEncoder armREncoder;//reversed, might have to switch which one is reversed
//Ultrasonic
extern pros::ADIUltrasonic UltraSensor;

#endif
