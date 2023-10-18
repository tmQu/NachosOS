#include "FDTable.h"


//-----------------------------------------FDTable---------------------------------------
FDTable :: FDTable ()
{
    table = new FileDecriptor *[NumFile];
    
    // Mark all table as NULL
    for (int i = 0; i < NumFile; i++)
    {
        table[i] = NULL;
    }
    
    bm = new BitMap(NumFile);

    //initialize the console
    bm -> Mark(0); // table[0] = ConsoleIn
    bm -> Mark(1); // table[1] = ConsoleOut
    
    table[0] = new ConsoleIn;
    table[1] = new ConsoleOut;
}


FDTable :: ~FDTable()
{
    for (int i = 0; i < NumFile; i++)
    {
        if(bm -> Test(i))
            delete table[i];
    }
    delete table;
    delete bm;
	DEBUG(debugChar, "Delete FDTable\n");
}


/*
return FALSE if id invalid 
return TRUE if id is used
*/
bool FDTable :: checkOpenFileID(OpenFileId id)
{
    if(id < 0 || id > NumFile) 
    {
        printf("Invalid OpenFileId\n");
        return FALSE;
    }

    if(bm -> Test(id) == FALSE)
    {
        printf("OpenFileId not in used\n");
        return FALSE;
    }
    
    return TRUE;
}

//Open the file
int FDTable :: Open(char *name, int type)
{
	if(type != WriteRead && type != ReadOnly)
    {
        printf("Invalid Type!!!\n");
        return -1;
    }
    
    // Find free slot
    int freeSlot = bm -> Find();
    
    if(freeSlot < 0)
    {
	printf("No free slot in file descriptor table\n");
        //DEBUG(debugChar, "No free slot in file descriptor table\n");
        return -1;
    }
    
    OpenFile *f = fileSystem -> Open(name);
    if(f == NULL)
    {
        printf("The file not exists\n");
        return -1;
    }


    switch(type)
    {
        case WriteRead:
            table[freeSlot] = new FWriteRead(f, name);
            break;
        case ReadOnly:
            table[freeSlot] = new FReadOnly(f, name);
            break;
    }
    return freeSlot;
}


// Close the file
int FDTable :: Close(OpenFileId id)
{
    if(checkOpenFileID(id) == FALSE)
    {
        return -1;
    }

    if(id == 0 || id == 1) // Cannot close ConsoleIn/Out id = 0 || id = 1
    {
        printf("Cannot close the console\n");
        return -1;
    }

    bm -> Clear(id);
    delete table[id];
    table[id] = NULL;
    return 0;
}

// Read
int FDTable :: Read(char *buffer, int charCount, OpenFileId id) {
    if(checkOpenFileID(id) == FALSE)
    {
        return -1;
    }

    if(charCount < 0)
        return -1;

	if(charCount  == 0)
		return 0;
    return table[id]->Read(buffer, charCount);
}

// Write
int FDTable :: Write(char *buffer, int charCount, OpenFileId id) {
    if(checkOpenFileID(id) == FALSE)
    {
        return -1;
    }
    
    if(charCount < 0)
        return -1;

	if(charCount  == 0)
		return 0;

    return table[id]->Write(buffer, charCount);
}


int FDTable :: Seek(int pos, OpenFileId id)
{
    if(checkOpenFileID(id) == FALSE)
    {
        return -1;
    }   

    return table[id] -> Seek(pos);
}


bool FDTable :: isOpen(char *name)
{
    for (int i = 0; i < NumFile;  i++)
    {
        if(bm -> Test(i)) // check whether the table[i] in use
        {
            if(table[i] -> isFilePath(name) == TRUE)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

bool FDTable :: isFileExists(char *fileName)
{
    OpenFile *f = fileSystem -> Open(fileName);
    if(f == NULL)
    {
        DEBUG(debugChar, "The file not exists\n"); 
	// this function only use to check whether the file exists no need to print the error notice
	    delete f;
        return FALSE;
    }
    delete f;
    return TRUE;
}
//------------------------------------------FReadOnly-------------------------------------

FReadOnly :: FReadOnly(OpenFile *f, char *name)
{
    file = f;

    filePath = new char[strlen(name) + 1];
    strcpy(filePath, name);

    seekPos = 0;
}

FReadOnly :: ~FReadOnly()
{
    delete filePath;
    delete file;
	DEBUG(debugChar, "Delete FReadOnly\n");
}

int FReadOnly :: Read(char *buffer, int charCount)
{
    int readChar = file->ReadAt(buffer, charCount, seekPos); // ReadAt use read(2) in C linux
                                                            // return 0 if end of file
                                                            // return -1 if errors
                                                            // return number of readed bytes
    
    // End of file
    if (readChar == 0)
        return -2;
    // File read error
    else if (readChar < 0)
        return -1;

    // Success
    seekPos += readChar;

    return readChar;
}

int FReadOnly :: Write(char* buffer, int charCount)
{
	printf("Read only cannot write\n");
    return -1;
}

int FReadOnly :: Seek(int pos)
{
    int lengthOfFile = file -> Length();
    if (pos == -1) // move to the end of the file
    {
        seekPos = lengthOfFile;
        return seekPos;
    }

    if(pos < 0 || pos > lengthOfFile)
    {
        printf ("Cannot move to this position!!!\n");
        return -1;
    }

    seekPos = pos;
    return seekPos;
}



//------------------------------------------FWriteRead-------------------------------------

FWriteRead :: FWriteRead(OpenFile *f, char *name)
{
    filePath = new char[strlen(name) + 1];
    strcpy(filePath, name);

    seekPos = 0;

    file = f;
}

FWriteRead :: ~FWriteRead()
{
    delete file;
    delete filePath;
	DEBUG(debugChar, "Delete FDTable\n");
}


int FWriteRead :: Read(char *buffer, int charCount)
{
    int readChar = file->ReadAt(buffer, charCount, seekPos); // ReadAt use read(2) in C linux
                                                            // return 0 if end of file
                                                            // return -1 if errors
                                                            // return number of readed bytes
    
    // End of file
    if (readChar == 0)
        return -2;
    // File read error
    else if (readChar < 0)
        return -1;

    // Success
    seekPos += readChar;

    return readChar;
}

int FWriteRead :: Write(char *buffer, int charCount)
{
    int writeChar = file->WriteAt(buffer, charCount, seekPos);
    


    // Error writing
    if(writeChar < 0)
        return -1;

    // end of file
    if(writeChar == 0)
        return -2;

    // Successfully writing, writeChar = 0 <=> cannot write
    seekPos += writeChar;
        
    return writeChar; 
}


int FWriteRead :: Seek(int pos)
{
    int lengthOfFile = file -> Length();
    if (pos == -1) // move to the end of the file
    {
        seekPos = lengthOfFile;
        return seekPos;
    }

    if(pos < 0 || pos > lengthOfFile)
    {
        printf ("Cannot move to this position!!!\n");
        return -1;
    }

    seekPos = pos;
    return seekPos;
}

//--------------------------------ConsoleIn----------------------------
int ConsoleIn :: Read(char* buffer, int charCount) {
    int readChar = gSynchConsole->Read(buffer, charCount);
    
    // End of file
    if (readChar == 0)
        return -2;
    // File read error
    else if (readChar < 0)
        return -1;

    // Return the amount of bytes read or -1 if error
    return readChar;
}

int ConsoleIn :: Write(char* buffer, int charCount) {
    printf("Console Input cannot write\n");
    return -1;
}



//-----------------------------ConsoleOut-----------------------------
int ConsoleOut :: Write(char* buffer, int charCount)
{
    int writeChar = gSynchConsole -> Write(buffer, charCount);

    // Error writing
    if(writeChar < 0)
        return -1;

    // end of file
    if(writeChar == 0)
        return -2;

    // Return the amount of bytes read or -1 if error
    return writeChar; 
}

int ConsoleOut :: Read(char* buffer, int charCount)
{
    printf("Console Output cannot read\n");
    return -1;
}
