#include "main.h"
#include "Variables.hpp"
#include "Functions.hpp"
#include "Motors.hpp"
#include <string>


void red() {

}

void blue() {
	presets("B");
	rollers(-95);
	slowController->setTarget("1");
	slowController->waitUntilSettled();

	pros::delay(500);
	rollers(0);
	profileController->setTarget("2", true);
	profileController->waitUntilSettled();

	pidTurn(90);
	pros::delay(300);
	profileController->setTarget("3");
	profileController->waitUntilSettled();

	rollers(-100);
	slowController->setTarget("4");
	slowController->waitUntilSettled();

	pros::delay(400);
	rollers(0);
	profileController->setTarget("5", true, true);
	profileController->waitUntilSettled();

	pros::delay(200);
	rollers(-100);
	slowController->setTarget("6");
	slowController->waitUntilSettled();

	pros::delay(200);
	rollers(0);
	profileController->setTarget("7", true);
	profileController->waitUntilSettled();

	pidTurn(-14);
	pros::delay(200);
	rollers(-100);
	profileController->setTarget("8");
	profileController->waitUntilSettled();

	pros::delay(100);
	profileController->setTarget("9", true);
	rollersPosition(-500, 100);
	profileController->waitUntilSettled();

	pidTurn(-173);
	pros::delay(200);
	slowController->setTarget("10");
	rollersPosition(170, 40);
	slowController->waitUntilSettled();

	presets("Left");
	slowController->setTarget("11");
	slowController->waitUntilSettled();
	while (tilter1.getPosition() > -1000) {
		if (tilter1.getPosition() > -250) {
			tilter(-100);
		} else {
			int vel = 50 + (450 + (tilter1.getPosition() + 250)) * 0.1111;
			if (vel < 50) {
				vel = 50;
			}
			tilter(-vel);
		}
	}
	tilter(0);
	pros::delay(800);
	slowController->setTarget("12");
	slowController->waitUntilSettled();

	slowController->setTarget("12");
	slowController->waitUntilSettled();

	slowController->setTarget("13", true);
	slowController->waitUntilSettled();

	presets("B");
	presets("Left");
	pidTurn(-56);


}

void progSkills() {

}

void testAuton() {
	pidTurn(360);
}

void autonSelect(std::string selected) {
	if (selected == "red") {
		red();
	} else if (selected == "blue") {
		blue();
	} else if (selected == "prog") {
		progSkills();
	} else if (selected == "test") {
		testAuton();
	}
}
