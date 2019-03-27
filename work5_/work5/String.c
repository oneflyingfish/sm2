#define  _CRT_SECURE_NO_WARNINGS
#include"String.h"
#include<stdio.h>
#include<stdlib.h>

char* ConvertStringAsHex(String* s)
{
	char* result = (char*)malloc(sizeof(char)*(s->size * 2+1));
	for (int i = 0; i < s->size; i++)
	{
		sprintf(&result[i * 2], "%02x", s->data[i]);
	}
	result[s->size * 2] = '\0';
	return result;
}