#include "main.h"
#include "Motors.hpp"


Motor rollerL(16, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor rollerR(15, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

Motor liftL(18, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor liftR(13, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

Motor tilter1(17, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

Motor driveR1(14, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveR2(11, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveR3(1, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveR4(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveL1(19, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveL2(20, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveL3(10, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveL4(9, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
