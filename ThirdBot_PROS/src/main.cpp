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
int8_t ARM_LEFT_ROLLER_PORT = 21;//formerly 10 which is now dead
int8_t ARM_RIGHT_ROLLER_PORT = 5;
int8_t TRAY_LEFT_ROLLER_PORT = 8;
int8_t TRAY_RIGHT_ROLLER_PORT = 7;

//Drive Wheels
int8_t LEFT_FRONT_DRIVE_PORT = 9;
int8_t LEFT_BACK_DRIVE_PORT = 14;
int8_t RIGHT_FRONT_DRIVE_PORT = 6;
int8_t RIGHT_BACK_DRIVE_PORT = 13;

//Arm Stop Buttons
int8_t ARM_LEFT_STOP_SWITCH_PORT = 1;
int8_t ARM_RIGHT_STOP_SWITCH_PORT = 2;

//Shaft encoders
//Left encoder
int8_t ARM_LEFT_SHAFT_ENCODER_TOP_PORT = 3;
int8_t ARM_LEFT_SHAFT_ENCODER_BOTTOM_PORT = ARM_LEFT_SHAFT_ENCODER_TOP_PORT + 1;
//right encoder
int8_t ARM_RIGHT_SHAFT_ENCODER_TOP_PORT = 5;
int8_t ARM_RIGHT_SHAFT_ENCODER_BOTTOM_PORT = ARM_RIGHT_SHAFT_ENCODER_TOP_PORT + 1;

//Ultrasonic
int8_t ULTRASONIC_PING_PORT = 7;
int8_t ULTRASONIC_ECHO_PORT = ULTRASONIC_PING_PORT + 1;

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

//Sensor Declarartion
//Button stops
ADIButton armLStop(ARM_LEFT_STOP_SWITCH_PORT);
ADIButton armRStop(ARM_RIGHT_STOP_SWITCH_PORT);
//Shaft Encoders
ADIEncoder armLEncoder(ARM_LEFT_SHAFT_ENCODER_TOP_PORT, ARM_LEFT_SHAFT_ENCODER_BOTTOM_PORT, false);
ADIEncoder armREncoder(ARM_RIGHT_SHAFT_ENCODER_TOP_PORT, ARM_RIGHT_SHAFT_ENCODER_BOTTOM_PORT, true);//reversed, might have to switch which one is reversed
//Ultrasonic
pros::ADIUltrasonic UltraSensor(ULTRASONIC_PING_PORT, ULTRASONIC_ECHO_PORT);

//pros controller
pros::Controller master(pros::E_CONTROLLER_MASTER);
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

//Structures
struct rollers{
	double outerRMean;
	double outerLMean;
	double outerRSD;
	double outerLSD;
	double innerRMean;
	double innerLMean;
	double innerRSD;
	double innerLSD;
};

//Global boolean variables
bool deployed = false; //Tells the deploy function if the robot has deployed yet. This helps ensure that it is not accidently used a second time during a match
bool armUp = false; //Is the arm up (used to determine whether or not to activate auto tower function)
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
 Sets the left motors velocity to speedL and the right motors velocity to speedR
 to turn the chassis
*/
void turn2(int speedL, int speedR){
	driveFL.moveVelocity(speedL);
	driveBL.moveVelocity(speedL);
	driveFR.moveVelocity(speedR);
	driveBR.moveVelocity(speedR);
}

/*
 Turns the robot to a specified angle, positive angle means teh robot turns ccw
 and a negatoive angle means it turns c, speed is the velocity of the motors
*/
void turnAngle2(int angle, int speed){
	turn2(-speed,speed);
	float ctime = 26000 / 10 / 360;//the conversion from milliseconds to degrees
	int time = (int)(angle * ctime) + 100;
	pros::delay(time);
	turn2(0,0);
}

/*
 Sets all drive motors velocity to speed, range -100 - 100
*/
void move(int speed){
	driveFL.moveVelocity(speed);
	driveBL.moveVelocity(speed);
	driveFR.moveVelocity(speed);
	driveBR.moveVelocity(speed);
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
void rollersDegrees(int degrees, int speed) {
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
 Calculates the standard deviation of an array given the array, the mean, and
 the size of the array
*/
double calculateSD(double data[], double mean, int size){
    double standardDeviation = 0.0;

    for(int i = 0; i < size; ++i)
        standardDeviation += pow(data[i] - mean, 2);
    return sqrt(standardDeviation / size);
}

/*
This function calcualtes the statistical variables needed for teh autoCubeGrab
function. This function is to be used in autonomous mode and for programming
skills. samples is the number of samples to take when calibrating
torque, tInterval is the time in pros::c::milliseconds between samples.
Ex(Rollers roller = rollersInit(30, 10);)
*/
rollers rollersInit(int samples, int tInterval)
{
	//set rollers to grab
	rollersArms(-100);
	pros::delay(20);
	rollersTray(-100);
	pros::delay(20);

	//initialize arrays
	double innerLTorque[samples];
	double innerRTorque[samples];
	double outerLTorque[samples];
	double outerRTorque[samples];

	//stat variables
	double meanInnerL = 0; double meanInnerR = 0; double stdInnerL; double stdInnerR; //outer rollers
	double meanOuterL = 0; double meanOuterR = 0; double stdOuterL; double stdOuterR; //inner rollers

	//intialize clocks
	long t4 = pros::millis();
	long t3;

	//int tin = (int)length * 25 / 28;
	//generate a baseline
		for(int iter = 0;iter < samples;iter++)
		{
			innerLTorque[iter] = rollertrayL.getTorque();
			innerRTorque[iter] = rollertrayR.getTorque();
			outerLTorque[iter] = rollerarmL.getTorque();
			outerRTorque[iter] = rollerarmR.getTorque();

			//calculate the means
			meanInnerL += innerLTorque[iter] / samples;
			meanInnerR += innerRTorque[iter] / samples;
			meanOuterL += outerLTorque[iter] / samples;
			meanOuterR += outerRTorque[iter] / samples;


			t3 = pros::millis();
				while((int)(t3-t4) < (tInterval)) //-0.000031 is timing calibration value, todo, recalculate
				{
					t3 = pros::millis();
					pros::delay(1);
				}
				t4 = pros::millis();
		}
		//set the roller arms to 0
		rollersArms(0);
		rollersTray(0);

		//Calculate the standard deviations
		stdInnerL = calculateSD(innerLTorque, meanInnerL, samples);
		stdInnerR = calculateSD(innerRTorque, meanInnerR, samples);
		stdOuterL = calculateSD(outerLTorque, meanOuterL, samples);
		stdOuterR = calculateSD(outerRTorque, meanOuterR, samples);
		//end calculation of baseline statistics

		//return the calculated statistics
		rollers roller = {meanOuterR, meanOuterL, stdOuterR, stdOuterL, meanInnerR, meanInnerL, stdInnerL, stdInnerL};
		return roller;
}

/*
 Uses the torque exerted by the motors to detect when a cube is present in the
 inner rollers(arms). This function is to be used in autonomous mode and for
 programming skills.r1 is the rollers structure contatiing statisics information
 zScore is the z score corresponding to the confidence level(alpha) for the test.
 Returns 0 when done. Recommended declaration autoCubeGrab(roller, -3);
*/
int innerRollerBump(rollers r1, double zScore)
{
	//pros::delay(200);
		double meanL = r1.innerLMean;
		double meanR = r1.innerRMean;
		double stdL = r1.innerLSD;
		double stdR = r1.innerRSD;

		bool rollerLTorqueFlag = false;
		bool rollerRTorqueFlag = false;
		/*
 		 Runs while loop while the roller torques are not statistically significantly
 	 	 greater than what they were when calibrated.
		*/
		while(!rollerLTorqueFlag && !rollerRTorqueFlag)
		{
			double rollerLTorqueSample = rollertrayL.getTorque();
			double rollerRTorqueSample = rollertrayR.getTorque();
			double testStatisticL = (meanL - rollerLTorqueSample) / stdL;
			double testStatisticR = (meanR - rollerRTorqueSample) / stdR;
			if(testStatisticL < zScore){
				rollerLTorqueFlag = true;
			}else{
				rollerLTorqueFlag = false;
			}

			if(testStatisticR < zScore){
				rollerRTorqueFlag = true;
			}else{
				rollerRTorqueFlag = false;
			}
			pros::delay(2);
		}
		//rollersArms(0);//sets the arm roller speed to zero
		//rollersTray(0);//sets the tray roller speed to zero
		if(rollerLTorqueFlag && rollerRTorqueFlag){
			return 0;
		}
		return 1;
}

/*
 Uses the torque exerted by the motors to detect when a cube is present in the
 outer rollers(arms). This function is to be used in autonomous mode and for
 programming skills.r1 is the rollers structure contatiing statisics information
 zScore is the z score corresponding to the confidence level(alpha) for the test.
 Returns 0 when done. Recommended declaration autoCubeGrab(roller, 3);
*/
int outerRollerBump(rollers r1, double zScore)
{
		double meanL = r1.outerLMean;
		double meanR = r1.outerRMean;
		double stdL = r1.outerLSD;
		double stdR = r1.outerRSD;

		bool rollerLTorqueFlag = false;
		bool rollerRTorqueFlag = false;
		/*
 		 Runs while loop while the roller torques are not statistically significantly
 	 	 greater than what they were when calibrated.
		*/
		while(!rollerLTorqueFlag && !rollerRTorqueFlag)
		{
			double rollerLTorqueSample = rollerarmL.getTorque();
			double rollerRTorqueSample = rollerarmR.getTorque();
			double testStatisticL = (meanL - rollerLTorqueSample) / stdL;
			double testStatisticR = (meanR - rollerRTorqueSample) / stdR;
			if(testStatisticL < zScore){
				rollerLTorqueFlag = true;
			}else{
				rollerLTorqueFlag = false;
			}

			if(testStatisticR < zScore){
				rollerRTorqueFlag = true;
			}else{
				rollerRTorqueFlag = false;
			}
			pros::delay(2);
		}
		//rollersArms(0);//sets the arm roller speed to zero
		//rollersTray(0);//sets the tray roller speed to zero
		if(rollerLTorqueFlag && rollerRTorqueFlag){
			return 0;
		}
		return 1;
}
/*
 Uses the torque exerted by the motors to detect when a cube is present in the
 inner rollers(arms). This function is to be used in autonomous mode and for
 programming skills.r1 is the rollers structure contatiing statisics information
 zScore is the z score corresponding to the confidence level(alpha) for the test.
 eject is for if it should pu the cube in place for towering, true for towering,
 false for putting it in the tray. Returns 0 when done. Recommended declaration
 autoCubeGrab(roller, 3, false);
*/
int autoCubeGrab(rollers r1, double zScore,bool eject)
{
		move(20);
		rollersArms(-100);
		rollersTray(-100);

		int flag = innerRollerBump(r1, zScore);//grab and go forward until the inner rollers are bumped by the cube

		move(0);//stops forward movement of the robot
		rollersArms(0);//sets the arm roller speed to zero
		rollersTray(0);//sets the tray roller speed to zero
		if(eject){
			rollersDegrees(150, 50);
		}else{
			rollersDegrees(-360, 50);
		}
		return flag;
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
	int posTemp = 800;//Position to raise the arms to, when they go up the tray will deploy, followed by the arm rollers
	liftPosition(posTemp, 100);
	while (armL.getPosition() < posTemp - 1) { //halts code exectuion until the arms reach their intended position with a minus one for error
		pros::delay(1);
	}
	lift(-100);//Brings the arms back down
	//Stops the arms when one or both arms triggers the zero button
	while (true) {
		if (armLStop.changedToPressed() || armRStop.changedToPressed()) {
			//stops moving the lift arms
			armL.moveVelocity(0);
			armR.moveVelocity(0);

			/*Since no brakeMode is set yet, the arms should fall to a natural resting
			position during the delay and will be synced together when they are zeroed*/
			pros::delay(200);

			//Zeros the lift arm position
			armL.tarePosition();
			armR.tarePosition();

			//Zeros the shaft encoders on the lift arms
			armLEncoder.reset();
			armREncoder.reset();

			pros::delay(100);

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
		pros::delay(1);
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
		if(toggleAssist) {
			master.rumble("-");//If toggle assist is on going off, rumble teo longs
		}else {
			master.rumble(".");//If toggle assist is off going on, rumble one short
		}
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
 This function is used for testing to get ultrasonic sensor vaules printed to
 terminal. Values can be accessed with putty see pros docs debug for details.
 tInterval is the time in milliseconds between samples, liftArmPos is the
 position to raise the arm too(number of degreees / 5 above the start position)
*/
void ultraScanner(int tIntervl, in liftArmPos){
	liftPosition(liftArmPos, 50);
	pros::delay(1000);
	while(true)
	 {
		 pros::delay(tInterval);
		 printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	 }
}

/*
	Used with the ultrasonic sensor to find the center of a pole and turn to it.
	Length is the number of data points to collect, range [5 is a good number] is
	the number of data points to sample on each side of a point for an average,
	tInterval is the time in pros::c::milliseconds between samples minimum is 50 milliseconds, decay [0.0 – 1.0] how
	slowly to decay from raw value.Speed determines the turn speed
*/
void centerDetect(int length, int range, int tInterval, double decay, int speed) //see also low-pass-filter-method now at [https://web.archive.org/web/20180922093343/http://www.robosoup.com/2014/01/cleaning-noisy-time-series-data-low-pass-filter-c.html] (no longer exists)[https://www.robosoup.com/2014/01/cleaning-noisy-time-series-data-low-pass-filter-c.html]
{
	int ns = length;
  double data[ns];
  double clean[ns];
	long t1 = pros::c::millis();
	long t2 = pros::c::millis();
  long t3;
	int iter;
    for(iter = 0;iter < ns;iter++)
    {
    	data[iter] = UltraSensor.get_value();
			if(data[iter] <= 0)
			{
				data[iter] = 5000;
			}
			//printf("Range: %f m\r\n",data[iter]);
			t2 = pros::c::millis();
				printf("\r\n%d, ",(int) (t2 - t1));
				printf("%d",(int)data[iter]);
  			t3 = pros::c::millis();
        while((t3-t1) < (tInterval * (iter + 1))) //-0.000031 is timing calibration value, todo, recalculate
        {
 					t3 = pros::c::millis();
					turn2(-speed,speed);
          pros::delay(1);
        }
    }
  turn2(0, 0);
	//printf("Data: %d",(int)data[1]);

	//If you are trying to figure out how this section works, good luck
/*
	//Calculate coefficients
  double coefficients[range+1];
  int i;
  for (i = 0; i <= range; i++)
  {
    coefficients[i] = pow((double)decay, (double)i);
      //printf("\nCoeff:%f",coefficients[i]);
  }
	//printf("Coeffs calcd");
  // calculate the divisor
  double divisor = 0;
  for (i = -range; i <= range; i++)
  {
    divisor += coefficients[abs(i)];
  }
	//printf("Divisor: %f",divisor);
  // clean data
  for (i = range; i < length - range; i++)
  {
    double temp = 0;
    int j;
    for (j = -range; j <= range; j++)
    {
      temp += data[i + j] * coefficients[abs(j)];
    }
    clean[i] = temp / divisor;
      //printf("Clean: %f\n",clean[i]);
  }

  // find leading and trailing slopes
  double leadSum = 0;
  double trailSum = 0;
  int leadRef = range;
  int trailRef = length - range - 1;
  for (i = 1; i <= range; i++)
  {
    leadSum += (clean[leadRef] - clean[leadRef + i]) / i;
    trailSum += (clean[trailRef] - clean[trailRef - i]) / i;
  }

  //clean edges
  double leadSlope = leadSum / range;
  double trailSlope = trailSum / range;
  for (i = 1; i <= range; i++)
  {
    clean[leadRef - i] = clean[leadRef] + leadSlope * i;
    clean[trailRef + i] = clean[trailRef] + trailSlope * i;
  }
    /*for(i=0;i<length;i++)
    {
     printf("Cleaned Data: %f\n", clean[i]);
	 }*/
	 //find the maximum value in the range, this should be the peak of a sine wave generated by looking at the pipe
	/*int pos = 3;
	 int x;
	 for(x = 4;x < length-4;x++)
	 {

		 if(clean[x] < clean[pos])
		 {
			 pos = x;
		 }
	 }*/
	 int pos = 0;
	 //calculate the inverse postition since it will be turnign backwards
	 ns = length - pos;
   t1 = pros::c::millis();
	 //printf("Ns: %d",ns);
	 //turn the robot to the intended position using time values
	 for(iter = 0;iter < ns ;iter++)
	 {
		 t3 = pros::c::millis();
		 while((t3 - t1) < (tInterval * (iter + 1))) //-0.000031 is timing calibration value
		 {
			 t3 = pros::c::millis();
			 turn2(speed,-speed);
			 pros::delay(1);
		 }
	 }
	 turn2(0,0);//sets the robots turn speed to zero so it stops turning

}

/*
 This functions serchs for and then navigates towards a  tower using a line
 follower like algorithm. lowerBound is the distacne a point must be above to
 trigger the navigation start, upperBound is the value the point must be below,
 tInterval is the time in milliseconds between samples(minimum of 50(approx:
 refresh rate of sensor)), turnSpeed is the speed the robot will turn at where
 positive turnSpeed is clockwise turning and negative turnSpeed is
 counterclockwise turning, moveSpeed is the speed to move the robot where
 positive moveSpeed is forward and negative moveSpeed is backward deployDistance
 is the distance in ultrasonic sensor units to move the arm up at.
*/
void towerDetect(int lowerBound, int upperBound, int tInterval, int turnSpeed, int moveSpeed, int deployDistance){
//turn2(0,0);
turn2(-3*turnSpeed,3*turnSpeed);

 /*while(true)
	{
		pros::delay(tInterval);
		printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	}*/
	while(!(UltraSensor.get_value() > lowerBound && UltraSensor.get_value() < upperBound))//turns until it detects an object in the bounding area
	{
		pros::delay(tInterval);
		printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	}
		turn2(-turnSpeed, turnSpeed);
	float x = UltraSensor.get_value();
	long t1 = pros::millis();
	bool ignore = true;
	while(x >= (float)UltraSensor.get_value() || ignore)
	{
		x = (float)UltraSensor.get_value();
		printf("Range: %f m\r\n",(float)x);
		ignore = false;
		if(pros::millis() - t1 < 500 && ((x < lowerBound) || (x > upperBound)))
		{
			ignore = true;
		}
		pros::delay(tInterval);
}
	printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	printf("end\r\n");
	turn2(0,0);
/*	long t1, t2;
	t1 = pros::millis();
	/*bool a = false;
	while(a || (UltraSensor.get_value() > lowerBound && UltraSensor.get_value() < upperBound))//turns until it no longer detects an object in the bounding area
	{
		a = false;
		if(UltraSensor.get_value() == -1)
		{
			a = true;
		}
		pros::delay(tInterval);
		printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	}
	t2 = pros::millis();
	turn2(turnSpeed, -turnSpeed);
	pros::delay((int)((t2 - t1) / 2)); // turn to the center of the object
move(moveSpeed);
	while(UltraSensor.get_value() < deployDistance)
	{
		pros::delay(tInterval);
	}*/
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

//backup auton path for one cube push, antitip wheel will push
/*runPath("1", true);
runPath("2");*/

/*
 Runs the autonomous function for the auton period.
*/
void autonomous() {
	bool blue = true;//if its running on the blue side this is true
	toggleAssist = false;//turns off tower assist
	move(-50);
	deployTray();//deploys the tray, roller arms, and anti tip
	deployed = true;/*sets deployed to true so that the function cannot run
	again until the program is restarted, this helps ensure that it is not
	accidently used a second time during a match*/

	//make sure the robot is against the wall
	move(-50);
	pros::delay(100);

	//value for comparison in roller bump tests
	int zScore = -3;
	int zScore2 = -1;
	//Create the roller object
	rollers roller;
<<<<<<< HEAD

=======
	//et all rollers to full intake speed
>>>>>>> d9cc297ea713d8b5d27a9067c1d446c3403d88a9
	rollersArms(-100);
	rollersTray(-100);
	//calibrate the rollers
	roller = rollersInit(30, 10);
	//Stop moving forward
	move(0);

	//grabs cube 1

	//Set all rollers to full intake speed
	rollersArms(-100);
	rollersTray(-100);
	//Start moving forward
	move(50);
	//Move forard until theouter roller bumps the cube
	outerRollerBump(roller, zScore);
<<<<<<< HEAD
	autoCubeGrab(roller, zScore2, false);

/*	rollersArms(-100);
	rollersTray(-100);
	pros::delay(500);
	rollersArms(0);
	rollersTray(0);
*/
	move(50);
	pros::delay(150);
	move(0);
	//liftPosition(110, 100);//comment

	//put cube 1 in the tower
	if(blue){
		turnAngle2(45,50);
	}else{
		turnAngle2(45,-50);
	}
=======
	//Grb the cube until it hits the inner rollers and then spit it out a bit
	autoCubeGrab(roller, zScore2);
	//go forward a bit
	move(50);
	pros::delay(150);
	move(0);
	//lift the arms up to 110 so the cube is off the ground
	liftPosition(110, 100);

	//put cube 1 in the tower
	turnAngle2(45,50);
>>>>>>> d9cc297ea713d8b5d27a9067c1d446c3403d88a9

	move(50);
	pros::delay(200);
	move(0);
/*	rollersArms(100);
	pros::delay(500);
<<<<<<< HEAD
	rollersArms(0);*///comment
	//pros::delay(2000);
	liftPosition(180, 100);
	if(blue){
		turnAngle2(10,50);
	}else{
		turnAngle2(10,-50);
	}

	if(blue){
	towerDetect(100, 500, 30, 1, 20, 300);
	}else{
		towerDetect(100, 500, 30, -1, 20, 300);
	}

	//pros::delay(2000);
=======
	rollersArms(0);

	turnAngle2(10,50);
	liftPosition(200, 100);

	towerDetect(100, 500, 30, 1, 20, 300);

>>>>>>> d9cc297ea713d8b5d27a9067c1d446c3403d88a9
	move(-50);
	pros::delay(200);
	move(0);
	roller = rollersInit(30, 10);//calibrate the rollers
	liftPosition(bPresetPos, 100);
	move(50);
	rollersArms(-100);
	rollersTray(-100);
	outerRollerBump(roller, zScore);//grab a second cube
	autoCubeGrab(roller, zScore2, true);
	move(0);
	pros::delay(500);
	move(-50);
	pros::delay(600);
	move(0);
	presets("X");
	pros::delay(200);

	move(50);

	pros::delay(900);

	move(0);

	rollersArms(70);
	pros::delay(1500);
	rollersArms(0);
	//end tower 1

 //backup
 move(-50);
 pros::delay(3000);
 move(0);

//turn to face scoring zone
 if(blue){
 	turnAngle2(135,50);
 }else{
 	turnAngle2(135,-50);
 }

 //move forward
 move(50);
 pros::delay(500);
 move(0);

 //move forward 2
 move(10);
 pros::delay(3000);
 move(0);
 
 //eject cube
 rollersTray(100);
 pros::delay(1000);
 rollersTray(0);

 //backup
 move(-50);
 pros::delay(500);
 move(0);


<<<<<<< HEAD
/*
	//turnAngle2(5,50);
=======

>>>>>>> d9cc297ea713d8b5d27a9067c1d446c3403d88a9
//back align on wall
	move(-90);
	pros::delay(4000);
	move(0);
//go forward
	move(50);
	pros::delay(1800);
	move(0);

	turnAngle2(95, -50);
	liftPosition(115, 100);
//find tower 2
	towerDetect(600, 1000, 30, 1, 20, 300);
/*Problem where it sees an image of tower 2 early and senses teh center as the
right most edge. Temporary solution is to rerun towerDetect
*/
	towerDetect(600, 1000, 30, 1, 20, 300);
<<<<<<< HEAD
	//towerDetect(600, 1000, 30, 1, 20, 300);
	int tInterval = 30;
	while(true)
	 {
		 pros::delay(tInterval);
		 printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	 }
	//pros::delay(2000);

=======
>>>>>>> d9cc297ea713d8b5d27a9067c1d446c3403d88a9
	rollersArms(-100);
	rollersTray(-100);
	pros::delay(50);
	roller = rollersInit(30, 10);//calibrate the rollers
	liftPosition(bPresetPos, 100);
	move(50);
	rollersArms(-100);
	rollersTray(-100);
	pros::delay(50);
	outerRollerBump(roller, zScore);
	autoCubeGrab(roller, zScore2);
	move(0);
	pros::delay(500);

	move(-50);
	pros::delay(700);
	move(0);
	presets("A");

	move(50);

	pros::delay(800);

	move(0);


	rollersArms(70);
	pros::delay(500);
	rollersArms(0);

	move(-50);
	pros::delay(800);
	move(0);
<<<<<<< HEAD
	*/
/*
	//grab cube 2
	presets("B");
	pros::delay(2000);
	move(50);
	//pros::delay(2000);
	rollersArms(-100);
	rollersTray(-100);
	//pros::delay(2000);
	outerRollerBump(roller, zScore);
=======
>>>>>>> d9cc297ea713d8b5d27a9067c1d446c3403d88a9

	//go forward until outer rollers hit cube
	//autocube grab
	//Backup a bit
	//arms up (maybee 200 degrees)
	//turn ccw 45 Degrees
	//tower detect medium tower
	//presets("X")
	//go forward
	//eject cube
	//Backup
	//presets("B")
	//go forward and auto cube grab
	//Backup a little
	//presets("A")
	//turn around
	//go forward until it hits the green cube(Bottem left)
	//arms to 200 Degrees
	//turn ccw 45 Degrees
	//tower detect
	//presets("A")
	//go forward
	//eject cube
	//Backup
	//presets("B")
	//go forward and auto cube grab
	//Backup a little
	//presets("A")
/*
	liftPosition(160,90);
	pros::delay(1000);
	centerDetect(50, 5, 30, 0.8, 2);
	*/
}

/*
 This function is called by the competition switch, and will call all of the
 control functions that are used during the driver control period.
*/
void opcontrol() {
<<<<<<< HEAD
/*	int tInterval = 30;
	while(true)
	 {
		 pros::delay(tInterval);
		 printf("Range: %f m\r\n",(float)UltraSensor.get_value());
	 }*/
	//turnAngle2(100000,50);
	//pros::delay(5000);
=======
	//ultraScanner(30, 100);//used to get ultrasonic sensor values for testing

>>>>>>> d9cc297ea713d8b5d27a9067c1d446c3403d88a9
	toggleAssist = true;//turns on tower assist

	//Multi threaded driveControl so that the robot can be driven while other code is executed
	pros::Task my_task(driveControl1,(void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "drive");

	deployTray();//deploys the tray, roller arms, and anti tip
	deployed = true;/*sets deployed to true so that the function cannot run
	again until the program is restarted, this helps ensure that it is not
	accidently used a second time during a match*/

	//zero the left and right lift arms
	armL.tarePosition();
	armR.tarePosition();

	//Set left and right lift arms and left and right lift arm rollers to brakeMode Hold
	armL.setBrakeMode(AbstractMotor::brakeMode::hold);
	armR.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmR.setBrakeMode(AbstractMotor::brakeMode::hold);

	//Multi threaded driveControl so that the robot can be driven while other code is executed
	//pros::Task my_task(driveControl1,(void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "drive");

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
