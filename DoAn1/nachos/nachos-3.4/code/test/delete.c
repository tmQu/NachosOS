#include "MyUtility.h"

int main()
{
	char fileName[32];

	print("------------------Delete program------------------\n\n");
	print("File name: ");
	Read(fileName, 32, ConsoleInput);
	if(Delete(fileName) == -1)
	{
		print("Cannot delete the file");
	}
	else
	{
		print("Delete the file successfully");
	}

}
