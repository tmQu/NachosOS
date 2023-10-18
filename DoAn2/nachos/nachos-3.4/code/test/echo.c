#include "MyUtility.h"


int main()
{
	char buffer[2];
	int readByte = 0; // store the return values of Read() function
	int header = 0; // to print the "\nWhat you have input\n\n" once time
	print("---------------------ECHO-------------------\n\n");
	print("Input the string\n");
	print("--------------------------\n->");
	while(1)
	{
		readByte = Read(buffer, 1, ConsoleInput); // the charCount must be 1 -> when they press enter it will read only 'enter' byte and return -2
							  // if we pass in 'charCount', the Read function only return -2 when it read exactly [charCount byte] + 'enter' byte
		if(readByte == -1 || readByte == -2) // -1 -> erros, -2 -> when they press enter
		{
			break;
		}
		if(header == 0)
		{
			print("\nWhat you have input\n");
			print("--------------------------\n->");
			header = 1;
		}
		Write(buffer, readByte, ConsoleOutput);

	}
	return 0;
}

