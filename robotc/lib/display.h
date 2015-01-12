/**********************************************************

Display Functions
display.h

Displays splash screen and enables custom diagnostics.

**********************************************************/

#ifndef bDisplayDiagnostics
    #include "JoystickDriver.c"  //Joystick and Bluetooth driver
#endif

//Disables RobotC diagnostic screen
void diagnosticsOff() {
    StopTask(displayDiagnostics);
    bDisplayDiagnostics = false;
}
//Enables RobotC diagnostic screen
void diagnosticsOn() {

    StartTask(displayDiagnostics);
    bDisplayDiagnostics = true;
}
//Disable top NXT bar
void nxtbarOff() { bNxtLCDStatusDisplay = false; }
//Enable top NXT bar
void nxtbarOn() { bNxtLCDStatusDisplay = true; }
//Enable smart diagnostic screen
bool bSmartDiagnostics = true;
//Enable competition mode
bool bCompetitionMode = true;

//Warning external voltage level
float fWarnVolt = 10.5;
//Critical external voltage level
float fCritVolt = 8.8;

//Display functions
int display_y(int line)     { return 63 - (line*8); }
int display_x(int position) { return 6*position; }
int display_xright(int positionfromright) { return 99-(6*(positionfromright)); }

/**
* Loads current version number (from git)
*/
void getVersion(string &versionnumber)
{
    nxtDisplayRICFile(0, 0, "version.ric"); //Forces RobotC to load the file
    TFileHandle hFileHandle; // will keep track of our file
    TFileIOResult nIOResult; // will store our IO results
    string sFileName = "version.ric"; // the name of our file
    int nFileSize = 100; // will store our file size
    char CR = (char)13; // define CR (carriage return)
    char LF = (char)10; // define LF (line feed)
    char SPACE = (char)32; // define SPACE (space)
    char incommingChar; // this will store each char as we read back in from the file
    OpenRead(hFileHandle, nIOResult, sFileName, nFileSize); // open our file 'sFileName' for reading
    string astring = "git-"; // String to hold results

    int i = 0;
    int m = 0;
    //read first 8 chars of SHA
    for(i = 0; i < 6; i++) // iterate through the file until we've hit the end:
    {
        ReadByte(hFileHandle, nIOResult, incommingChar); // read in a single byte
        if(incommingChar == CR || incommingChar == LF) { }
            // do nothing
        else
        {
            astring = astring + incommingChar; //append char
        }
    }
    //finish reading SHA
    bool cont = true;
    while(cont)
    {
        ReadByte(hFileHandle, nIOResult, incommingChar); // read in a single byte
        i++;
        if(incommingChar == CR || incommingChar == LF) { cont = false; }
    }

    astring = astring + (char)'(';

    //Read revision ID
    for(m = i; m < nFileSize; m++) // iterate through the file until we've hit the end:
    {
        ReadByte(hFileHandle, nIOResult, incommingChar); // read in a single byte
        if(incommingChar == CR || incommingChar == LF || incommingChar == SPACE)
        {
            // if the incomming byte is a carriage return or a line feed (or space) do nothing
        }
        else
        {
            astring = astring + incommingChar;
            // append that byte (char) to the end of our final string, at the right index
        }
    }
    astring = astring + (char)')';

    Close(hFileHandle, nIOResult); // close our file
    versionnumber = astring; //set output
}
/**
* Displays a quick splash screen
* @param Program title to display on screen
* @param Status string to display, "" if none
* @param Version string to display, "" if none
*/
void displaySplash(const string title, const string statustext, bool displayversion)
{
    diagnosticsOff();
    nxtbarOff();
    eraseDisplay();
    wait10Msec(1);
    PlaySound(soundFastUpwardTones);
    nxtDisplayRICFile(0, 5, "lasa.ric");
    nxtDisplayCenteredTextLine(5, "%s", title);
    nxtDisplayCenteredTextLine(6, "%s", statustext);
    if (displayversion)
    {
        string versiontext;
        getVersion(versiontext);
        nxtDisplayCenteredTextLine(7, "%s", versiontext);
    }

    PlaySound(soundFastUpwardTones);
    wait10Msec(250);
    eraseDisplay();
}

//Displays smart diagnostic screen
task displaySmartDiags()
{
    diagnosticsOff();

    string sFileName, filename;
    getUserControlProgram(sFileName);
    if (sFileName == "") { filename = "TELEOP NOT SET!"; }
    else { StringFormat(filename, "%s.rxe", sFileName); }
    while (true)
    {
        if (bSmartDiagnostics)
        {
            //Update variables with current joystick values
            getJoystickSettings(joystick);

            //NXT Battery Levels
            float nbat = (float)nAvgBatteryLevel / (float)1000;
            nxtDisplayTextLine(3, "NXT :%4.1fV", nbat);
            if (nbat >= 7.5) { nxtDisplayStringAt(display_xright(2), display_y(3), "OK"); }
            if ((nbat < 7.5) && (nbat > 6.8)) { nxtDisplayStringAt(display_xright(4), display_y(3), "WARN"); }
            if (nbat <= 6.8) { nxtDisplayStringAt(display_xright(4), display_y(3), "CRIT"); }

            if (externalBatteryAvg < 0)
            {
                nxtDisplayTextLine(2, "NO BATTERY!");
            }
            else
            {
                float bat = externalBatteryAvg / (float)1000;
                nxtDisplayTextLine(2, "Batt:%4.1fV", bat);
                if (bat >= fWarnVolt) { nxtDisplayStringAt(display_xright(2), display_y(2), "OK"); }
                if ((bat >= fCritVolt) && (bat < fWarnVolt)) { nxtDisplayStringAt(display_xright(4), display_y(2), "WARN"); }
                if (bat < fCritVolt) { nxtDisplayStringAt(display_xright(4), display_y(2), "CRIT");}
            }
            if (bCompetitionMode)
            {
                nxtbarOff();
                nxtDisplayClearTextLine(0);
                nxtDisplayTextLine(1, "%s", filename);
                if (joystick.StopPgm)
                nxtDisplayCenteredTextLine(0, "--- DISABLED ---");
                else if (joystick.UserMode)
                nxtDisplayCenteredTextLine(0, "=== ENABLED ===");
                else
                nxtDisplayCenteredTextLine(0, "=== AUTO ===");
            }
        }

        abortTimeslice();
        wait1Msec(2000);
    }
}
