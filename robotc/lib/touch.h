/**********************************************************

IR Operation
ir.h

Mathematics and drivers required for operation
and functionality of the IR sensor.

**********************************************************/
#ifndef TOUCH_H
#define TOUCH_H

/**** INCLUDES ****/
#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/lego-touch.h"
#include "../lib/logging.h" //logging

/**** READING OPERATIONS ****/
static tSensors touch_sensor;
static tMUXSensor touch_sensor_mux;
bool touchval = false;
bool touchval2 = false;

const tMUXSensor touchSensorTwo = msensor_S3_4;
/***** QUICK METHODS *****/
//Get current raw touch value
bool get_touch(int sensor) {
  if (sensor == 1)
     return touchval;
   else
     return touchval2;
}


/**
* Initialize the IR sensor for reading.
**/
void touch_init_internal(tSensors touch)
{
	touch_sensor = touch;
}
void touch_init_internal(tMUXSensor touch)
{
	touch_sensor_mux = touch;
}
/**
* Update IR
*/
void updateTouch(){
  if(touch_sensor_mux != -1){
    touchval = TSreadState(touch_sensor_mux);
    touchval2 = TSreadState(touchSensorTwo);
  }
  else {
    touchval = TSreadState(touch_sensor);
  }
}

#endif
