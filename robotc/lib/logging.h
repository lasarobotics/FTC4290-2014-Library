/**********************************************************

File Logging
logging.h

Allows rapidly writing to files on the NXT.

**********************************************************/
#ifndef _LOGGING_ENABLED
#define _LOGGING_ENABLED

/**** INCLUDES ****/
#include "../drivers/hitechnic-irseeker-v2.h" //IR seeker drivers

/**** Globals ****/
bool log_enabled = true; //enable logging
int logid = -1; //main log passed down literally everywhere

/**** File OPERATIONS ****/
static string filename[4] = { "", "", "", "" }; // the name of our files
static TFileHandle filehandle[4] = { NULL, NULL, NULL, NULL };
static TFileIOResult ioresult[4] = { SUCCESS, SUCCESS, SUCCESS, SUCCESS };
static int logcount = 0; //count of currently writing logs
static TSemaphore log_mutex[4];

static int filesize = 5000; // will store our file size

static bool log_failure(int i)
{
	if (i > 4) { return false; }
	if (ioresult[i] == SUCCESS) { return true; }
	Close(filehandle[i], ioresult[i]);
	if (ioresult[i] != SUCCESS) { return false; }
	return true;
}

/**
* Recieve last error
* Returns SUCCESS if no error
*/
TFileIOResult log_lasterror(int logid)
{
	if (logid > 3) { return SUCCESS; }
	return ioresult[logid];
}

/**
* File Setup
* Setup File Handles for Logging
*/
bool log_init(const string file, bool append, int& logid)
{
	if (!log_enabled) { return true; }
	if (logcount > 4) { logid = -1; return false; }
	int i = logcount;
	SemaphoreInitialize(log_mutex[i]);
	SemaphoreLock(log_mutex[i], 50);

	//Delete Previous File
	if (!append) {
		Delete(file, ioresult[i]); log_failure(i);
	}
	//open the file for writing (creates the file if it does not exist)
	OpenWrite(filehandle[i], ioresult[i], file, filesize);
	SemaphoreUnlock(log_mutex[i]);

	logid = logcount;
	logcount++;
	filename[i] = file;
	return log_failure(i);
}

/**
* Write a string to a log file
**/
bool log_write(const string text, int logid)
{
	if (!log_enabled) { return true; }
	if (logid > logcount - 1) { return false; }
	SemaphoreLock(log_mutex[logid], 50);
	WriteText(filehandle[logid], ioresult[logid], text);
	SemaphoreUnlock(log_mutex[logid]);
	return log_failure(logid);
}

/**
* Safely Stop Logging
**/
void log_stop()
{
	if (!log_enabled) { return; }
	log_enabled = false;
	for (int i=0; i<logcount; i++)
	{
		SemaphoreLock(log_mutex[i], 50);
		Close(filehandle[i], ioresult[i]);
		SemaphoreUnlock(log_mutex[i]);
	}
}

#endif
