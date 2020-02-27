#include "main.h"
#include "Motors.hpp"
#include "Sensors.hpp"
#include "Variables.hpp"
#include "DriveFunctions.hpp"
#include "LiftFunctions.hpp"
#include "RollerFunctions.hpp"
#include "TilterFunctions.hpp"
#include "AssistFunctions.hpp"
#include "Autonomous.hpp"
#include "ControlFunctions.hpp"
//#include "Vectors2.hpp"

using namespace std;
#define VISION_PORT 2
#define EXAMPLE_SIG 1

/*pros::vision::signature YELLOW1 (1, -47, 49, 2, -3723, -2333, -3028, 0.000, 0);
pros::vision::signature RED1 (2, 12679, 13347, 13013, -3091, -2681, -2886, 4.900, 0);
pros::vision::signature PURPLE1 (3, 4039, 6127, 5082, -1121, 525, -298, 1.400, 0);
pros::vision::signature ORANGE1 (4, 5977, 10265, 8122, -5599, -4373, -4986, 1.700, 0);
pros::vision::signature GREEN1 (5, -5609, -2759, -4184, -8575, -4273, -6424, 1.500, 0);
pros::vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3.000, 0);
pros::vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision vision1 ( vex::PORT1, 150, YELLOW1, RED1, PURPLE1, ORANGE1, GRREN1, SIG_6, SIG_7 );*/
/*new
vision::signature YELLOW1 (1, 299, 2261, 1280, -2551, -1171, -1860, 1.200, 0);
vision::signature RED1 (2, 6741, 9537, 8140, -2881, -987, -1934, 3.000, 0);
vision::signature PURPLE1 (3, 511, 1873, 1192, 7043, 9691, 8366, 3.400, 0);
vision::signature ORANGE1 (4, 4299, 6291, 5294, -2579, -1199, -1888, 1.700, 0);
vision::signature GRREN1 (5, -7329, -5873, -6602, -4253, -2475, -3364, 5.200, 0);
vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3.000, 0);
vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision vision1 ( vex::PORT1, 150, YELLOW1, RED1, PURPLE1, ORANGE1, GRREN1, SIG_6, SIG_7 );
*/
// values acquired from the vision utility
pros::vision_signature_s_t GREEN_SIG =
	 pros::Vision::signature_from_utility(5, -5609, -2759, -4184, -8575, -4273, -6424, 1.500, 0);
	 pros::vision_signature_s_t PURPLE_SIG =
 		 pros::Vision::signature_from_utility(3, 511, 1873, 1192, 7043, 9691, 8366, 3.400, 0);

double xPos = 0; /*Distance in meters from the blue aliance tower corner to the
center of the anti tip wheel in thedirection of the blue protected zone*/
double yPos = 0; /*Distance in meters from the blue aliance tower corner to the
back of the undeployed robot in the direction of the red alliance tower corner*/
double heading = 0; /*Angle in degrees from blue alliance side wall to the line
passing through the anti tip wheel on the robot first and then following the rail*/


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::c::lcd_initialize();
	rollertrayL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollertrayR.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerarmR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterR.setBrakeMode(AbstractMotor::brakeMode::hold);
	tilterL.setBrakeMode(AbstractMotor::brakeMode::hold);

	tilterR.tarePosition();
	tilterL.tarePosition();

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Vision vision_sensor (VISION_PORT); //initializes the vision_sensor object at vision_port port
	vision_sensor.set_exposure(150);

	int size = 7;
	  pros::vision_object_s_t purpleCubes[7];

  while (true) {
		int counter = 0;
		while(counter++ < size)
		{
			purpleCubes[counter] = vision_sensor.get_by_sig(counter, 3);//initialize signature object
		}

for(counter = 0;counter < size;counter++)
{
	
}
    // Gets the largest object
    //std::cout << "sig: " << rtn.signature;
    //if (rtn.signature == 255) std::cout << " " << errno;

	int	x = rtn.x_middle_coord;//x middle coordinate of the object with signature 1
		int y = rtn.y_middle_coord;//y middle coordinate of the object with signature 1

		//Prints the center coordnates of the object with signature 1
	 printf("\r\nX %d", x);
		printf("\tY %d", y);

		    pros::delay(100);
  }
}
