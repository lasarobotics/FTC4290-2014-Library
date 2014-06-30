/**********************************************************

I2C Error Checking
i2c.h

Reads sensor ports and finds if devices are missing.

**********************************************************/

/**
* Checks a robot component for connection.
* This function allows you to validate that a robot sensor is on and functioning.
* @param The expected port of the sensor on the NXT brick;
* @param The expected address of the sensor in the daisy chain.  1=first, 2=second, etc.
* @param The first 8 characters of the name of the expected sensor.
* @return Returns whether the sensor exists or not.  True if no error.
*/
bool errorcheck (int port, int address, string &expected)
{
    //Local vars
    const int responsesize = 8;
    sbyte I2CmessageS1[4];
    sbyte I2CreplyS1[8];
    //Setup port
    switch (port){
    case 1:
        SensorType[S1] = sensorI2CCustom;
        break;
    case 2:
        SensorType[S1] = sensorI2CCustom;
        break;
    case 3:
        SensorType[S1] = sensorI2CCustom;
        break;
    case 4:
        SensorType[S1] = sensorI2CCustom;
        break;
    }

    wait10Msec(5);
    I2CmessageS1[0] = 0x02; //Request flag
    I2CmessageS1[1] = address * 2; //Address
    //Request starts at byte 10, where sensor name is stored
    I2CmessageS1[2] = 0x10;

    switch (port){
    case 1:
        //Send message
        sendI2CMsg(S1, &I2CmessageS1[0], responsesize);
        //Wait while message processed
        while (nI2CStatus[S1] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        //Read reply
        readI2CReply(S1, &I2CreplyS1[0], responsesize);
        break;
    case 2:
        sendI2CMsg(S2, &I2CmessageS1[0], responsesize);
        while (nI2CStatus[S2] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        readI2CReply(S2, &I2CreplyS1[0], responsesize);
        break;
    case 3:
        sendI2CMsg(S3, &I2CmessageS1[0], responsesize);
        while (nI2CStatus[S3] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        readI2CReply(S3, &I2CreplyS1[0], responsesize);
        break;
    case 4:
        sendI2CMsg(S4, &I2CmessageS1[0], responsesize);
        while (nI2CStatus[S4] == STAT_COMM_PENDING){
            wait1Msec(2);
        }
        readI2CReply(S4, &I2CreplyS1[0], responsesize);
        break;
    }
    //get string returned
    string display = "";
    int convArray[responsesize];
    //convert array to int array
    for (int i=0; i<responsesize; ++i){
        convArray[i] = I2CreplyS1[i];
        display = display + (char)convArray[i];
    }
    bool debug = false;
    //compare result to expected
    if (display == expected){
        return true;
    }
    else {
        return false;
    }
}
