/*
    ProcessTable only manage the process which manage to run the user program
*/
#include "system.h"

#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H
class ProcessTable
{
private:
    Thread ** pTable;
    BitMap * pBitMap;
public:
    ProcessTable();

    ~ProcessTable();

    int createNewProcess(char *fileName, int priority);
    
    void cleanUpCurrentProcess();
};

#endif
