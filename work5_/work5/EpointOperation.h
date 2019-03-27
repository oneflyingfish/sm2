#ifndef EPOINTOPERATION_H
#define EPOINTOPERATION_H

#include<stdlib.h>
#include"miracl.h"
#include"mirdef.h"

extern miracl *mip;
extern int Max;

epoint* NewPoint(big x, big y);
epoint* AddEpoint(epoint*, epoint*);
epoint* MultiplyEpoint(big, epoint*);
big PointX(epoint*);
big PointY(epoint*);

//big类型基本运算封装
big Xor2(big x, big y);
big Add2(big x, big y);
big Sub2(big x, big y);
big Multiply2(big x, big y);
big Divide2(big x, big y);
big Mod2(big x, big y);
big Pow2(big x, int y);					// x^y
char* BigToHexChars2(big x);

#endif
