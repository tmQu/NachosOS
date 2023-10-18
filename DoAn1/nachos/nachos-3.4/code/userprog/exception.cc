// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MaxFileLength 32
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

/*
    increase the program counter to move to the next instruction

*/
void increaseProgramCounter() {
	int reg = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, reg); // for debugging, in case we
						// are jumping into lala-land
	reg = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, reg); //move to the next instruction
	machine->WriteRegister(NextPCReg, reg + 4); // move toward 4 bytes (the next next instruction)
}

/*
    INPUT 
        virAddr -> address of memory in user mode
        limit -> limit of bytes transfer
    
    OUTPUT
        return the array of bytes created in kernel mode
*/
char* User2System(int virtAddr, int limit)
{
	int i; 
	int oneChar; // oneChar is integer bc ReadMem use integer argument
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // + 1 for null
	if (kernelBuf == NULL)
		return kernelBuf;
		
	memset(kernelBuf, 0, limit + 1);
	
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
        // Read one byte starting from virtAddr
		// Return the value of said byte to oneChar

		kernelBuf[i] = (char)oneChar;
	}
	return kernelBuf;
}



/*
    INPUT
        virAddr -> address of memory in user mode
        len -> len of bytes transfer
        buffer -> addr of data tranfer from kernel mode to user mode
    OUTPUT
        return the number of byte transferd
        or -1 if errors
*/
int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) 
        return -1;

	if (len == 0)
        return len;

	int i = 0;
	int oneChar = 0; // oneChar is integer bc WriteMem use integer argument
	do{
		oneChar = (int)buffer[i];

		machine->WriteMem(virtAddr + i, 1, oneChar);
        // Overwrite one byte to virtAddr + 1 with oneChar

		i++;
	} while (i < len);
	return i;
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch(which)
    {
        case NoException: // Everything ok return the control for the OS
            return;

        case PageFaultException:
            printf("No valid translation found\n");
            interrupt -> Halt();
            break;

        case ReadOnlyException:
            printf(" Write attempted to page marked read-only\n");
            interrupt -> Halt();
            break;

        case BusErrorException:
            printf("Translation resulted in an invalid physical address\n");
            interrupt -> Halt();
            break;

        case AddressErrorException:
            printf("Unaligned reference or one that was beyond the end of the address space\n");
            interrupt -> Halt();
            break;

        case OverflowException:
            printf("Integer overflow in add or sub.\n");
            interrupt -> Halt();
            break;

        case IllegalInstrException:
            printf("Unimplemented or reserved instr.\n");
            interrupt -> Halt();
            break;

        case NumExceptionTypes:
            printf("Number of exception type\n");
            interrupt -> Halt();
            break;

        case SyscallException:
            switch(type)
            {
                
                case SC_Halt:
                {
                    /*
                        Function: Halt the OS
                    */
			        DEBUG('a', "\nShutdown, initiated by user program. ");
			        printf("\nShutdown, initiated by user program. ");
			        interrupt->Halt();
			        return;
                }

                
                case SC_Create:
                {
                    /*
                        INPUT
                            char *name = reg(4)
                        OUTPUT- reg(2)
                            -1 -> error
                            0  -> success
                        Function: Create File with the given name
                    */
                    int virtAddr;
		            char* fileName;
				
                    DEBUG('a', "\nSC_Create call...");
                    DEBUG('a', "\nReading virtual address of filename");
                    
                    // Read the addr of fileName from reg(4)
                    virtAddr = machine->ReadRegister(4);
                    DEBUG('a', "\nReading filename.");
                    
                    //Convert from user memory to system memory
                    fileName = User2System(virtAddr, MaxFileLength + 1);
                    
                    if(strlen(fileName) == 0) // the virAddr is invalid
                    {
                        printf("Invalid file name\n");
                        machine->WriteRegister(2, -1); 
          
                        delete fileName;
                        break; // use break to increaseProgramCounter

                    }

                    if (fileName == NULL) {
                        printf("\nNot enough memory in system\n");
                        DEBUG('a', "\nNot enough memory in system\n");
                        
                        machine->WriteRegister(2, -1); 
          
                        delete fileName;
                        break; // use break to increaseProgramCounter
                    }                
                    DEBUG('a', "\nFinish reading filename.");
                    
                    // Create file with size = 0
                    // Using fileSystem from class OpenFile to create file
                    // Which uses Linux's way to create file
                    
                    // Check the condition
                    if(fTable -> isFileExists(fileName) == TRUE)
                    {
                        printf("\nFile exist, delete before create\n");
                        DEBUG('a', "\nFile exist, delete before create\n");
                        
                        machine->WriteRegister(2, -1);
                        delete fileName;
                        break;                        
                    }
                    if (!fileSystem->Create(fileName, 0)) {
                        printf("\nError creating file '%s'", fileName);
                        machine->WriteRegister(2, -1);
                        delete fileName;
                        break;
                    }
                    machine->WriteRegister(2, 0); // return success

                    delete fileName;
                    break;
                }

                case SC_Open:
                {
                    /*
                        INPUT
                            char *name - reg(4)
                            int typeFile - reg(5) {0 - WriteRead, 1 - ReadOnly}
                        OUTPUT - reg(2)
                            OpenFIleId
                            -1 - errors
                        
                        Function: Open the file with the given name in 2 mode ReadOnly or ReadWrite
                    */
                    
                    // get the FileName
                    int virAddr = machine -> ReadRegister(4);
                    char *fileName = User2System(virAddr, MaxFileLength + 1);

                    if(strlen(fileName) == 0) // the virAddr is invalid
                    {
                        printf("Invalid file name\n");
                        machine->WriteRegister(2, -1); 
          
                        delete fileName;
                        break; // use break to increaseProgramCounter

                    }

                    if (fileName == NULL) {
                        printf("\nNot enough memory in system");
                        DEBUG('a', "\nNot enough memory in system");
                        
                        machine->WriteRegister(2, -1); 
          
                        delete fileName;
                        break; // use break to increaseProgramCounter
                    }
                    
                    DEBUG('a', "\nFinish reading filename.");

                    // get the type of file
                    int typeFile = machine -> ReadRegister(5);

                    OpenFileId id = fTable -> Open(fileName, typeFile);
                    machine -> WriteRegister(2, id); delete fileName;
                    break;
                }

                case SC_Close:
                {
                    /*
                        INPUT
                            OpenFileId id - reg(4)
                        OUTPUT - reg(2)
                            0 - success
                            -1 - unsuccess
                        FUNCTION: close the file with the given fileID
                    */

                    OpenFileId id = machine -> ReadRegister(4);
                    int res = fTable -> Close(id);
                    machine -> WriteRegister(2, res);
                    break;
                }

                case SC_Read:
                {
                    /*
                        INPUT
                            char *buffer - reg(4) -> store the read bytes
                            int charCount - reg(5) -> Allow to read 'charCount' bytes
                            OpenFileId id
                        OUTPUT - reg(2)
                            -1 -> error
                            -2 -> end of file
                            number of readByte
                        FUNCTION: Attempt to read 'charCount' bytes and store in buffer, return the number of bytes read
                    */
                    int virAddr = machine -> ReadRegister(4); // addr of buffer
                    int charCount = machine -> ReadRegister(5);
                    OpenFileId id = machine -> ReadRegister(6); // id of file in table
					
                    char *buffer = new char[charCount]; // create the temp memory in kernel mode
					memset(buffer, 0, charCount);	
                    int readByte = fTable -> Read(buffer, charCount, id);

                    if(readByte > 0)
                    {
                        System2User(virAddr, charCount, buffer);
                    }
                    DEBUG('a', "\nRead byte %d\n", readByte);
                    machine -> WriteRegister(2, readByte);
                    delete buffer;
                    break;
                }

                case SC_Write:
                {
                    /*
                        INPUT
                            char *buffer - reg(4) -> addr of data needs to be written
                            int charCount - reg(5) -> Allow to write'charCount' bytes
                            OpenFileId id
                        OUTPUT - reg(2)
                            -1 -> error
                            -2 -> end of file cannot write anny more
                            number of writeByte
                        FUNCTION: Attempt to write 'charCount' bytes, return the number of actually written byte
                    */ 
                    int virAddr = machine -> ReadRegister(4); // addr of buffer
                    int charCount = machine -> ReadRegister(5);
                    OpenFileId id = machine -> ReadRegister(6); // id of file in table

                    char *buffer = User2System(virAddr, charCount);
                    int writeByte = fTable -> Write(buffer, charCount, id);
                    DEBUG('a', "\nWrite byte %d\n", writeByte);
                    machine -> WriteRegister(2, writeByte);
                    delete buffer;
                    break;
                }

                case SC_Seek:
                {
                    /*
                        INPUT
                            int pos - reg(4) //move the cursor to the pos
                            OpenFileId id - reg(5)
                        OUTPUT
                            return the pos if seek success 
                            or -1 if errors (seek on Console will return errors)
                    */
                    int pos = machine -> ReadRegister(4);
                    OpenFileId id = machine -> ReadRegister(5);
                    int res = fTable -> Seek(pos, id);
                    machine -> WriteRegister(2, res);
                    break;
                }

                case SC_Delete:
                {
                    /*
                        INPUT 
                            char *fileName - reg(4)
                        Output
                            return 0 if success
                            return -1 if not success (if the file is opening, Delete Syscall cannot be done and return errors )
                    */
                    int virAddr = machine -> ReadRegister(4);
                    char *fileName = User2System(virAddr, MaxFileLength + 1);

                    if(strlen(fileName) == 0) // the virAddr is invalid
                    {
                        printf("Invalid file name\n");
                        machine->WriteRegister(2, -1); 
          
                        delete fileName;
                        break; // use break to increaseProgramCounter

                    }

                    if (fileName == NULL) {
                        printf("\nNot enough memory in system");
                        DEBUG('a', "\nNot enough memory in system");
                        
                        machine->WriteRegister(2, -1); 
          
                        delete fileName;
                        break; // use break to increaseProgramCounter
                    }

                    if(fTable -> isOpen(fileName) == TRUE)
                    {
                        printf("Cannot delete the file because it's opening!!!\n");
                        machine -> WriteRegister(2, -1);
                        delete fileName;
                        break;
                    }

                    if(fileSystem -> Remove(fileName))
                    {
                        machine -> WriteRegister(2, 0);
                    }
                    else
                    {
                        machine -> WriteRegister(2, -1);
                    }
                    delete fileName;
                    break;

                }

            }
            increaseProgramCounter();
            break;
        default:
        	printf("Unexpected user mode exception %d %d\n", which, type);
	        ASSERT(FALSE);   
    }
}
