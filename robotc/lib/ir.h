/**********************************************************

IR Operation
ir.h

Mathematics and drivers required for operation
and functionality of the IR sensor.

**********************************************************/

/**** INCLUDES ****/
#include "../../drivers/hitechnic-irseeker-v2.h" //IR seeker drivers

/**** File OPERATIONS ****/
string filename = "lastirlog.txt"; // the name of our file
int filesize = 5000; // will store our file size
TFileHandle   filehandle;
TFileIOResult ioresult;

/**** READING OPERATIONS ****/
static tSensors ir_sensor = 0;
static int ir_thresholds[10]; //min, max
static bool ir_read = true;
int IRraw[5];
bool loggingEnabled = true; //Enable Logging

/**** MOVING AVERAGE VARS ****/
const int samples = 5; // moving average sample size
int indexavg[5];       // current index for each sensor
int IRavgstore[5][samples];  // sample list to contain ir averages
int IRavg[5];//Current IR Averages

/***** QUICK METHODS *****/
//Get current raw IR value
float ir_getraw(int sensor) { return IRraw[sensor]; }
//Get current avg IR value from the last <samples> samples
float ir_getavg(int sensor) { return IRavg[sensor]; }
//Stop current IR reading operation.
void ir_stopRead(){ir_read = false;}
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
    ir_thresholds[2*i] = -999; //min
    ir_thresholds[3*i+1] = 999; //max
  }
  for (int i=0; i<5; i++)
  {
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
 * Set IR thresholds for next reading operation.
 * @param A sensor on the IR, between 1 and 5
 * @param Minimum value, between 0 and 255, or -1
 * @param Maximum value, between 0 and 255. or -1
 **/
void ir_setThreshold(int sensor, int min, int max)
{
  if (min > max) { return; }
  if (min != -1) { ir_thresholds[2*(sensor-1)] = min; }
  else           { ir_thresholds[2*(sensor-1)] = 999; }
  if (max != -1) { ir_thresholds[2*(sensor-1)+1] = max; }
  else           { ir_thresholds[2*(sensor-1)+1] = 999; }
}
/**
 * Read IR sensor until the sensor's level reaches a specific threshold.
 **/

 /////******************** TOOOOOOODOOOOOOOOOO ********************************

/**
 * Moving Average
 * @param Sensor (1-5)
 * @param New data point
 * @param Returns current average
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
  int avg = 0;
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
void setupLogging(){
    //Delete Previous File
    Delete(filename,ioresult);
	// open the file for writing (creates the file if it does not exist)
    OpenWrite(filehandle, ioresult, filename, filesize);
    WriteText(filehandle, ioresult, "IR0,IR1,IR2,IR3,IR4,AVG0,AVG1,AVG2,AVG3,AVG4\r\n");
}
void logValues(int ir0,int ir1,int ir2,int ir3,int ir4,int avg0,int avg1,int avg2,int avg3,int avg4){
    string s = "";
    //IR0
    StringFormat(s,"%i",ir0);
    WriteText(filehandle, ioresult, s);
    //IR1
    StringFormat(s,"%i",ir1);
    WriteText(filehandle, ioresult, s);
    //IR2
    StringFormat(s,"%i",ir2);
    WriteText(filehandle, ioresult, s);
    //IR3
    StringFormat(s,"%i",ir3);
    WriteText(filehandle, ioresult, s);
    //IR4
    StringFormat(s,"%i",ir4);
    WriteText(filehandle, ioresult, s);
    //AVG0
    StringFormat(s,"%i",avg0);
    WriteText(filehandle, ioresult, s);
    //AVG1
    StringFormat(s,"%i",avg1);
    WriteText(filehandle, ioresult, s);
    //AVG2
    StringFormat(s,"%i",avg2);
    WriteText(filehandle, ioresult, s);
    //AVG3
    StringFormat(s,"%i",avg3);
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
    if (loggingEnabled){
        setupLogging();
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
        if (loggingEnabled){
            logValues(IRraw[0],IRraw[1],IRraw[2],IRraw[3],IRraw[4],IRavg[0],IRavg[1],IRavg[2],IRavg[3],IRavg[4]);
        }
    }
}
