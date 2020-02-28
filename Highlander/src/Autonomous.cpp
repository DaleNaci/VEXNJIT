#include "main.h"
#include "Variables.hpp"
#include "Functions.hpp"
#include "Motors.hpp"
#include <string>


std::string SELECTED_AUTON = "prog";

void auton_initialize(std::string auton) {
	if (auton == "red") {
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{4.25_ft, 0_ft, 0_deg}
			},
			"1"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{2.63_ft, 0_ft, 0_deg}
			},
			"2"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.0_ft, 0_ft, 0_deg}
			},
			"3"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.75_ft, 0_ft, 0_deg}
			},
			"4"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.85_ft, 0.7_ft, -9.5_deg}
			},
			"5"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.23_ft, 0_ft, 0_deg}
			},
			"6"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.6_ft, 0_ft, 0_deg}
			},
			"7"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.73_ft, 0_ft, 0_deg}
			},
			"8"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.8_ft, 0_ft, 0_deg}
			},
			"9"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{2.6_ft, 0_ft, 0_deg}
			},
			"10"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.4_ft, 0_ft, 0_deg}
			},
			"11"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.8_ft, 0_ft, 0_deg}
			},
			"12"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.5_ft, 0_ft, 0_deg}
			},
			"13"
		);
	} else if (auton == "blue") {
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{4.25_ft, 0_ft, 0_deg}
			},
			"1"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{2.59_ft, 0_ft, 0_deg}
			},
			"2"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.0_ft, 0_ft, 0_deg}
			},
			"3"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.75_ft, 0_ft, 0_deg}
			},
			"4"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.85_ft, 0.7_ft, 40.5_deg}
			},
			"5"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.48_ft, 0_ft, 0_deg}
			},
			"6"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.257_ft, 0_ft, 0_deg}
			},
			"7"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.63_ft, 0_ft, 0_deg}
			},
			"8"
		);
		profileController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.8_ft, 0_ft, 0_deg}
			},
			"9"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{2.5_ft, 0_ft, 0_deg}
			},
			"10"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.3_ft, 0_ft, 0_deg}
			},
			"11"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.3_ft, 0_ft, 0_deg}
			},
			"12"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.5_ft, 0_ft, 0_deg}
			},
			"13"
		);
	} else if (auton == "prog") {
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{3.3_ft, 0_ft, 0_deg}
			},
			"1"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.9_ft, 0_ft, 0_deg}
			},
			"2"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{2.0_ft, 2.15_ft, 0_deg}
			},
			"3"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.0_ft, 0_ft, 0_deg}
			},
			"4"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{3.21_ft, 0_ft, 0_deg}
			},
			"5"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{4.5_ft, 0_ft, 0_deg}
			},
			"6"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.8_ft, 0_ft, 0_deg}
			},
			"7"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.4_ft, 0_ft, 0_deg}
			},
			"8"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{2.2_ft, 0_ft, 0_deg}
			},
			"9"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{2.1_ft, 0_ft, 0_deg}
			},
			"10"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.1_ft, 0_ft, 0_deg}
			},
			"11"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{0.8_ft, 0_ft, 0_deg}
			},
			"12"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.7_ft, 0_ft, 0_deg}
			},
			"13"
		);
		slowController->generatePath(
			{
				{0_ft, 0_ft, 0_deg},
				{1.29_ft, 0_ft, 0_deg}
			},
			"14"
		);

	} else if (auton == "testAuton") {

	}
}

void red() {
	presets("B");
	rollers(-95);
	slowController->setTarget("1");
	slowController->waitUntilSettled();

	pros::delay(1200);
	rollers(0);
	profileController->setTarget("2", true);
	profileController->waitUntilSettled();

	pidTurn(-62);
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

	pros::delay(600);
	rollers(0);
	profileController->setTarget("7", true);
	profileController->waitUntilSettled();

	pidTurn(5);
	pros::delay(200);
	rollers(-100);
	profileController->setTarget("8");
	profileController->waitUntilSettled();

	pros::delay(100);
	profileController->setTarget("9", true);
	profileController->waitUntilSettled();

	pidTurn(180);
	pros::delay(1200);
	slowController->setTarget("10");
	rollersPosition(130, 40);
	slowController->waitUntilSettled();

	presets("Left");
	slowController->setTarget("11");
	slowController->waitUntilSettled();
	while (tilter1.getPosition() > -1090) {
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

	pros::delay(1000);
	slowController->setTarget("12");
	slowController->waitUntilSettled();

	slowController->setTarget("13", true);
	slowController->waitUntilSettled();

	presets("B");
	presets("Left");
	pidTurn(65);
}

void blue() {
	presets("B");
	rollers(-95);
	slowController->setTarget("1");
	slowController->waitUntilSettled();

	pros::delay(1000);
	rollers(0);
	profileController->setTarget("2", true);
	profileController->waitUntilSettled();

	pidTurn(77);
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

	pros::delay(600);
	rollers(0);
	profileController->setTarget("7", true);
	profileController->waitUntilSettled();

	pidTurn(-15);
	pros::delay(200);
	rollers(-100);
	profileController->setTarget("8");
	profileController->waitUntilSettled();

	pros::delay(100);
	profileController->setTarget("9", true);
	profileController->waitUntilSettled();

	pidTurn(-181);
	pros::delay(1200);
	slowController->setTarget("10");
	rollersPosition(140, 100);
	slowController->waitUntilSettled();

	presets("Left");
	slowController->setTarget("11");
	slowController->waitUntilSettled();
	while (tilter1.getPosition() > -1100) {
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

	pros::delay(1000);
	slowController->setTarget("12");
	slowController->waitUntilSettled();

	slowController->setTarget("13", true);
	slowController->waitUntilSettled();

	presets("B");
	presets("Left");
	pidTurn(-65);
}

void progSkills() {
	presets("B");
	rollers(-95);
	slowController->setTarget("1");
	slowController->waitUntilSettled();

	pros::delay(500);
	rollers(0);
	slowController->setTarget("2", true);
	slowController->waitUntilSettled();

	slowController->setTarget("3", true);
	slowController->waitUntilSettled();

	slowController->setTarget("4", true);
	slowController->waitUntilSettled();

	slowController->setTarget("5");
	rollers(-95);
	slowController->waitUntilSettled();

	pros::delay(1000);
	rollers(0);
	pidTurn(129);
	slowController->setTarget("6");
	rollersPosition(-500, 100);
	slowController->waitUntilSettled();

	presets("Left");
	rollersPosition(170, 100);
	slowController->setTarget("7");
	slowController->waitUntilSettled();

	while (tilter1.getPosition() > -1090) {
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
	slowController->setTarget("8");
	slowController->waitUntilSettled();

	pros::delay(500);
	slowController->setTarget("9", true);
	slowController->waitUntilSettled();

	pros::delay(100);
	presets("B");
	pidTurn(-143);
	slowController->setTarget("10");
	rollers(-95);
	slowController->waitUntilSettled();

	pros::delay(1500);
	rollersPosition(115, 100);
	pros::delay(200);
	slowController->setTarget("11", true);
	slowController->waitUntilSettled();

	presets("X");
	while (tilter1.getPosition() >= -240) {
		continue;
	}
	presets("X");
	slowController->setTarget("12");
	slowController->waitUntilSettled();

	rollersPosition(600, 100);
	pros::delay(1000);
	slowController->setTarget("13", true);
	slowController->waitUntilSettled();

	presets("B");
	pidTurn(-99);
	slowController->setTarget("14");
	rollers(-95);
	slowController->waitUntilSettled();

	pros::delay(1500);
	rollersPosition(115, 100);
	pros::delay(200);
	slowController->setTarget("11", true);
	slowController->waitUntilSettled();

	presets("A");
	while (tilter1.getPosition() >= -240) {
		continue;
	}
	presets("A");
	slowController->setTarget("12");
	slowController->waitUntilSettled();

	rollersPosition(600, 100);
	pros::delay(2000);
	slowController->setTarget("13", true);
	presets("B");
	slowController->waitUntilSettled();
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
