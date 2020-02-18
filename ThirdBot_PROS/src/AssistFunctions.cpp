#include "main.h"
#include "Motors.hpp"
#include "Sensors.hpp"
#include "DriveFunctions.hpp"
#include "LiftFunctions.hpp"
#include "RollerFunctions.hpp"
#include "TilterFunctions.hpp"
#include <string>
#include "AssistFunctions.hpp"

//These are the driver assist functions

/*
 Functionm to deploy the anti-tip by lifting the tray up and then lowering it
*/
void deployAntitip(void* param){
	//Deploy the anti-tip by moving the tray up and then back down
	int tilterPosTemp = 100;//Position to raise the tilter(tray) to
 tilterPosition(tilterPosTemp, 90);

	 //halts code exectuion until the tray reachs its intended position with a minus one for error
	while (tilterR.getPosition() < tilterPosTemp - 1) {
		pros::delay(1);
	}

	tilterPosition(0, 90);//brings the tilter(tray) back to zero position
}

/*
 Function to  deploy the tray, roller arms, and anti-tip on match start or when
 the right arrow button on the dpad is pressed.
*/
void deployTray(){
	int posTemp = 100;//Position to raise the arms to, when they go up the tray will deploy, followed by the arm rollers
	liftPositionDelay(posTemp, 40); //halts code exectuion until the arms reach their intended position with a minus one for error

	posTemp = 800;
	liftPositionDelay(posTemp, 80);//halts code exectuion until the arms reach their intended position with a minus one for error

	//deploy the anti-tip simultaneously with arm movement
	pros::Task my_task(deployAntitip,(void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "deploy");

	liftZero();//zero the lift arms

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
			master.rumble("-");//If toggle assist is on going off, rumble one long
		}else {
			master.rumble(".");//If toggle assist is off going on, rumble one short
		}
		toggleAssist = !toggleAssist;//Toggles the cube tower scoring assist feature
	}
	if (preset == "right" && deployed) {
		liftZero();//zero the lift arms
		liftPosition(bPresetPos, 100);//brings the arm rollers up a bit from the zero position, this improves intake and cube grabbing for towering
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
