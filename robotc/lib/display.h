/**********************************************************

Display Functions
display.h

Displays splash screen and enables custom diagnostics.

**********************************************************/

#ifndef bDisplayDiagnostics
#include "JoystickDriver.c"  //Joystick and Bluetooth driver
#endif

//Disables RobotC diagnostic screen
void diagnosticsOff() { bDisplayDiagnostics = false; }
//Enables RobotC diagnostic screen
void diagnosticsOn() { bDisplayDiagnostics = true; }
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
/**
* Loads current version number (from git)
*/
void getVersion(string &versionnumber)
{
    nxtDisplayRICFile(0, 5, "version.ric"); //Forces RobotC to load the file
    TFileHandle hFileHandle; // will keep track of our file
    TFileIOResult nIOResult; // will store our IO results
    string sFileName = "version.ric"; // the name of our file
    int nFileSize = 100; // will store our file size
    byte CR = 0x13; // define CR (carriage return)
    byte LF = 0x10; // define LF (line feed)
    char incommingChar; // this will store each char as we read back in from the file
    OpenRead(hFileHandle, nIOResult, sFileName, nFileSize); // open our file 'sFileName' for reading
    string astring = "Git Commit: "; // String to hold results
    for(int i = 0; i < nFileSize; i++) // iterate through the file until we've hit the end:
    {
        ReadByte(hFileHandle, nIOResult, incommingChar); // read in a single byte
        if(incommingChar == CR || incommingChar == LF || incommingChar == ' ')  {} // if the incomming byte is a carriage return or a line feed do nothing
        else
        {
            astring = astring + incommingChar; // append that byte (char) to the end of our final string, at the right index
        }
    }
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
    nxtDisplayRICFile(0, 5, "lasa.ric");
    nxtDisplayCenteredTextLine(5, "%s", title);
    nxtDisplayCenteredTextLine(6, "%s", statustext);
    if (displayversion)
    {
        string versiontext;
        getVersion(versiontext);
        nxtDisplayTextLine(7, "%s", versiontext);
    }

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

            if (externalBatteryAvg < 0)
            {
                nxtDisplayTextLine(2, "NO BATTERY!");
            }
            else
            {
                float bat = externalBatteryAvg / (float)1000;
                nxtDisplayTextLine(2, "Batt:%4.1fV", bat);
                if (bat >= fWarnVolt) { nxtDisplayStringAt(88, 47, "OK"); }
                if ((bat >= fCritVolt) && (bat < fWarnVolt)) { nxtDisplayStringAt(76, 47, "WARN"); }
                if (bat < fCritVolt) { nxtDisplayStringAt(76, 47, "CRIT");}
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
        wait1Msec(200);
    }
}
