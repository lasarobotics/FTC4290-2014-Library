/**********************************************************

Display Functions
display.h

Displays splash screen and enables custom diagnostics.

**********************************************************/

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
* Displays a quick splash screen
* @param Program title to display on screen
* @param Status string to display, "" if none
* @param Version string to display, "" if none
*/
void displaySplash(const string title, const string statustext, const string versiontext)
{
    diagnosticsOff();
    nxtbarOff();
    eraseDisplay();
    wait10Msec(1);
    nxtDisplayRICFile(0, 5, "lasa.ric");
    nxtDisplayCenteredTextLine(5, "%s", title);
    nxtDisplayCenteredTextLine(6, "%s", statustext);
    nxtDisplayCenteredTextLine(7, "%s", versiontext);
    wait10Msec(250);
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
                if ((bat >= fCritVolt) && (fWarnVolt < 10.5)) { nxtDisplayStringAt(76, 47, "WARN"); }
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
