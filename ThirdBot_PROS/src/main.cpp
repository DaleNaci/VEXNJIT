#include "main.h"

using namespace std;

/**
 * These are the variable constants for the ports.
*/
//Tray Lift
int8_t TILTER_LEFT_PORT = 15;
int8_t TILTER_RIGHT_PORT = 18; //formerly 16 which is now dead

//Arm Lift
int8_t LEFT_LIFT_PORT = 4;
int8_t RIGHT_LIFT_PORT = 3;

//Rollers
int8_t ARM_LEFT_ROLLER_PORT = 10;
int8_t ARM_RIGHT_ROLLER_PORT = 5;
int8_t TRAY_LEFT_ROLLER_PORT = 8;
int8_t TRAY_RIGHT_ROLLER_PORT = 7;

//Drive Wheels
int8_t LEFT_FRONT_DRIVE_PORT = 9;
int8_t LEFT_BACK_DRIVE_PORT = 14;
int8_t RIGHT_FRONT_DRIVE_PORT = 6;
int8_t RIGHT_BACK_DRIVE_PORT = 13;

//Arm Stop Buttons
int8_t ARM_LEFT_STOP_SWITCH_PORT = 8;
int8_t ARM_RIGHT_STOP_SWITCH_PORT = 7;


/**
 * These are the different motor variables that are used to move
 * different parts of the robot.
*/
Motor tilterL(TILTER_LEFT_PORT, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);//Reversed
Motor tilterR(TILTER_RIGHT_PORT, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

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

ADIButton armLStop(ARM_LEFT_STOP_SWITCH_PORT);
ADIButton armRStop(ARM_RIGHT_STOP_SWITCH_PORT);

/**
 * These are the variables that are used to configure controller inputs
 * to perform various tasks for the robot.
*/
Controller joystick;

ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);
ControllerButton trayDown(ControllerDigital::R2);
ControllerButton trayUp(ControllerDigital::R1);

ControllerButton liftUp(ControllerDigital::up);
ControllerButton liftDown(ControllerDigital::down);
ControllerButton toggleAuto(ControllerDigital::left);
ControllerButton presetRight(ControllerDigital::right);

ControllerButton presetA(ControllerDigital::A);
ControllerButton presetX(ControllerDigital::X);
ControllerButton presetB(ControllerDigital::B);
ControllerButton presetY(ControllerDigital::Y);

//Global boolean variables
bool deployed = false; //Tells the deploy function if the robot has deployed yet. This helps ensure that it is not accidently used a second time during a match
bool armUp = false; //is the arm up (used to determine whether or not to activate auto tower function)
bool toggleAssist = true; //Auto Tower Assist feature starts on


int bPresetPos = 40;//number of degrees / 5 to set the lift arm above the zero position when the bPreset is pressed

/*
 These are the chassis variables that are used for the driver control
 period and the autonomous period.
*/
auto chassis = ChassisControllerBuilder()
	.withMotors(
		{driveFL, driveBL},
		{driveFR, driveBR}
	).withDimensions(
		AbstractMotor::gearset::green,
		{
			{4.08_in, 8.1_in},
			static_cast<int32_t>(imev5GreenTPR * 2.0)
		}
	).build();

auto profileController = AsyncMotionProfileControllerBuilder()
	.withLimits(
		{
			0.3,
			1.4,
			6.0
		}
	).withOutput(
		chassis
	).buildMotionProfileController();


/*
 This code is used at the beginning, when the program starts.
 This function is mainly used to set up motor brakemodes and encoders,
 and then generate paths (2D motion profiling) for auton.
*/
void initialize() {

pros::c::lcd_initialize();
	rollertrayL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollertrayR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterL.setBrakeMode(AbstractMotor::brakeMode::hold);
	armL.setBrakeMode(AbstractMotor::brakeMode::hold);
	armR.setBrakeMode(AbstractMotor::brakeMode::hold);

	tilterR.tarePosition();
	tilterL.tarePosition();

	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.8_ft, 0_ft, 0_deg}
		},
		"A"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{1_ft, 0_ft, 0_deg}
		},
		"B"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2_ft, 1.1_ft, 15_deg}
		},
		"C"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.6_ft, 0_ft, 0_deg}
		},
		"D"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2.5_ft, -1_ft, 0_deg}
		},
		"E"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{3.8_ft, 0_ft, 0_deg}
		},
		"F"
	);

	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{0.9_ft, 0_ft, 0_deg}
		},
		"1"
	);
	profileController->generatePath(
		{
			{0_ft, 0_ft, 0_deg},
			{2_ft, 0_ft, 0_deg}
		},
		"2"
	);
}

/*
 This runs the drive on an arcade control setup. The left vertical
 axis moves the drive up and down, while the right horizontal axis
 point the drive left and right.
*/
void driveControl() {
	chassis->getModel()->arcade(
		joystick.getAnalog(ControllerAnalog::leftY),
		joystick.getAnalog(ControllerAnalog::rightX) * .6
	);
}

/*
 Task runnable version of driveControl
*/
void driveControl1(void* param) {
	while(true) {
		driveControl();
		pros::delay(20);
	}
}

/*
 Moves the roller lift. Speed will depend on the speed parameter. Positive
 speed moves the lift up, negative speed moves the lift down. The range is -100
 to 100.
*/
void lift(int speed) {
	armL.moveVelocity(speed * 2);
	armR.moveVelocity(speed * 2);
}

/*
 Moves the roller lift to a specified position. Position is based on the pos
 parameter, positive pos will move the lift pos degrees up from the last tare
 angle and negative pos will move it pos degrees down from the last tare angle
 the range is all int numbers; Speed will depend on the speed parameter. The
 range is -100 to 100.
*/
void liftPosition(int pos, int speed) {
    armL.moveAbsolute(pos, speed);
    armR.moveAbsolute(pos, speed);
}

/*
 Moves the lift up or down, depending on the state of the up and down
 buttons(D pad). If the up button is pressed, the lift will move upwards. If
 the down button is pressed, the lift will move downwards. The up
 button has priority.
*/
void liftControl() {
	if (liftUp.changedToPressed()) {
		lift(60);
		if(armL.getPosition() > 100){
			armUp = true;
		} else {
			armUp = false;
		}
	} else if (liftDown.changedToPressed()) {
		lift(-60);
		if(armL.getPosition() < 100) {
			armUp = false;
		}
		if (armL.getPosition() < 0) {
			armL.tarePosition();
			armR.tarePosition();
			armUp = false;
		}
	}
	if (liftUp.changedToReleased() || liftDown.changedToReleased()) {
		lift(0);
		if(armL.getPosition() > 100) {
			armUp = true;
		}
	}
}

/*
 Moves both tray rollers. Speed will depend on the speed parameter and positive
 speed specifies out(Spit) while negative speed specifies in(Grab). The range
 is -100 to 100.
*/
void rollersTray(int speed) {
	rollertrayL.moveVelocity(speed);
	rollertrayR.moveVelocity(speed);
}

/*
 Moves both arm rollers. Speed will depend on the speed parameter and positive
 speed specifies out(Spit) while negative speed specifies in(Grab). The range
 is -100 to 100.
*/
void rollersArms(int speed) {
	rollerarmL.moveVelocity(speed);
	rollerarmR.moveVelocity(speed);
}

/*
 Moves the tray rollers a specified number of degrees. Degrees is based on
 the degrees parameter, positive degrees will turn the rollers out(spit) the
 specified numeber of degrees, negative degrees will turn the rollers in(Grab)
 the specified numeber of degrees; the range of degrees is all int numbers;
 Speed will depend on the speed parameter. The range is -100 to 100.
*/
void rollersTrayDegrees(int degrees, int speed) {
	rollertrayL.moveRelative(degrees, speed);
	rollertrayR.moveRelative(degrees, speed);
}

/*
 Moves the arm rollers a specified number of degrees. Degrees is based on
 the degrees parameter, positive degrees will turn the rollers out(spit) the
 specified numeber of degrees, negative degrees will turn the rollers in(Grab)
 the specified numeber of degrees; the range of degrees is all int numbers;
 Speed will depend on the speed parameter. The range is -100 to 100.
*/
void rollersArmsDegrees(int degrees, int speed) {
	rollerarmL.moveRelative(degrees, speed);
	rollerarmR.moveRelative(degrees, speed);
}

/*
 Function to move both sets of rollers a specified number of degrees(relative),
 speed is absolute and positive degrees specifies out(spit) while negative degrees
 specifies in(grab). Speed range is +100 to -100, degrees range is any int value
*/
void rollersDegrees(int degrees, int speed)
{
	rollersTrayDegrees(degrees, speed);
	rollersArmsDegrees(degrees, speed);
	int errorRange = 50; //the range where the function will stop moving the motors, plus or minus errorRange
	while ((rollerarmL.getPosition() > rollerarmL.getTargetPosition() + errorRange || rollerarmL.getPosition() < rollerarmL.getTargetPosition() - errorRange))
	{
		// Continue running this loop as long as the motor is not at its goal position
		pros::c::delay(2);
	}
}

/*
 Moves the rollers to intake and outtake depending on the state of
 the up and down buttons. If the up button is pressed, the
 manipulator will intake, and if the down button is pressed, the
 manipulator will outtake. The up button has priority.
*/
void rollersControl() {
	if (armL.getPosition() > 600) {
		rollersArms(0);
		if (intakeIn.isPressed()) {
			rollersTray(100);
		} else if (intakeOut.isPressed()) {
			rollersTray(-100);
		} else {
			rollersTray(0);
		}
	} else if (armL.getPosition() > 100) {
		rollersTray(0);
		if (intakeIn.isPressed()) {
			rollersArms(100);
		} else if (intakeOut.isPressed()) {
			rollersArms(-5);
		} else {
			rollersArms(0);
		}
	} else {
		if (intakeIn.isPressed()) {
			rollersArms(70);
			rollersTray(100);
			//pros::c::lcd_print(0, "Current Draw: %f ma",rollerarmL.getEfficiency());
		} else if (intakeOut.isPressed()) {
			rollersArms(-70);
			rollersTray(-100);
		} else {
			rollersArms(0);
			rollersTray(0);
		}
	}
}

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

/*
 Move the tray up or down, depending on the state of the up and down
 buttons. If the up button is pressed, the tray will move upwards. If
 the down button is pressed, the tray will move downwards. The up
 button has priority. When the tray is moving up, the closer it is to
 its resting position, the faster it will be. Once it reaches a
 certain point in its range of motion, it is capped at a maximum
 velocity.
*/
void tilterControl() {
	if (trayUp.changedToPressed()) {
		//If the the trayUp button is pressed, move the tray up at speed 80
		tilter(80);
	} else if (trayDown.changedToPressed()) {
		/*If the tilter(tray) is not all of the way up, then move it down at speed 100
		while the trayDown button is pressed*/
		if (tilterL.getPosition() < 100) {
			tilter(-100);
		} else {
			int vel = 50 + (250 + (-tilterL.getPosition() + 100)) * 0.2000;
			if (vel < 50) {
				vel = 50;
			}
			tilter(-vel);
		}
	}
	//When the tray control buttons are released, stop moving the tray
	if (trayUp.changedToReleased() || trayDown.changedToReleased()) {
		if (tilterL.getTargetVelocity() != 100) {
			tilter(0);
		}
	}
}

/*
 Function to  deploy the tray, roller arms, and anti-tip on match start or when
 the right button on the dpad is pressed.
*/
void deployTray() {
	int posTemp = 280;//Position to raise the arms to, when they go up the tray will deploy, followed by the arm rollers
	liftPosition(posTemp, 30);
	while (armL.getPosition() < posTemp - 1) { //halts code exectuion until the arms reach their intended position with a minus one for error
		continue;
	}
	lift(-30);//Brings the arms back down
	//Stops the arms when one or both arms triggers the zero button
	while (true) {
		if (armLStop.changedToPressed() || armRStop.changedToPressed()) {
			//stops moving the lift arms
			armL.moveVelocity(0);
			armR.moveVelocity(0);

			/*Since no brakeMode is set yet, the arms should fall to a natural resting
			position during the delay and will be synced together when tehy are zeroed*/
			pros::delay(200);

			//Zeros the lift arm position
			armL.tarePosition();
			armR.tarePosition();

			pros::delay(200);

			armL.setBrakeMode(AbstractMotor::brakeMode::hold);
			armR.setBrakeMode(AbstractMotor::brakeMode::hold);
			break;
		}
	}
	//Deploy the anti-tip by moving the tray up and then back down
	int tilterPosTemp = 100;//Position to raise the tilter(tray) to
	tilterPosition(tilterPosTemp, 90);

	 //halts code exectuion until the tray reachs its intended position with a minus one for error
	while (tilterR.getPosition() < tilterPosTemp - 1) {
		continue;
	}

	tilterPosition(0, 90);//brings the tilter(tray) back to zero position
	liftPosition(bPresetPos, 100);//brings the arm rollers up a bit from the zero position, this improves intake and cube grabbing for towering
	armUp = false;//Since the arms are down, set the armUp flag to false so that TowerAssist knows what to do
}

/*
 Function to automatically load cubes into the lift arms for scoring towerAssist
*/
void towerAssist()
{
	if(toggleAssist && !armUp) //if the assist is on and the arm is not up then run the following
	{
		// bring up cubes first
		rollersDegrees(-360, 100);
		pros::delay(20);
		//push 1 cube down
		rollersDegrees(360, 100);
	}
	armUp = true;//Lift arm will be moving up after this function so set ArmUp to true
}

/*
 Moves different mechanisms to certain positions based on the parameter, preset.
 Each preset will call different functions.
*/
void presets(string preset) {
	if (preset == "B") {
		/*
		 If the tilter(tray) is more than 10 degrees up, bring it down to resting
		 position(last zeroed position), otherwise bring the arms back down(usually)
		 to bPresetPos position.
		*/
		if (tilterR.getPosition() > 10) {
			tilterPosition(0, 100);//brings the tilter(tray) to the zero position
		} else {
			liftPosition(bPresetPos, 100);//Bring arms to bPresetPos
			armUp = false;
		}
	}
	/*
	 If the towerAssist is enabled, run towerAssist to put 1 cube in the arm
	 rollers and then bring the arm rollers up to specified heights.
	 Note: towerAssist does not run on presetY
	*/
	if (preset == "X") {
		towerAssist();
		liftPosition(445, 100);//Shortest tower height
	}
	if (preset == "A") {
		towerAssist();
		liftPosition(350, 100);//Medium tower height
	}
	if (preset == "Y") {
		liftPosition(800, 90); // behind the tray
	}
	/*
	 If the right arrow on the dpad is pressed, run the deploy function once
	 Note: the function runs when the key is changed to pressed
	*/
	if (preset == "right" && !deployed) {
		deployed = true;/*sets deployed to true so that the function cannot run
		again until the program is restarted, this helps ensure that it is not
		accidently used a second time during a match*/
		deployTray();//Runs the full deploy tray function
	}
	if (preset == "left") {
		toggleAssist = !toggleAssist;//Toggles the cube tower scoring assist feature
	}
}

/*
 Moves the lift to a specific height, depending on the button pressed.
*/
void presetControl() {
	if (presetA.isPressed()) {
		presets("A");
	}
	if (presetX.isPressed()) {
		presets("X");
	}
	if (presetB.isPressed()) {
		presets("B");
	}
	if (presetY.isPressed()) {
		presets("Y");
	}
	if (toggleAuto.changedToPressed()) {
	 presets("left");
 	}
	if (presetRight.isPressed()) {
	presets("right");
 	}
}

/*
 Runs the path (pathName). It can take in the "reversed" and
 "mirrored," but by default it treats both of them as false. This
 function will hold the code in place until it successfully reaches
 its target.
*/
void runPath(string pathName, bool reversed=false, bool mirrored=false) {
	profileController->setTarget(pathName, reversed, mirrored);
	profileController->waitUntilSettled();
}

/*
 Turns the robot clockwise to a certain angle (angle) with a certain
 velocity (speed).
*/
void turn(QAngle angle, int speed) {
	chassis->setMaxVelocity(speed * 2);
	chassis->turnAngle(angle / 2.2);
	chassis->setMaxVelocity(200);
}

/*
 Unused function to zero the lift arms with the buttons under each arm
*/
/*void waitForArmReset() {
	while (true) {
		bool l = armLStop.isPressed();
		bool r = armRStop.isPressed();

		if (l) {
			armL.moveVelocity(0);
		}
		if (r) {
			armR.moveVelocity(0);
		}

		if(l && r) {
			break;
		}
	}
}*/

/*
 Runs the autonomous function for the auton period.
*/
void autonomous() {
	//whichever runPath contains the true argument will run
	runPath("1", true);
	runPath("2");

	deployTray();//deploys the tray, roller arms, and anti tip
	deployed = true;/*sets deployed to true so that the function cannot run
	again until the program is restarted, this helps ensure that it is not
	accidently used a second time during a match*/
}

/*
 This function is called by the competition switch, and will call all of the
 control functions that are used during the driver control period.
*/
void opcontrol() {
	//zero the left and right lift arms
	armL.tarePosition();
	armR.tarePosition();

	//Set left and right lift arms and left and right lift arm rollers to brakeMode Hold
	armL.setBrakeMode(AbstractMotor::brakeMode::hold);
	armR.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmR.setBrakeMode(AbstractMotor::brakeMode::hold);

	//Multi threaded driveControl so that the robot can be driven while other code is executed
	pros::Task my_task(driveControl1,(void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "drive");

	//Loop through all Control Function groups to use the robots functionality
	while (true) {
		liftControl();
		rollersControl();
		tilterControl();
		presetControl();

		pros::delay(20);//delay by good convention
	}
}

/*
 Unused methods that are required by PROS. They might be used later.
*/
void disabled() {}
void competition_initialize() {}
void on_center_button() {}
