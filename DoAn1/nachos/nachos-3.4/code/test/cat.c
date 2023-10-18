#include "MyUtility.h"

int main()
{
	char fileName[32];
	int readByte = 0;
	OpenFileId id;
	char buffer[100];
	print("------------------Cat program------------------\n\n");
	print("File name: ");
	Read(fileName, 32, ConsoleInput);
	id = Open(fileName, 1);
	if(id == -1)
	{
		return;
	}
	
	print("\nThe content\n");
	print("-------------------------------\n");
	while(1)
	{
		readByte = Read(buffer, 100, id);
		if(readByte == -1 || readByte == -2) // end of file or errors
			break;
		
		Write(buffer, readByte, ConsoleOutput);
	}
	print("\n-------------------------------");
	Close(id);
}
