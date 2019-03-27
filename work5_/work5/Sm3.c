#include<stdlib.h>
#include"miracl.h"
#include"mirdef.h"
#include"Sm3.h"
#include"functionL.h"

big SM3(big input)
{

	big z = mirvar(1);
	big x = mirvar(1);
	big V = mirvar(0);
	big one = mirvar(1);
	unsigned int V0[8]; 						//V(0)为256比特初始值IV
	big B[5];
	int l;										//消息的长度
	int ll = 63;
	int length[64];								//消息的二进制表示 
	int n; 										//迭代次数 
	int i;										//for循环所用 

	for (i = 0; i < 5; i++)
	{
		B[i] = mirvar(0);
	}

	V0[0] = 0X7380166f;
	V0[1] = 0X4914b2b9;
	V0[2] = 0X172442d7;
	V0[3] = 0Xda8a0600;
	V0[4] = 0Xa96f30bc;
	V0[5] = 0X163138aa;
	V0[6] = 0Xe38dee4d;
	V0[7] = 0Xb0fb0e4e;

	l = 4 * numdig(input);							//返回大数中数字的个数 
	bTd(length, l);									//长度的二进制表示，放到length数组中 


	//填充
	sftbit(input, 1, z);							//将一个大数左移1位
	incr(z, 1, z);									//加1
	copy(z, input);
	l++;
	//填充0 
	while (l % 512 != 448)
	{
		sftbit(input, 1, z);						//:将一个大数左移n位
		copy(z, input);
		l++;
	}
	// 
	while (ll >= 0)
	{
		sftbit(input, 1, z);						//:将一个大数左移1位

		if (length[ll] == 1)
		{
			incr(z, 1, z);							//+1
		}
		copy(z, input);
		ll--;
	}

	//迭代
	expb2(512, x);									//2的512次方 	
	n = (l + 64) / 512;								//512的组数 

	//将输入按照512bit分组 
	for (i = 0; i < n; i++)
	{
		powmod(input, one, x, B[n - 1 - i]);		//求模取余放到B[i]中 
		sftbit(input, -512, input);					//右移512位 
	}
	for (i = 0; i < n; i++)
	{
		CF(V0, B[i]);								//传入Vi，输出Vi+1  
	}
	for (i = 0; i < 8; i++)
	{
		*V->w = V0[i];
		V->len = (i + 1) * 8;
		sftbit(V, 32, V);
	}
	sftbit(V, -32, V);
	return V;
}

char* SM3ByHexString(char* input)
{
	mip->IOBASE = 16;
	big bigCodeNumber = mirvar(0);
	cinstr(bigCodeNumber, input);
	big resultNUmber = mirvar(0);
	copy(SM3(bigCodeNumber),resultNUmber);

	char* result = (char*)malloc(sizeof(char)*Max);
	cotstr(resultNUmber, result);
	return result;
}

//#define _CRT_SECURE_NO_WARNINGS
//#include<stdlib.h>
//#include"Sm3.h"
//#include"InputInfo.h"
//#include"function.h"
//#include"BitList.h"
//
//big SM3(big m)
//{
//	mip->IOBASE = 16;
//	char * mString = (char*)malloc(sizeof(char)*Max);
//	int lengthM = big_to_bytes(0, m, mString, FALSE);
//	char* mHexString = (char*)malloc(sizeof(char)*(lengthM*2+1));
//
//	for (int i = 0; i < lengthM; i++)
//	{
//		sprintf(&mHexString[i * 2], "%02x", (unsigned char)mString[i]);
//	}
//	mHexString[lengthM * 2] = '\0';
//
//	mip->IOBASE = 10;
//	//读取磁盘输入
//	InputInfo *inputInfo = ReadInput(mHexString);
//	//填充
//	FillInputInfo(inputInfo);
//	//读物Bn和Wj
//	GroupsInfo *groupInfo = DivideIntoGroups(inputInfo);
//	//计算n
//	int n = groupInfo->count;
//
//	big Vi[8];
//	//计算V0
//	Vi[0] = mirvar(0);
//	cinstr(Vi[0], "1937774191");
//	Vi[1] = mirvar(0);
//	cinstr(Vi[1], "1226093241");
//	Vi[2] = mirvar(0);
//	cinstr(Vi[2], "388252375");
//	Vi[3] = mirvar(0);
//	cinstr(Vi[3], "3666478592");
//	Vi[4] = mirvar(0);
//	cinstr(Vi[4], "2842636476");
//	Vi[5] = mirvar(0);
//	cinstr(Vi[5], "372324522");
//	Vi[6] = mirvar(0);
//	cinstr(Vi[6], "3817729613");
//	Vi[7] = mirvar(0);
//	cinstr(Vi[7], "2969243214");
//
//	//计算Vn
//	big* V = Vi;
//	for (int i = 0; i < n; i++)
//	{
//		V = CF(V, groupInfo, i);
//	}
//
//	char* result = (char*)malloc(sizeof(char) * 65);
//	for (int i = 0; i < 8; i++)
//	{
//		char *resultString = BigWordToHexString(V[i]);
//		sprintf(&result[8*i],"%s", resultString);
//		//释放内存
//		free(resultString);
//	}
//	result[64] = '\0';
//
//	FreeList(inputInfo->inputBitList);
//	free(inputInfo);
//	free(groupInfo);
//	free(mHexString);
//	free(mString);
//	free(V);
//	
//	
//	big r = mirvar(0);
//	mip->IOBASE = 16;
//	cinstr(r,result);
//	free(result);
//	return r;
//}
//
//
//char* SM3ByHexString(char* mHexString)
//{
//	//读取磁盘输入
//	InputInfo *inputInfo = ReadInput(mHexString);
//	//填充
//	FillInputInfo(inputInfo);
//	//读物Bn和Wj
//	GroupsInfo *groupInfo = DivideIntoGroups(inputInfo);
//	//计算n
//	int n = groupInfo->count;
//
//	mip->IOBASE = 10;
//	big Vi[8];
//	//计算V0
//	Vi[0] = mirvar(0);
//	cinstr(Vi[0], "1937774191");
//	Vi[1] = mirvar(0);
//	cinstr(Vi[1], "1226093241");
//	Vi[2] = mirvar(0);
//	cinstr(Vi[2], "388252375");
//	Vi[3] = mirvar(0);
//	cinstr(Vi[3], "3666478592");
//	Vi[4] = mirvar(0);
//	cinstr(Vi[4], "2842636476");
//	Vi[5] = mirvar(0);
//	cinstr(Vi[5], "372324522");
//	Vi[6] = mirvar(0);
//	cinstr(Vi[6], "3817729613");
//	Vi[7] = mirvar(0);
//	cinstr(Vi[7], "2969243214");
//
//	//计算Vn
//	big* V = Vi;
//	for (int i = 0; i < n; i++)
//	{
//		V = CF(V, groupInfo, i);
//	}
//
//	char* result = (char*)malloc(sizeof(char) * 65);
//	for (int i = 0; i < 8; i++)
//	{
//		char *resultString = BigWordToHexString(V[i]);
//		sprintf(&result[8 * i], "%s", resultString);
//		//释放内存
//		free(resultString);
//	}
//	result[64] = '\0';
//
//	FreeList(inputInfo->inputBitList);
//	free(inputInfo);
//	DeleteGroupInfo(groupInfo);
//	free(groupInfo);
//	free(V);
//
//	return result;
//}