/**********************************************************

I2C Error Checking
i2c.h

Reads sensor ports and finds if devices are missing.

**********************************************************/

typedef enum {
MOTORCON = 0,
SERVOCON = 1,
TOUCH = 2,
LIGHT = 3,
PSPV4 = 4
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
* @param The first 8 characters of the name of the expected sensor.
* @return Returns whether the sensor exists or not. True if no error.
*/
bool errorcheck(int port, int address, device sensor)
{
    //Local vars
    const int responsesize = 8;
    sbyte I2Cmessage[4] = { 0x00,0x00,0x00,0x00 };
    sbyte I2Creply[8] =  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    TSensorTypes prevtype;
    string expected = "";
    error_translate(sensor, expected);

    //Setup port
    switch (port){
    case 1:
        prevtype = SensorType[S1];
        SensorType[S1] = sensorI2CCustom;
        break;
    case 2:
        prevtype = SensorType[S2];
        SensorType[S2] = sensorI2CCustom;
        break;
    case 3:
        prevtype = SensorType[S3];
        SensorType[S3] = sensorI2CCustom;
        break;
    case 4:
        prevtype = SensorType[S4];
        SensorType[S4] = sensorI2CCustom;
        break;
    }

    wait10Msec(5);
    I2Cmessage[0] = 0x02; //Request flag
    I2Cmessage[1] = address * 2; //Address
    //Request starts at byte 10, where sensor name is stored
    I2Cmessage[2] = 0x10;
    switch (port){
    case 1:
        //Send message
        sendI2CMsg(S1, &I2Cmessage[0], responsesize);
        //Wait while message processed
        while (nI2CStatus[S1] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        //Read reply
        readI2CReply(S1, &I2Creply[0], responsesize);
        break;
    case 2:
        sendI2CMsg(S2, &I2Cmessage[0], responsesize);
        while (nI2CStatus[S2] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        readI2CReply(S2, &I2Creply[0], responsesize);
        break;
    case 3:
        sendI2CMsg(S3, &I2Cmessage[0], responsesize);
        while (nI2CStatus[S3] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        readI2CReply(S3, &I2Creply[0], responsesize);
        break;
    case 4:
        sendI2CMsg(S4, &I2Cmessage[0], responsesize);
        while (nI2CStatus[S4] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        readI2CReply(S4, &I2Creply[0], responsesize);
        break;
    }
    //get string returned
    string display = "";
    int convArray[responsesize];
    //convert array to int array
    for (int i=0; i<responsesize; ++i){
        convArray[i] = I2Creply[i];
        display = display + (char)convArray[i];
    }

    //reset sensor settings
    switch (port){
    case 1:
        SensorType[S1] = prevtype;
        break;
    case 2:
        SensorType[S2] = prevtype;
        break;
    case 3:
        SensorType[S3] = prevtype;
        break;
    case 4:
        SensorType[S4] = prevtype;
        break;
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
