/**********************************************************
Option Selection Display
options.h
Displays splash screen and enables custom diagnostics.
**********************************************************/

#include "display.h"

static const int maxoptions = 6; //maximum options count

static const int startline = 7 - maxoptions; //line of origin for options
static string options[maxoptions][maxoptions]; //[option index][choice index]
static char* optionnames[maxoptions]; //names of each option
static int optionscount = 0; //count of options (0-6)
static int choicecount[maxoptions]; //count of choices given to user
int options_get[maxoptions]; //indices of selected choices (PUBLIC!)

void options_reset()
{
	for (int i=0;	i<maxoptions; i++)
	{
		for (int j=0; j<maxoptions; j++)
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
  if (optionscount >= maxoptions) { return; }
	if (option > maxoptions - 1) { return; }
	if (option < 0) { return; }
	if (choicecount[option] >= maxoptions) { return; }

	int c = choicecount[option];
	options[option][c] = choice;
  choicecount[option]++;
}

void options_invert(int line)
{
  int max = display_y(line+startline+1)+1;
  int min = display_y(line+startline);
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
	nxtEraseLine(0, display_y(option+startline), 99, display_y(option+startline+1)-1);
	nxtDisplayTextLine(option+startline, optionnames[option]);
	string c = options[option][choice];
  nxtDisplayStringAt(display_xright(strlen(c)), display_y(option+startline), c);
	options_select(-1, option);
}

/**
 * Displays options immediately to the screen
 */
void options_display(char* title, char* confirmation)
{
  //Clear stuff
  diagnosticsOff();
  eraseDisplay();
  //Display caption and title
  nxtDisplayCenteredTextLine(0, title);
  nxtDisplayCenteredTextLine(7, confirmation);
  if (optionscount > maxoptions) { optionscount = maxoptions; }
  if (optionscount < 1) { return; }

  //Loop display default options
  for (int i=0; i<optionscount; i++)
  {
    nxtDisplayTextLine(i+startline, optionnames[i]);
    string d;
    StringFromChars(d, options[i][0]);
    nxtDisplayStringAt(display_xright(strlen(d)), display_y(i+startline), d);
  }

  //Display initial dot
  options_select(-1, 0);
  int selected = 0;
  int selchoices[maxoptions];
  for (int i=0; i<maxoptions; i++) { selchoices[i]=0; }

  //Wait for user input
  while (true)
  {
    //ENTER button - rotate the item cycle
    if (nNxtButtonPressed == 3)
    {
        // If we're selecting the "OK" button
        if (selected == maxoptions) {
          for (int i=0; i<maxoptions; i++)
          {
            options_get[i] = selchoices[i];
          }
          eraseDisplay();
          return;
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
      	wait10Msec(50);
    }
    //LEFT arrow - move up an item
    if ((nNxtButtonPressed == 2) && (selected > 0))
    {
        if (selected == maxoptions) //if we're on the OK button
        {
          options_select(selected, optionscount - 1);
          selected = optionscount - 1;
        }
        else {
          options_select(selected, selected - 1);
          selected--;
        }
        wait10Msec(25);
    }
    //RIGHT arrow - move down an item
    if ((nNxtButtonPressed == 1) && (selected <= optionscount - 1))
    {
        if (selected == optionscount - 1)
        {
          //if we are on the last item, select the "OK" button
          options_select(selected, maxoptions);
          selected = maxoptions;
        }
        else {
	        options_select(selected, selected + 1);
	        selected++;
        }
        wait10Msec(25);
    }
    wait1Msec(10);
  }
}
