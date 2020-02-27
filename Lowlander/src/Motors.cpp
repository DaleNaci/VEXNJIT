#include "main.h"
#include "Ports.cpp"
#include "Motors.hpp"

/**
 * These are the different motor variables that are used to move
 * different parts of the robot.
*/
Motor tilterL(TILTER_LEFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor tilterR(TILTER_RIGHT_PORT, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);//Reversed

Motor armL(LEFT_LIFT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor armR(RIGHT_LIFT_PORT, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);//Reversed

Motor rollerarmL(ARM_LEFT_ROLLER_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);//Reversed
Motor rollerarmR(ARM_RIGHT_ROLLER_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor rollertrayL(TRAY_LEFT_ROLLER_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor rollertrayR(TRAY_RIGHT_ROLLER_PORT, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);//Reversed

Motor driveFL(LEFT_FRONT_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBL(LEFT_BACK_DRIVE_PORT, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveFR(RIGHT_FRONT_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBR(RIGHT_BACK_DRIVE_PORT, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
