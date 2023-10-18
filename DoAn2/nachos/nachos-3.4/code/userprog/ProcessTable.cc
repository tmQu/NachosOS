#include "ProcessTable.h"


extern void StartProcess(int addrFileName);

ProcessTable :: ProcessTable()
{
    pTable = new Thread*[NumPhysPages]; // the maximum number of process <= NumPhysPage
    pBitMap = new BitMap(NumPhysPages);

    // main process
    pBitMap -> Mark(0);
    pTable[0] = currentThread;
}


ProcessTable :: ~ProcessTable()
{
    // not delete each thread (element in pTable)
	// it is deleted when the next thread (come after it) is running
    delete pTable;
    delete pBitMap;
}


void ProcessTable :: cleanUpCurrentProcess()
{
    int id = currentThread -> getPid();
    if(id < 1)
        return;
    currentThread -> freeSpace();
    pBitMap -> Clear(id);
}

int ProcessTable :: createNewProcess(char *fileName, int priority)
{
    int freeSlot = pBitMap -> Find();
    
    if(freeSlot < 0)
    {
	    printf("No free slot in file process table\n");
        //DEBUG(debugChar, "No free slot in file process table\n");
        return -1;
    }
    char *pName = new char[strlen(fileName) + 1]; // create the new memory for pName and delete it in thread destructor
	strcpy(pName, fileName);
    pTable[freeSlot] = new Thread(pName, freeSlot, priority);

    int virtualAddr = (int) pName;
    pTable[freeSlot] -> ForkWithPriority(StartProcess, virtualAddr);

    return freeSlot;
}


