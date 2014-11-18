/**********************************************************
Option Selection Display
options.h
Displays splash screen and enables custom diagnostics.
**********************************************************/

#include "display.h"

static string options[5][5]; //[option index][choice index]
static string optionnames[5]; //names of each option
static int optionscount = 0; //count of options (0-6)
static int choicecount[5]; //count of choices given to user
int options_get[5]; //indices of selected choices (PUBLIC!)

void options_reset()
{
	for (int i=0;	i<5; i++)
	{
		for (int j=0; i<5; j++)
		{
			options[i][j] = "";
		}
		choicecount[i] = 0;
		optionnames[i] = "";
		options_get[i] = 0;
	}
	 optionscount = 0;
}

void options_create(int option, string name)
{
  optionnames[option] = name;
}

//BE SURE TO START AT OPTION ZERO... OR EVERYTHING DIES
void options_additem(int option, char* choice)
{
  if (optionscount >= 5) { return; }
	if (option > 5) { return; }
	if (option < 1) { return; }
	if (choicecount[option] >= 5) { return; }

	int c = choicecount[option];
	options[option][c] = choice;
  if (c == 0) { optionscount++; }
  choicecount[option]++;
}

//old and new are OPTIONS (starting from zero)
void options_select(int old, int new)
{
  if (old != -1) { nxtInvertLine(0, display_y(old+2), 99, display_y(old+2+1)-1); }
  nxtInvertLine(0, display_y(new+2), 99, display_y(new+2+1)-1);
}

void options_redisplay(int option, int choice)
{
	nxtEraseLine(0, display_y(option+2), 99, display_y(option+2+1)-1);
	nxtDisplayTextLine(option, optionnames[option]);
	string c = options[option][choice];
  nxtDisplayStringAt(display_xright(strlen(c)), display_y(option+2), c);
	options_select(-1, option);
}

/**
 * Displays options immediately to the screen
 */
void options_display(string title, string confirmation)
{
  //Clear stuff
  diagnosticsOff();
  eraseDisplay();
  //Display caption and title
  nxtDisplayCenteredTextLine(0, title);
  nxtDisplayCenteredTextLine(7, confirmation);
  if (optionscount > 5) { optionscount = 5; }
  if (optionscount < 1) { return; }

  //Loop display default options
  for (int i=0; i<optionscount; i++)
  {
    nxtDisplayTextLine(i+2, optionnames[i]);
    string d = options[i][0]; //default choice
    nxtDisplayStringAt(display_xright(strlen(d)), display_y(i+2), d);
  }
  options_select(-1, 0);

  //Display initial dot
  options_select(-1, 0);
  int selected = 0;
  int selchoices[5] = { 0, 0, 0, 0, 0 };

  //Wait for user input
  while (true)
  {
    //ENTER button - rotate the item cycle
    if (nNxtButtonPressed == 3)
    {
        eraseDisplay();
        // If we're selecting the "OK" button
        if (selected == 6) { options_get = selchoices; return; }
        // If the optioncount available is greater than one, rotate through
        if (choicecount[selected] > 1)
        {
        	selchoices[selected]++;
        	if (selchoices[selected] >= choicecount[selected])
        	{ selchoices[selected] = 0; }

        	//Display the rotated choice
        	options_redisplay(selected, selchoices[selected]);
      	}
    }
    //LEFT arrow - move up an item
    if ((nNxtButtonPressed == 2) && (selected > 0))
    {
        options_select(selected, selected - 1);
        selected--;
    }
    //RIGHT arrow - move down an item
    if ((nNxtButtonPressed == 1) && (selected < optionscount - 1))
    {
        options_select(selected, selected + 1);
        selected++;
    }
    wait10Msec(20);
  }
}
