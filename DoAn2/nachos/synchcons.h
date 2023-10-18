// CAE - MULTI - SYNCHCONSOLE DEFINITION

#ifndef SYNCH_CONSOLE_H
#define SYNCH_CONSOLE_H
#include "console.h"

class SynchConsole {
public:
	SynchConsole();				// A SynchConsole Constructor
	SynchConsole(char *in, char *out);	// Same with fn pointers
	~SynchConsole();			// Delete a console instance

	int Read(char *into, int numBytes);	// Read synch line
						// Ends in EOLN or ^A
	int Write(char *from, int numBytes);	// Write a synchronous line
private:
	Console		*cons;			// Pointer to an async console
} ;
#endif

// CAE - MULTI - END SECTION

