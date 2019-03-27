#define _CRT_SECURE_NO_WARNINGS
#include"ConstValueForSm2.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"EpointOperation.h"
#include"miracl.h"
#include"mirdef.h"
#include"String.h"
#include"Sm3.h"

big HexCharsToBig(char* str)
{
	mip->IOBASE = 16;
	big result = mirvar(0);
	cinstr(result, str);
	return result;
}

big GetBigRandom(big a, big b)
{
	irand((unsigned)time(NULL));
	big result = mirvar(0);
	bigrand(Add2(Sub2(b,a),mirvar(1)), result);		// 0<= result <b-a+1
	return Add2(result, a);							// a<= xxx <=b
}

void InitRandomK()
{
	k = mirvar(0);
	copy(GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(1))),k);	// 1<= k <= n-1
}

epoint * CalculateG()
{
	epoint *G = (epoint*)malloc(sizeof(epoint));
	G = epoint_init();
	//ecurve_init(HexCharsToBig(a), HexCharsToBig(b), HexCharsToBig(p), MR_PROJECTIVE);	//初始化椭圆曲线内部参数
	epoint_set(HexCharsToBig(Gx), HexCharsToBig(Gy),0, G);
	return G;
}

epoint *CalculatePB()
{
	//ecurve_init(HexCharsToBig(a), HexCharsToBig(b), HexCharsToBig(p), MR_PROJECTIVE);	//初始化椭圆曲线内部参数
	epoint *G = (epoint*)malloc(sizeof(epoint));
	G= epoint_init();
	epoint_set(PBx, PBy,0, G);
	return G;
}

//计算私钥和私钥 DB、PB
void CalculateKeys()
{
	big dm = mirvar(0);
	dm=GetBigRandom(mirvar(1), Sub2(HexCharsToBig(n), mirvar(2)));				// 私钥	[1,n-2]
	epoint* pm = epoint_init();
	pm=MultiplyEpoint(dm, CalculateG());										// 公钥 
	PBx = PointX(pm);
	PBy = PointY(pm);
	DB = dm;
}

int VerifyKeys()
{
	if(!compare(Mod2(Pow2(PBy, 2),HexCharsToBig(p)) , Mod2(Add2(Pow2(PBx,3), Add2(Multiply2(PBx,HexCharsToBig(a)), HexCharsToBig(b))),HexCharsToBig(p))))		// if((PBy^2 %p) != ((PBx^3 + a*PBx +b)%p))
	{
		printf("公钥验证无效，请重启程序\n");
		system("pause");
		exit(1);
	}
	printf("公钥验证有效!\n\n");
	return 1;
}

epoint *CalculatePoint1()
{
	return MultiplyEpoint(k, CalculateG());			// 公钥
}

epoint *CalculatePoint2()
{
	return MultiplyEpoint(k, CalculatePB());			// 公钥
}

String* EpointToBytes(epoint *point)
{
	unsigned char *x = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	unsigned char *y = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	int lengthX = big_to_bytes(0, PointX(point), x, FALSE);
	int lengthY= big_to_bytes(0, PointY(point), y, FALSE);

	String *result = (String*)malloc(sizeof(String));
	result->size = lengthX + lengthY + 1;
	result->data = (unsigned char*)malloc(sizeof(unsigned char)*(result->size));

	int i = 0;
	result->data[i++] = 4;

	for (int j = 0; j < lengthX; j++)
	{
		result->data[i++] = x[j];
	}

	for (int j = 0; j < lengthY; j++)
	{
		result->data[i++] = y[j];
	}

	return result;		// 04 || point->X || point->Y
}

void ReadInputFile()
{
	FILE *fp = fopen(inputFileName, "r");
	//打开输入文件
	if (fp==NULL)
	{
		printf("%s不存在\n", inputFileName);
		system("pause");
		exit(2);
	}
	//读取数据，可能包含回车、空格等，故使用fgetc
	char* data = (char*)malloc(sizeof(char) * Max);
	int dataSize = 0;
	char ch;
	while ((ch = fgetc(fp)) != EOF)
	{
		data[dataSize] =ch;
		dataSize++;
	}
	fclose(fp);
	data[dataSize] = '\0';

	fileData.data = data;
	fileData.size = dataSize;
}

void Encryption()
{
	CalculateKeys();		//计算公钥和私钥
	VerifyKeys();			//验证公钥
	ReadInputFile();

	Restart:				//重新开始生成参数
	InitRandomK();			//初始化随机数

	//计算并处理C1
	String *C1 = CalculateC1();
	char* c1String = ConvertStringAsHex(C1);
	free(C1->data);
	free(C1);
	
	String *C2 = CalculateC2();
	if (C2->size <= 0 && C2->data == NULL)	//t全零
	{
		goto Restart;
	}
	char* c2String = ConvertStringAsHex(C2);
	free(C2->data);
	free(C2);
	
	char* c3String = CalculateC3();

	//打印加密重要参数
	printf("参数如下(以下数据均为16进制)\n");
	printf("椭圆曲线方程为：y^2=x^3+a*x+b\n");
	printf("p=%s\n", p);
	printf("a=%s\n", a);
	printf("b=%s\n", b);
	printf("n=%s\n", n);
	printf("Gx=%s\n", Gx);
	printf("Gy=%s\n", Gy);
	printf("k=%s\n\n", BigToHexChars2(k));
	printf("秘钥参数如下: \n");
	printf("私钥d=%s\n\n", BigToHexChars2(DB));
	printf("公钥x=%s\n\n", BigToHexChars2(PBx));
	printf("公钥y=%s\n\n", BigToHexChars2(PBy));
	printf("C1=%s\n\n", c1String);
	printf("C2=%s\n\n", c2String);
	printf("C3=%s\n\n", c3String);

	char *c = (char*)calloc(strlen(c1String) + strlen(c2String) + strlen(c3String) + 1,sizeof(char));
	strcat(c, c1String);
	strcat(c, c2String);
	strcat(c, c3String);
	free(c1String);
	free(c2String);
	free(c3String);

	ccode = c;
	printf("明文为:\n%s\n\n", fileData.data);
	printf("密文为：\n%s\n\n\n", ccode);
}

// C1 = [k]G
String * CalculateC1()
{
	epoint *point1 = CalculatePoint1();
	unsigned char *x = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	unsigned char *y = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	lengthC1x = big_to_bytes(0, PointX(point1), x, FALSE)*2;
	lengthC1y = big_to_bytes(0, PointY(point1), y, FALSE)*2;

	String *result= EpointToBytes(point1);
	epoint_free(point1);
	return result;
}

String * CalculateC2()
{
	String *result = (String*)malloc(sizeof(String));
	epoint *point2 = CalculatePoint2();

	big t = KDF(point2, fileData.size);
	if (compare(t, mirvar(0)) == 0)
	{
		result->data = NULL;
		result->size = -1;
		return result;
	}

	epoint_free(point2);

	unsigned char *tString = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	int lengthT = big_to_bytes(0, t, tString, FALSE);		//求出byte数,将t转换成字符串
	if (lengthT != fileData.size)
	{
		for (int i = lengthT-1; i >=0; i--)
		{
			tString[fileData.size - lengthT+i] = tString[i];
		}

		for (int i = 0; i < fileData.size - lengthT; i++)
		{
			tString[i] = 0;
		}
		lengthT = fileData.size;
	}
	
	result->data= (unsigned char*)malloc(sizeof(unsigned char)*lengthT);
	for (int i = 0; i < lengthT; i++)
	{
		result->data[i] = tString[i] ^ fileData.data[i];
	}
	result->size = lengthT;

	free(tString);
	mirkill(t);

	return result;
}

// 备注：文档中：Z=x2 || y2
big KDF(epoint* point2,int klen)
{
	unsigned char* xStr = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	unsigned char* yStr = (unsigned char*)malloc(sizeof(unsigned char)*Max);
	int lengthX = big_to_bytes(0, PointX(point2), xStr, FALSE);
	int lengthY = big_to_bytes(0, PointY(point2), yStr, FALSE);
	free(xStr);
	free(yStr);

	big x = PointX(point2);
	big y = PointY(point2);

	big t = mirvar(0);
	big ct = mirvar(1);						//计数器至少32位，初值为1
	big V0 = mirvar(0);
	big Ha = mirvar(0);
	big Z = mirvar(0);
	int v = 32;								//sm3的hash值长度为32字
	
	//求解Z
	copy(x, Z);
	sftbit(Z, lengthY * 8, Z);				//左移y位数
	add(Z, y, Z);							//Z比特拼接y 最终Z= x||y

	if (klen / v >= 1)
	{
		for (int i = 1; i <= klen / v; i++)
		{
			sftbit(Z, 32, t);					//Z左移ct位数
			add(t, ct, t);						//t比特拼接ct

			big sm3Value = SM3(t);
			copy(sm3Value,V0);					//求t的hash值 t= Z||ct
			mirkill(sm3Value);
			add(Ha, V0, Ha);					//Ha比特拼接V0

			sftbit(Ha, 256, Ha);				//Ha左移V0位数，为下一次拼接做准备 Ha= Ha||x||y||ct
			add(ct, mirvar(1),ct);				//计数器加一
		}
		sftbit(Ha, -256, Ha);					//去掉多出的一次左移
	}

	if (klen%v == 0)	//klen/v为整数
	{
		//同上算法
		sftbit(Z, 32, t);						//左移32位
		add(t, ct, t);							//结合后的数

		big sm3Value = SM3(t);
		copy(sm3Value, V0);						//求t的hash值 t= Z||ct
		mirkill(sm3Value);

		sftbit(Ha, 256, Ha);
		add(V0, Ha, Ha);						//哈希后的数和之前的数相加
	}
	else
	{
		sftbit(Z, 32, t);						//左移32位
		add(t, ct, t);							//结合后的数

		copy(SM3(t), V0);						//哈希后的数，保存到V0中,256位
		sftbit(V0, -(256 - (klen * 8 - (klen / v)*v * 8)), V0);

		sftbit(Ha, klen * 8 - (klen / v)*v * 8, Ha);
		add(Ha,V0, Ha);							//哈希后的数和之前的数相加
	}

	mirkill(V0);
	mirkill(x);
	mirkill(y);
	mirkill(t);
	mirkill(ct);
	mirkill(Z);
	return Ha;
}

char* CalculateC3()
{
	epoint *point2 = CalculatePoint2();
	char* x = (char*)malloc(sizeof(char)*Max);
	char* y = (char*)malloc(sizeof(char)*Max);
	int lengthX = big_to_bytes(0, PointX(point2), x, FALSE);
	int lengthY = big_to_bytes(0, PointY(point2), y, FALSE);
	epoint_free(point2);

	char* xmy = (char*)malloc(sizeof(char)*(lengthX*2 + lengthY*2 + fileData.size*2+1));
	int i = 0;
	for (int j = 0; j < lengthX; j++)
	{
		sprintf(&xmy[i], "%02x", (unsigned char)(x[j]));
		i += 2;
	}

	for (int j = 0; j < fileData.size; j++)
	{
		sprintf(&xmy[i], "%02x", (unsigned char)(fileData.data[j]));
		i += 2;
	}

	for (int j = 0; j < lengthY; j++)
	{
		sprintf(&xmy[i], "%02x", (unsigned char)(y[j]));
		i += 2;
	}
	xmy[i] = '\0';

	free(x);
	free(y);

	xmy = SM3ByHexString(xmy);											//hash（x2||data||y2)
	return xmy;
}

void Decryption()
{
	//从密文中得到C1、C2、C3
	char* x1String = GetPartString(ccode,2, lengthC1x);					//2代表去除头部的04压缩种类标识
	char* y1String = GetPartString(ccode, 2+lengthC1x, lengthC1y);
	char* C2String = GetPartString(ccode, 2+lengthC1x + lengthC1y, strlen(ccode)-(2 + lengthC1x + lengthC1y+v));
	char* C3String=  GetPartString(ccode, strlen(ccode)-v,v);
	free(ccode);

	//求解明文长度
	int klen = strlen(C2String) / 2;									//获取明文长度

	//求解C1
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	mip->IOBASE = 16;
	cinstr(x1, x1String);
	cinstr(y1, y1String);
	epoint* C1 = NewPoint(x1, y1);			//还原C1
	
	//求解t
	epoint* C2 = MultiplyEpoint(DB, C1);	//求解 [DB]C1=(x2, y2)
	epoint_free(C1);

	big t = KDF(C2, klen);					//求解 t = KDF(x2∥y2,klen)
	if (compare(t, mirvar(0)) == 0)
	{
		printf("解密时t全0，错误\n");
		system("pause");
		exit(3);
	}

	//求解异或值M'=Xor(C2,t)
	mip->IOBASE = 16;
	//求解M'字节串形式
	big C2Number = mirvar(0);
	cinstr(C2Number, C2String);
	char* mcode = BigToHexChars2(Xor2(C2Number, t));
	mirkill(C2Number);

	char* xmy = (char*)calloc(strlen(BigToHexChars2(PointX(C2))) + strlen(BigToHexChars2(PointY(C2))) + strlen(mcode) + 1, sizeof(char));
	strcat(xmy, BigToHexChars2(PointX(C2)));
	strcat(xmy, mcode);
	strcat(xmy, BigToHexChars2(PointY(C2)));
	xmy[strlen(BigToHexChars2(PointX(C2))) + strlen(BigToHexChars2(PointY(C2))) + strlen(mcode)] = '\0';

	if (strcmp(SM3ByHexString(xmy), C3String) != 0)
	{
		printf("破译失败\n");
		return;
	}
	free(xmy);

	printf("经验证，解密成功，明文十六进制为:\n%s\n", mcode);

	mip->IOBASE = 16;
	big mNumber = mirvar(0);
	cinstr(mNumber,mcode);
	char mString[1000];
	int mStringLength=big_to_bytes(0, mNumber, mString, FALSE);
	mString[mStringLength] = '\0';
	printf("翻译成明文为:\n%s\n\n",mString);
}

char* GetPartString(char* string, int startIndex, int length)
{
	char* str = (char*)malloc(sizeof(char)*(length+1));
	
	int i = 0;
	for (int j = 0; j < length; j++)
	{
		str[i++] = string[startIndex + j];
	}
	str[i] = '\0';
	return str;
}

//定义给定常量
char* p = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
char* a = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
char* b = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
char* n = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
char* Gx = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
char* Gy = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";

////参数h尚未求解
//epoint *CalculateS()
//{
//	//big h = Multiply(, HexCharsToBig(n));					// h= #E(Fq)/n，为椭圆曲线E(Fq)的阶 / n是基点G的阶
//	//epoint *result = MultiplyEpoint(h, CalculatePB());
//	//if (point_at_infinity(result))
//	//{
//	//	printf("S是无穷远点，错误");
//	//	system("pause");
//	//	exit(1);
//	//}
//	//return result;
//	return NULL;
//}