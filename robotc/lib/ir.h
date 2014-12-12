/**********************************************************

IR Operation
ir.h

Mathematics and drivers required for operation
and functionality of the IR sensor.

**********************************************************/
#ifndef IR_H
#define IR_H

/**** INCLUDES ****/
#include "../drivers/hitechnic-irseeker-v2.h" //IR seeker drivers

/**** Globals ****/
string ir_filename = "irlog.txt"; // the name of our file

/**** READING OPERATIONS ****/
static int ir_logid = -1;
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
	IRavgstore[sensor][indexavg[sensor]] = new;

	//Calculate Average
	int avg = 0;
	for (int i=0; i<samples; i++)
	{
		avg += (int)IRavgstore[sensor][i];
	}
	avg = (int)(avg/samples);

	//Store Average
	IRavg[sensor] = avg;
	indexavg[sensor]++;
}

/**
* File Setup
* Setup File Handles for Logging
*/
void ir_setupLogging()
{
	log_init(ir_filename, false, ir_logid);
	log_write("IR0,IR1,IR2,IR3,IR4,AVG0,AVG1,AVG2,AVG3,AVG4", ir_logid);
}

void logValues(bool isdecision,int ir0,int ir1,int ir2,int ir3,int ir4,int avg0,int avg1,int avg2,int avg3,int avg4){
	if (!log_enabled) { return; }
	string s = "";
	//IR0
	StringFormat(s,"%i,",ir0);
	log_write(s, ir_logid);
	//IR1
	StringFormat(s,"%i,",ir1);
	log_write(s, ir_logid);
	//IR2
	StringFormat(s,"%i,",ir2);
	log_write(s, ir_logid);
	//IR3
	StringFormat(s,"%i,",ir3);
	log_write(s, ir_logid);
	//IR4
	StringFormat(s,"%i,",ir4);
	log_write(s, ir_logid);
	//AVG0
	StringFormat(s,"%i,",avg0);
	log_write(s, ir_logid);
	//AVG1
	StringFormat(s,"%i,",avg1);
	log_write(s, ir_logid);
	//AVG2
	StringFormat(s,"%i,",avg2);
	log_write(s, ir_logid);
	//AVG3
	StringFormat(s,"%i,",avg3);
	log_write(s, ir_logid);
	//AVG4
	if (isdecision)
	{
		StringFormat(s,"%iDECISION\r\n",avg4);
		log_write(s, ir_logid);
	}
	else
	{
		StringFormat(s,"%i\r\n",avg4);
		log_write(s, ir_logid);
	}
}


/**
* Initialize the IR sensor for reading.
**/
void ir_init_internal(tSensors ir)
{
	ir_sensor = ir;
	ir_reset();
	if (log_enabled) {
        ir_setupLogging();
    }
}

/**
* Update IR
*/
void updateIR(){
  //Read
	HTIRS2readAllACStrength(ir_sensor, IRraw[0], IRraw[1], IRraw[2], IRraw[3], IRraw[4]);
	//Move Averages
	ir_moveavg(0,IRraw[0]);
	ir_moveavg(1,IRraw[1]);
	ir_moveavg(2,IRraw[2]);
	ir_moveavg(3,IRraw[3]);
	ir_moveavg(4,IRraw[4]);
	//Log
	if (log_enabled){
	    logValues(false, IRraw[0],IRraw[1],IRraw[2],IRraw[3],IRraw[4],IRavg[0],IRavg[1],IRavg[2],IRavg[3],IRavg[4]);
    }
	samplecount++;
	if (samplecount > 1022) { samplecount = 0; }
}

#endif
