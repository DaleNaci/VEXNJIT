#include "main.h"
#include "AutonomousFunctions.hpp"
#include "AssistFunctions.hpp"
#include "DriveFunctions.hpp"
#include "LiftFunctions.hpp"
#include "RollerFunctions.hpp"
#include "TilterFunctions.hpp"
#include "Autonomous.hpp"

void auton(bool blue)
{
  toggleAssist = false;//turns off tower assist
  move(-50);
  deployTray();//deploys the tray, roller arms, and anti tip
  deployed = true;/*sets deployed to true so that the function cannot run
  again until the program is restarted, this helps ensure that it is not
  accidently used a second time during a match*/

  //make sure the robot is against the wall
  move(-50);
  //pros::delay(500);

  //value for comparison in roller bump tests
  int zScore = -3;
  int zScore2 = -1;
  //Create the roller object
  rollers roller;

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
  //Move forard until the outer roller bumps the cube
  int i = outerRollerBump(roller, zScore, 4000);
  if(i == 2)
  {
    move(0);
    rollersArms(0);//sets the arm roller speed to zero
    rollersTray(0);//sets the tray roller speed to zero
    pros::delay(60000);
  }

  autoCubeGrab(roller, zScore2, false, 2000);

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

  move(50);
  pros::delay(200);
  move(0);
  /*	rollersArms(100);
  pros::delay(500);
  rollersArms(0);*///comment
  //pros::delay(2000);
  liftPosition(230, 100);
  if(blue){
    turnAngle2(10,50);
  }else{
    turnAngle2(10,-50);
  }

  if(blue){
  towerDetect(100, 700, 30, 1, 20, 300);
  }else{
    towerDetect(100, 700, 30, -1, 20, 300);
  }

  //pros::delay(2000);
  move(-50);
  pros::delay(200);
  move(0);
  roller = rollersInit(30, 10);//calibrate the rollers
  liftPosition(bPresetPos, 100);
  move(50);
  rollersArms(-100);
  rollersTray(-100);
  outerRollerBump(roller, zScore, 10000);//grab a second cube
  autoCubeGrab(roller, zScore2, true, 10000);
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


  /*
  //turnAngle2(5,50);
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
  /*
  towerDetect(600, 1000, 30, 1, 20, 300);
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
  autoCubeGrab(roller, zScore2,);
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
  */

}

void autonBlue(){
  auton(true);
}

void autonRed(){
  auton(false);
}

void autonPush(){
  toggleAssist = false;//turns off tower assist
  //backup auton path for one cube push, antitip wheel will push
  runPath("1", true, false);
  runPath("2", false, false);

  deployTray();//deploys the tray, roller arms, and anti tip
  deployed = true;/*sets deployed to true so that the function cannot run
  again until the program is restarted, this helps ensure that it is not
  accidently used a second time during a match*/

}
