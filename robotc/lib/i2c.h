/**********************************************************

I2C Error Checking
i2c.h

Reads sensor ports and finds if devices are missing.

**********************************************************/
/***** INCLUDES *****/
#include "../drivers/hitechnic-sensormux.h" //Mux Driver

typedef enum {
MOTORCON = 0,
SERVOCON = 1,
TOUCH = 2,
LIGHT = 3,
PSPV4 = 4,
MUX = 5,
MUXAnalogue = 10,
MUXLegoUS = 11,
MUXCompass = 12,
MUXColor = 13,
MUXAccel = 14,
MUXIRSeeker = 15,
MUXProto = 16,
MUXColorNew = 17,
MUXAngle = 18,
MUXIRSeekerNew = 19
} device;
//TODO: Oksisane - Add other devices

/**
* Translates from int to string identifier of sensor
* @param int id of the sensor corresponding to enum in constants.h
*/
void error_translate(int sensor, string& expected){
    switch (sensor){
    case MOTORCON:
        expected = "MotorCon";
        break;
    case SERVOCON:
        expected = "ServoCon";
        break;
    case PSPV4:
        expected = "PSPNxV4";
        break;
    case MUX:
        expected = "SensrMux";
        break;
    //TODO: Oksisane - Add other devices
    default:
        expected = "None";
        break;
    }
}

/**
* Checks a robot component for connection.
* This function allows you to validate that a robot sensor is on and functioning.
* @param The expected port of the sensor on the NXT brick;
* @param The expected address of the sensor in the daisy chain. 1=first, 2=second, etc.
* @param The integer id corresponding to the name of the expected sensor.
* @param Port of the hitechnic multiplexer, if one exists. This value is only checked if port in the range 5-8
* @return Returns whether the sensor exists or not. True if no error.
*/
bool errorcheck(int port, int muxport, int address, device sensor)
{
    //Local vars
    const int responsesize = 8;
    sbyte I2Cmessage[4] = { 0x00,0x00,0x00,0x00 };
    sbyte I2Creply[8] =  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    TSensorTypes prevtype;
    string expected = "";
    error_translate(sensor, expected);
    //Setup port
    if (port < 5) {
        prevtype = SensorType[(port-1)];
        SensorType[(port-1)] = sensorI2CCustom;
    }
    wait10Msec(5);
    I2Cmessage[0] = 0x02; //Size of message
    I2Cmessage[1] = address * 2; //Address
    //Request starts at byte 10, where sensor name is stored
    if (port > 4){
        HTSMUXSensorType response;
        tMUXSensor checksensor = (muxport-1)*4 + (port - 5);
        response = HTSMUXreadSensorType(checksensor);
        if ((sensor-10) == response){
            return true;
        }
        else {
            return false;
        }
    }
    else {
        I2Cmessage[2] = 0x10;
        //Send message
        sendI2CMsg((port-1), &I2Cmessage[0], responsesize);
        //Wait while message processed
        while (nI2CStatus[(port-1)] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        //Read reply
        readI2CReply((port-1), &I2Creply[0], responsesize);
    }
    //get string returned
    string display = "";
    int convArray[responsesize];
    //convert array to int array
    for (int i=0; i<responsesize; ++i){
        convArray[i] = I2Creply[i];
        display = display + (char)convArray[i];
    }
    if (port < 5){
        //reset sensor settings
        SensorType[(port-1)] = prevtype;
    }

    //compare result to expected
    if (display == expected){
        return true;
    }
    else {
        return false;
    }
}

/**
* Converts boolean to "OK!" or "ERROR!"
* @param Is OK?  True for "OK!""
*/
void error_btos(bool ok, string& isok) {
    if (ok) { isok = "OK!"; }
    else { isok = "ERROR!"; }
}

/**
* Displays results of error check test
* @param Boolean array of test results (ex. errorcheck(1,1,MOTORCON))
* @param String array of test description, 11 chars max (ex. "MotorCon 1")
*/
bool error_display(bool* test, string* desc, int testcount)
{
    bool error = false;
    for (int i=0; i<testcount; i++)
    {
        if(test[i]==false){error = true;}
    }

    if (error)
    {
        string isok;
        for (int j=0; j<testcount; j++) {
            error_btos(test[j], isok);
            string c = (string)desc[j];
            nxtDisplayTextLine(j, "%s", c);
            int y = 63-(8*j);
            nxtDisplayStringAt(62, y, "%s", isok);
        }

        return false;
    }
    return true;
}
