#include "main.h"
#include <string>

using namespace std;

#ifndef ASSISTFUNCTIONS_HPP
#define ASSISTFUNCTIONS_HPP


//These are the driver assist functions

/*
 Functionm to deploy the anti-tip by lifting teh tray up and then lowering it
*/
void deployAntitip(void* param);

/*
 Function to  deploy the tray, roller arms, and anti-tip on match start or when
 the right arrow button on the dpad is pressed.
*/
void deployTray();

/*
 Function to automatically load cubes into the lift arms for scoring towerAssist
*/
void towerAssist();

/*
 Moves different mechanisms to certain positions based on the parameter, preset.
 Each preset will call different functions.
*/
void presets(string preset);//using namespace std

/*
 Runs the path (pathName). It can take in the "reversed" and
 "mirrored," but by default it treats both of them as false. This
 function will hold the code in place until it successfully reaches
 its target.
*/
void runPath(string pathName, bool reversed, bool mirrored);


#endif
