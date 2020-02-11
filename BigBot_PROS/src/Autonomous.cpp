#include "main.h"
#include "Variables.hpp"
#include "Functions.hpp"
#include <string>


void red() {

}

void blue() {
	presets("B");
	rollers(-95);
	slowController->setTarget("1");
	slowController->waitUntilSettled();

	pros::delay(400);
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

	for (int i = 0; i < 2; i++) {
		profileController->setTarget("5", true);
		profileController->waitUntilSettled();

		pidTurn(-30 + (7 * i));
		pros::delay(200);
		rollers(-100);
		profileController->setTarget("6");
		profileController->waitUntilSettled();

		pros::delay(200);
		rollers(0);
	}

	pidTurn(-155);
	pros::delay(200);
	profileController->setTarget("7");
	profileController->waitUntilSettled();


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
