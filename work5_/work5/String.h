#ifndef FILEDATA_H
#define FILEDATA_H

typedef struct String
{
	unsigned char* data;
	int size;
}String;

char* ConvertStringAsHex(String*);

#endif