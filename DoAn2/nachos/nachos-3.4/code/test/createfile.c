#include "MyUtility.h"

int main()
{
	char fileName[32];
	int res = 0;
	print("================CreateFile program=====================\n\n");
	print("Input the file name: ");
	Read(fileName, 32, ConsoleInput); // read the file name from console
	res = Create(fileName); // store the result

	if(res == -1)
	{
		print("Create file not successfully\n");
	} 
	else
		print("Create file successfully\n");
	Halt();
}
