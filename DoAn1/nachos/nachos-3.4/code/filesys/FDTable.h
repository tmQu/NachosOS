
#include "openfile.h"
#include "bitmap.h"
#include "system.h"

#ifndef FDTABLE_H
#define FDTABLE_H
//number of element in file descriptor table
#define NumFile 10

//Type of File
#define WriteRead 0
#define ReadOnly 1

//debug character
#define debugChar 'a'

typedef int OpenFileId;

class FileDecriptor
{

public:

	virtual  ~FileDecriptor(){}; // Destructor is not pure virtual bc we dont want to make 2 destructor for class ConsoleIn ConsoleOut
	virtual int Read(char *buffer, int charCount) = 0;
	virtual int Write(char *buffer, int charCount) = 0;

	virtual int Seek(int pos) = 0;
	virtual bool isFilePath(char *path) = 0;
};

class FReadOnly : public FileDecriptor
{
private:
	int seekPos; // store the pos of cursor
	OpenFile *file;
	char *filePath;
public:
	FReadOnly(OpenFile *f, char *fileName);
	~FReadOnly();
	
	int Read(char *buffer, int charCount);
	int Write(char* buffer, int charCount);

	int Seek(int pos);

	/*
		Check whether the "name" is the directory of this File
		if it is return TRUE
		if it is NOT return FALSE
	*/
	bool isFilePath(char *path)
	{
		return strcmp(filePath, path) == 0;
	}
};

class FWriteRead : public FileDecriptor
{
private:
	int seekPos; // store the pos of cursor
	OpenFile *file;
	char *filePath;
public:
	FWriteRead(OpenFile *f, char *fileName);
	~FWriteRead();
	
	int Read(char *buffer, int charCount);
	
	int Write(char *buffer, int charCount);

	int Seek(int pos);

	/*
		Check whether the "name" is the directory of this File
		if it is return TRUE
		if it is NOT return FALSE
	*/
	bool isFilePath(char *path)
	{
		return strcmp(filePath, path) == 0;
	}
};

class ConsoleIn : public FileDecriptor
{
public:
	
	int Read(char* buffer, int charCount);
	int Write(char* buffer, int charCount);
	int Seek(int pos)
	{
		printf("Cannot seek in console!!!\n");
		return -1;
	}

	/*
		Console not have Directory return false
	*/
	bool isFilePath(char *path)
	{
		return FALSE;
	}
};


class ConsoleOut : public FileDecriptor // console
{
public:
	int Write(char* buffer, int charCount);
	int Read(char* buffer, int charCount);
	int Seek(int pos)
	{
		printf("Cannot seek in console!!!\n");
		return -1;
	}

	/*
		Console not have Directory return false
	*/
	bool isFilePath(char *path)
	{
		return FALSE;
	}
};


class FDTable
{
private:
	FileDecriptor **table; //the file table
	
	BitMap *bm; //if bitmap[i] on, the table[i] in use

public:
	FDTable ();
	~FDTable();

	/*
	return FALSE if id invalid 
	return TRUE if id is used (table[id] have stored a file)
	*/
	bool checkOpenFileID(OpenFileId id);

	int Open(char *name, int type);
	int Close(OpenFileId id);


	// Read
	int Read(char *buffer, int charCount, OpenFileId id);
	
	// Write
	int Write(char *buffer, int charCount, OpenFileId id);


	//Seek
	int Seek(int pos, OpenFileId id);

	/*
		Check whether the file is openning with the given FileName
		return TRUE if OPEN
		return FALSE if not open
	*/
	bool isOpen(char *fileName);
	bool isFileExists(char *fileName);
};
#endif //FDTABLE_H
