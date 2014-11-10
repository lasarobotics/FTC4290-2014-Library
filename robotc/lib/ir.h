/**********************************************************

IR Operation
ir.h

Mathematics and drivers required for operation
and functionality of the IR sensor.

**********************************************************/

/**** INCLUDES ****/
#include "../../drivers/hitechnic-irseeker-v2.h" //IR seeker drivers

/**** READING OPERATIONS ****/
static tSensors ir_sensor;
static int ir_thresholds[10]; //min, max
static bool ir_read = true;

/**** MOVING AVERAGE VARS ****/
const int samples = 5; // moving average sample size
int indexavg = 0;       // current index
int IRavg[5][samples];  // sample list to contain ir averages

/**
 * Reset IR thresholds and average values to default values for next reading operation.
 **/
void ir_reset()
{
  for (int i=0; i<5; i++)
  {
    ir_thresholds[2*i] = -999; //min
    ir_thresholds[3*i+1] = 999; //max
  }
  for (int i=0; i<5; i++)
  {
    for (int j=0; j<samples;j++)
	  {
	    IRavg[i][j] = 0;
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
 * Stop current IR reading operation.
 **/
void ir_stopRead()
{
  ir_read = false;
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
 * @param Last data point
 * @param Returns current average
 */
void ir_moveavg(int sensor, int last, float& avg)
{
  if (indexavg == samples){
    indexavg = 0;
  }
  IRavg[sensor][indexavg] = last;
  avg = 0;
  for (int i=0; i<samples; i++)
  {
    avg += (float)IRavg[sensor][i];
  }
  avg = avg/samples;
  indexavg++;
  return;
}
