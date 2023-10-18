/*
This header file will contains uitility function (such as printNum, print function)
*/

#include "syscall.h"

// print the integer
void printInteger(int num)
{
    int temp = num;
    int i = 1;
    char digit[1];

    if(temp < 0)
        temp = -temp;
    while(temp/10 > 0)
    {
        i *= 10;
        temp /= 10;
    }


    if (num  < 0){
        Write("-", 1, ConsoleOutput);
        num = -num;
    }
    while(i > 0)
    {
        digit[0] = (num / i) % 10 + 48;
        Write(digit, 1, ConsoleOutput);
        i /= 10;
    }
}


//print const string until NULL (but not print NULL byte) or 1000 bytes to console
int print(const char *str)
{
    int i = 0;
    int res = 0;
    while(str[i] != 0)
    {
        i++;
		if(i > 1000)
			break;
    }
    return Write(str, i, ConsoleOutput);
}

