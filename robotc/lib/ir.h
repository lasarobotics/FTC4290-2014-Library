/**********************************************************

IR Operation
ir.h

Mathematics and drivers required for operation
and functionality of the IR sensor.

**********************************************************/

/**** INCLUDES ****/
#include "../../drivers/hitechnic-irseeker-v2.h" //IR seeker drivers

/**** Globals ****/
bool ir_loggingEnabled = true; //Enable Logging
string ir_filename = "irlog.txt"; // the name of our file

/**** File OPERATIONS ****/
static int filesize = 5000; // will store our file size
static TFileHandle   filehandle;
static TFileIOResult ioresult;

/**** READING OPERATIONS ****/
static tSensors ir_sensor = 0;
static bool ir_read = true;
static int IRraw[5];
static int samplecount = 0;

/**** MOVING AVERAGE VARS ****/
static const int samples = 5; // moving average sample size
static int indexavg[5];       // current index for each sensor
static int IRavgstore[5][samples];  // sample list to contain ir averages
static int IRavg[5];//Current IR Averages

/***** QUICK METHODS *****/
//Get current raw IR value
float ir_getraw(int sensor) { return IRraw[sensor]; }
//Get current avg IR value from the last <samples> samples
float ir_getavg(int sensor) { return IRavg[sensor]; }
//Stop current IR reading operation.
void ir_stopRead() { ir_read = false; }

/**
 * Holds processing until a certain count of samples has been read
 **/
void ir_wait(int count)
{
	int read = 0;
	int lastcount = -1;
	while (read < count)
	{
		if (samplecount != lastcount)
		{
			read++;
			lastcount = samplecount;
		}
		wait1Msec(10); //wait half of the actual read time, eliminates most error
	}
}

/**
* Reset IR thresholds and average values to default values for next reading operation.
**/
void ir_reset()
{
	for (int i=0; i<5; i++)
	{
		//Initialize Arrays
		IRavg[i] = 0;
		IRraw[i] = 0;
		indexavg[i] = 0;
		for (int j=0; j<samples;j++)
		{
			IRavgstore[i][j] = 0;
		}
	}
}

/**
* Initialize the IR sensor for reading.
**/
void ir_init(tSensors ir)
{
	ir_sensor = ir;
	ir_read = true;
	ir_reset();
}

/**
* Moving Average
* @param Sensor (1-5)
* @param New data point
*/
void ir_moveavg(int sensor, int new)
{
	//Reset to index 0 when index too large
	if (indexavg[sensor] == samples){
		indexavg[sensor] = 0;
	}

	//Get new value
	IRavgstore[sensor][indexavg] = new;

	//Calculate Average
	float avg = 0;
	for (int i=0; i<samples; i++)
	{
		avg += (float)IRavgstore[sensor][i];
	}
	avg = avg/samples;

	//Store Average
	IRavg[sensor] = avg;
	indexavg[sensor]++;

	return;
}

/**
* File Setup
* Setup File Handles for Logging
*/
void ir_setupLogging()
{
	if (ir_loggingEnabled) {
		//Delete Previous File
		Delete(ir_filename,ioresult);
		//open the file for writing (creates the file if it does not exist)
		OpenWrite(filehandle, ioresult, ir_filename, filesize);
		WriteText(filehandle, ioresult, "IR0,IR1,IR2,IR3,IR4,AVG0,AVG1,AVG2,AVG3,AVG4\r\n");
	}
}

/**
* Safely Stop Logging
**/
void ir_stopLogging()
{
	if (ir_loggingEnabled) {
		ir_loggingEnabled = false;
		wait1Msec(50); //wait in case handles are still in use
		Close(filehandle, ioresult);
	}
}

void logValues(int ir0,int ir1,int ir2,int ir3,int ir4,int avg0,int avg1,int avg2,int avg3,int avg4){
	if (!ir_loggingEnabled) { return; }
	string s = "";
	//IR0
	StringFormat(s,"%i,",ir0);
	WriteText(filehandle, ioresult, s);
	//IR1
	StringFormat(s,"%i,",ir1);
	WriteText(filehandle, ioresult, s);
	//IR2
	StringFormat(s,"%i,",ir2);
	WriteText(filehandle, ioresult, s);
	//IR3
	StringFormat(s,"%i,",ir3);
	WriteText(filehandle, ioresult, s);
	//IR4
	StringFormat(s,"%i,",ir4);
	WriteText(filehandle, ioresult, s);
	//AVG0
	StringFormat(s,"%i,",avg0);
	WriteText(filehandle, ioresult, s);
	//AVG1
	StringFormat(s,"%i,",avg1);
	WriteText(filehandle, ioresult, s);
	//AVG2
	StringFormat(s,"%i,",avg2);
	WriteText(filehandle, ioresult, s);
	//AVG3
	StringFormat(s,"%i,",avg3);
	WriteText(filehandle, ioresult, s);
	//AVG4
	StringFormat(s,"%i\r\n",avg4);
	WriteText(filehandle, ioresult, s);
}

/**
* IR calibration task
* Call before your while(true) loop with priority 8
*/
task ir_calibrate()
{
	if (ir_loggingEnabled){
		ir_setupLogging();
	}
	while (ir_read)
	{
		// Wait until 20ms has passed
		wait1Msec(20);
		//Read
		HTIRS2readAllACStrength(HTIRS2, IRraw[0], IRraw[1], IRraw[2], IRraw[3], IRraw[4]);
		//Move Averages
		ir_moveavg(0,IRraw[0]);
		ir_moveavg(1,IRraw[1]);
		ir_moveavg(2,IRraw[2]);
		ir_moveavg(3,IRraw[3]);
		ir_moveavg(4,IRraw[4]);
		//Log
		if (ir_loggingEnabled){
			logValues(IRraw[0],IRraw[1],IRraw[2],IRraw[3],IRraw[4],IRavg[0],IRavg[1],IRavg[2],IRavg[3],IRavg[4]);
		}
		samplecount++;
		if (samplecount > 1022) { samplecount = 0; }
	}
}
