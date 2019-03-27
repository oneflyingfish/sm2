#ifndef CONSTVALUEFORSM2_H
#define CONSTVALUEFORSM2_H

#include"String.h"
#include"miracl.h"
#include"mirdef.h"
#include"Sm3.h"
#include"EpointOperation.h"

//定义外部导入
extern miracl *mip;
extern int Max;
extern big k;						//随机数k
extern big PBx;						//公钥x
extern big PBy;						//公钥y
extern big DB;						//私钥
extern char* p;						//给定参数
extern char* a;						//给定参数
extern char* b;						//给定参数
extern char* n;						//给定参数
extern char* Gx;					//给定参数
extern char* Gy;					//给定参数
extern int lengthC1x;				//C1的x长度
extern int lengthC1y;				//C1的y长度
extern int v;					//hash值得到的十六进制串长度
extern String fileData;				//输入文件数据
extern char* ccode;					//加密得到的密文
extern char *inputFileName;			//输入文件名

//功能函数声明
big HexCharsToBig(char*);
big GetBigRandom(big , big);		// 产生随机数 a<= result <=b
void InitRandomK();					//产生随机参数K
void CalculateKeys();				//产生公钥和私钥
int VerifyKeys();					//验证公钥和私钥
epoint *CalculateG();				//自定义参数G
epoint *CalculatePB();				//公钥PB(PBx,PBy)
String* EpointToBytes(epoint *);	//未压缩形式，故头部PC=04
void ReadInputFile();				//读取文件输入
void Encryption();					//加密
epoint *CalculatePoint1();			//计算(x1,y1)
epoint *CalculatePoint2();			//计算(x2,y2)
String *CalculateC1();				//计算C1
String *CalculateC2();				//计算C2
char* CalculateC3();				//计算C3
big KDF(epoint*,int);				//计算t

void Decryption();					//解密
char* GetPartString(char*,int,int);	//截取字符串
//epoint *CalculateS();

#endif