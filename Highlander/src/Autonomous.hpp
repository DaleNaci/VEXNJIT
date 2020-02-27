#include "main.h"
#include <string>

#ifndef AUTONOMOUS_HPP
#define AUTONOMOUS_HPP


extern std::string SELECTED_AUTON;

void auton_initialize(std::string auton);

/**
 * Calling this function runs the red auton.
*/
void red();

/**
 * Calling this function runs the blue auton.
*/
void blue();

/**
 * Calling this function runs programming skills.
*/
void progSkills();

/**
 * This is a test auton that is not intended to be used during the
 * competition. This is only for testing purposes.
*/
void testAuton();

/**
 * This will call the auton depending on the "selected" parameter.
*/
void autonSelect(std::string selected);


#endif
