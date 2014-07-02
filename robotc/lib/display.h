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
/**
* Displays a quick splash screen
* @param Program title to display on screen
* @param Version string to display, "" if none
*/
void displaySplash(const string title, const string versiontext)
{
    diagnosticsOff();
    nxtbarOff();
    eraseDisplay();
    wait10Msec(1);
    nxtDisplayRICFile(0, 5, "lasa.ric");
    nxtDisplayCenteredTextLine(5, "%s", title);
    nxtDisplayCenteredTextLine(6, "%s", versiontext);
    wait10Msec(250);
}
//Displays smart diagnostic screen
void displaySmartDiags()
{
    diagnosticsOff();
    nxtbarOn();
}
