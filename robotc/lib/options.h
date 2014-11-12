/**********************************************************

Option Selection Display
options.h

Displays splash screen and enables custom diagnostics.

**********************************************************/

#include "display.h"

int options_yvalue(int line)     { return 63 - (line*8); }
int options_xvalue(int position) { return 6*position; }

string options[8] = { "", "", "", "", "", "", "", "" };
int optionscount = 0;

void options_reset() { string o[8] = { "", "", "", "", "", "", "", "" }; options=o; optionscount = 0;}

void options_add(char* option)
{
  if (optionscount >= 7) { return; }
  options[optionscount] = option;
  optionscount++;
}

void options_displaydot(int old, int new, int addend)
{
  nxtDisplayStringAt(options_xvalue(0), options_yvalue(old+addend), " ");
  nxtDisplayStringAt(options_xvalue(0), options_yvalue(new+addend), ">");
}

/**
 * Displays options immediately to the screen
 * @returns Selected option index, or -1 on failure
 */
int options_DisplayList(string title, string caption, int defaultoption)
{
  //Clear stuff
  diagnosticsOff();
  eraseDisplay();
  //Display caption and title
  nxtDisplayCenteredTextLine(0, title);
  nxtDisplayTextLine(7, caption);
  if (optionscount > 7) { optionscount = 7; }
  if (optionscount < 1) { return -1; }

  //Loop display
  int addend = 2; //the first line is where?
  if (optionscount > 4) { addend = 1; }
  for (int i=0; i<optionscount; i++)
  {
    nxtDisplayTextLine(i+addend, "  %s", options[i]);
  }

  //Display initial dot
  nxtDisplayStringAt(options_xvalue(0), options_yvalue(defaultoption+addend), ">");
  int selected = defaultoption;

  //Wait for user input
  while (true)
  {
    //ENTER button
    if (nNxtButtonPressed == 3)
    {
        eraseDisplay();
        return selected;
    }
    //LEFT arrow
    if ((nNxtButtonPressed == 2) && (selected > 0))
    {
        options_displaydot(selected, selected - 1, addend);
        selected--;
    }
    //RIGHT arrow
    if ((nNxtButtonPressed == 1) && (selected < optionscount - 1))
    {
        options_displaydot(selected, selected + 1, addend);
        selected++;
    }
    wait10Msec(20);
  }
}

int options_DisplayInt(int defaultvalue, int min, int max, int increment)
{

}
