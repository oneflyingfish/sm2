#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<Windows.h>
#include"miracl.h"
#include"mirdef.h"
#include"ConstValueForSm2.h"
#include"String.h"

int Max =1200;
miracl *mip;
big k;								//随机数
big PBx;							//公钥x
big PBy;							//公钥y
big DB;								//私钥
String fileData;					//输入文件数据
int lengthC1x;						//C1的x长度
int lengthC1y;						//C1的y长度
int v = 64;							//hash值得到的十六进制串长度
char* ccode;						//加密得到的密文
char *inputFileName = "input.txt";	//输入文件名

int main()
{
	mip= mirsys(500, Max);
	ecurve_init(HexCharsToBig(a), HexCharsToBig(b), HexCharsToBig(p), MR_PROJECTIVE);	//初始化椭圆曲线内部参数
	Encryption();
	//Sleep(100);
	Decryption();
	//free(NULL);
	system("pause");
	return 0;
}