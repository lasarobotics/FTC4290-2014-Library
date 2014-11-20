/**********************************************************
Option Selection Display
options.h
Displays splash screen and enables custom diagnostics.
**********************************************************/

#include "display.h"

static string options[5][5]; //[option index][choice index]
static char* optionnames[5]; //names of each option
static int optionscount = 0; //count of options (0-6)
static int choicecount[5]; //count of choices given to user
int options_get[5]; //indices of selected choices (PUBLIC!)

void options_reset()
{
	for (int i=0;	i<5; i++)
	{
		for (int j=0; j<5; j++)
		{
			options[i][j] = "";
		}
		choicecount[i] = 0;
		optionnames[i] = "";
		options_get[i] = 0;
	}
	 optionscount = 0;
}

void options_create(int option, char* name)
{
  optionnames[option] = name;
  optionscount++;
  return;
}

//BE SURE TO START AT OPTION ZERO... OR EVERYTHING DIES
void options_add(int option, char* choice)
{
  if (optionscount >= 5) { return; }
	if (option > 4) { return; }
	if (option < 0) { return; }
	if (choicecount[option] >= 5) { return; }

	int c = choicecount[option];
	options[option][c] = choice;
  choicecount[option]++;
}

void options_invert(int line)
{
  int max = display_y(line+2+1)+1;
  int min = display_y(line+2);
  for(int i=max; i<=min; i++)
  {
    nxtInvertLine(0, i, 99, i);
  }
}

//old and new are OPTIONS (starting from zero)
void options_select(int old, int new)
{
  if (old != -1) { options_invert(old); }
  options_invert(new);
}

void options_redisplay(int option, int choice)
{
	nxtEraseLine(0, display_y(option+2), 99, display_y(option+2+1)-1);
	nxtDisplayTextLine(option+2, optionnames[option]);
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
    string d;
    StringFromChars(d, options[i][0]);
    nxtDisplayStringAt(display_xright(strlen(d)), display_y(i+2), d);
  }

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
        // If we're selecting the "OK" button
        if (selected == 5) {
          options_get = selchoices; eraseDisplay(); return;
        }
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
        if (selected == 5)
        {
          options_select(selected, optionscount - 1);
          selected = optionscount - 1;
        }
        else {
          options_select(selected, selected - 1);
          selected--;
        }
    }
    //RIGHT arrow - move down an item
    if ((nNxtButtonPressed == 1) && (selected <= optionscount - 1))
    {
        if (selected == optionscount - 1)
        {
          //if we are on the last item, select the "OK" button
          options_select(selected, 5);
          selected = 5;
        }
        else {
	        options_select(selected, selected + 1);
	        selected++;
        }
    }
    wait10Msec(20);
  }
}
