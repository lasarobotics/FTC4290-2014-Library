#pragma config(Sensor, S4,     HTSPB,                sensorI2CCustom9V)

/***** DEFINES *****/
//#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../drivers/hitechnic-superpro.h" //SuperPro Prototype Board Driver

/***** CONSTANTS *****/
const int responsesize = 4; //Response size
const int motorarray[4] =  {50,46,48,32};
const int servoarray[4] =  {49,46,52,32};
sbyte I2CmessageM1[4]; //Motor Controler 1
sbyte I2CreplyM1[4]; //Motor Controler 1
sbyte I2CmessageS1[4]; //Servo Controler 1
sbyte I2CreplyS1[4]; //Servo Controler 1

/***** VARIABLES *****/
//Global Error Checking
bool error = false;
void reset(){
   I2CreplyM1[0] = 0;
 	 I2CreplyM1[1] = 0;
 	 I2CreplyM1[2] = 0;
 	 I2CreplyM1[3] = 0;
 	 I2CreplyS1[0] = 0;
 	 I2CreplyS1[1] = 0;
 	 I2CreplyS1[2] = 0;
 	 I2CreplyS1[3] = 0;
 	 error = false;
}

void interpret (sbyte* sensor,string &display, bool servo ){
	int convArray[responsesize];

	//Convert array to int array
	for (int i=0; i<responsesize; ++i)
  	 convArray[i] = sensor[i];
  bool same = true;
 	//compare to servo or motor array
  if (servo){
  	for (int i = 0; i < responsesize; i++){
  		if (convArray[i] != servoarray[i])
  			same = false;
  	}
	}
	else {
		 for (int i = 0; i < responsesize; i++){
  		if (convArray[i] != motorarray[i])
  			same = false;
  	}
	}

	//Set message
 	if (same){
 		if (servo){
 			display = "SC good to go!";
 		}
 		else {
 			display = "MC good to go!";
 		}
	}
	else {
 		if (servo){
 			display = "SC error!";
 			error = true;
 		}
 		else {
 			display = "MC error!";
 			error = true;
 		}
	}
}

task main()
{
	 //Setup PrototypeBoard for output on B0 of NXT Port 4 (0x01 represents the pin B0 and Port 4 is defined above in pragma config)
   HTSPBsetupIO(HTSPB, 0x1);
	 //Set sensor type to custom I2C so that we are using I2C drivers and not default MotorController
	 SensorType[S1] = sensorI2CCustom;
 	 wait10Msec(5);
 	 //Wait for sensor to be ready for I2C, poll every 2ms
	 while (nI2CStatus[S1] == STAT_COMM_PENDING)
	 	wait1Msec(2);
	//Expected output size
	I2CmessageM1[0] = responsesize;
	I2CmessageS1[0] = responsesize;
	//Device address
	//Servo(Daisy Chain 1 = 0x02)
	I2CmessageS1[1] = 0x02;
	//Motor(Daisy Chain 2 = 0x04)
	I2CmessageM1[1] = 0x04;
	//Read Location
	I2CmessageM1[2] = 0x00;
	I2CmessageS1[2] = 0x00;
	//I2C coms loop
	while (true){
		//Send message
		sendI2CMsg(S1, &I2CmessageM1[0], 4);
		//Wait while I2C is processing request, check every 2ms
 		while (nI2CStatus[S1] == STAT_COMM_PENDING)
 			wait1Msec(2);
 		//Set array to reply
	 	readI2CReply(S1, &I2CreplyM1[0], responsesize);
	 	//Repeat for sevo
	 	sendI2CMsg(S1, &I2CmessageS1[0], 4);
	 	while (nI2CStatus[S1] == STAT_COMM_PENDING)
 			wait1Msec(2);
	 	readI2CReply(S1, &I2CreplyS1[0], responsesize);
	 //Display human readable results, based on logic in interpret()
	 //refresh display
	 eraseDisplay();
	 string display = "";
	 //Logic to set display string
	 interpret(I2CreplyM1,display,false);
	 wait1Msec(10);
	 nxtDisplayString(3, "%s",display );
	 //Logic to set display string
	 interpret(I2CreplyS1,display,true);
	 wait1Msec(10);
	 nxtDisplayString(4, "%s", display);
	 //LED logic based on "error" global bool"
	if (error){
	  //No LED, because we have error
		HTSPBwriteIO(HTSPB, 0x00);
	}
	else {
 		//LED, we are good
 		HTSPBwriteIO(HTSPB, 0x01);
	}
 	 //Reset for next tick
	 reset();
	 wait1Msec(1000);
	}
}
