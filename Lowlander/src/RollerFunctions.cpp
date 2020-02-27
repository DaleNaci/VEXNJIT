#include "main.h"
#include "UtilityAndMath.hpp"
#include "Motors.hpp"
#include "Variables.hpp"
#include "RollerFunctions.hpp"

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
This function calcualtes the statistical variables needed for the autoCubeGrab
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
int innerRollerBump(rollers r1, double zScore, int maxTime)
{
	//pros::delay(200);
		double meanL = r1.innerLMean;
		double meanR = r1.innerRMean;
		double stdL = r1.innerLSD;
		double stdR = r1.innerRSD;

		bool rollerLTorqueFlag = false;
		bool rollerRTorqueFlag = false;
		long t1;
		t1 = pros::millis();
		/*
 		 Runs while loop while the roller torques are not statistically significantly
 	 	 greater than what they were when calibrated.
		*/
		while((!rollerLTorqueFlag && !rollerRTorqueFlag) && ((int)(pros::millis() - t1) < maxTime))
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
		if((int)(pros::millis() - t1 >= maxTime)){
			return 2;
		}else if(rollerLTorqueFlag && rollerRTorqueFlag){
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
int outerRollerBump(rollers r1, double zScore, int maxTime)
{
		double meanL = r1.outerLMean;
		double meanR = r1.outerRMean;
		double stdL = r1.outerLSD;
		double stdR = r1.outerRSD;

		bool rollerLTorqueFlag = false;
		bool rollerRTorqueFlag = false;

		long t1;
		t1 = pros::millis();
		/*
 		 Runs while loop while the roller torques are not statistically significantly
 	 	 greater than what they were when calibrated.
		*/
		while((!rollerLTorqueFlag && !rollerRTorqueFlag) && ((int)(pros::millis() - t1) < maxTime))
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
		if((int)(pros::millis() - t1 >= maxTime)){
			return 2;
		}else if(rollerLTorqueFlag && rollerRTorqueFlag){
			return 0;
		}
		return 1;
}
