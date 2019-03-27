#include"EpointOperation.h"
#include<stdlib.h>
#include"miracl.h"
#include"mirdef.h"

epoint* NewPoint(big x, big y)
{
	epoint* result = (epoint*)malloc(sizeof(epoint));
	result = epoint_init();
	epoint_set(x, y, 0, result);
	return result;
}

epoint* AddEpoint(epoint* a, epoint* b)
{
	epoint* result = (epoint*)malloc(sizeof(epoint));
	result = epoint_init();
	epoint_copy(b, result);
	ecurve_add(a, result);
	return result;
}

epoint* MultiplyEpoint(big a, epoint* b)
{
	epoint* result = (epoint*)malloc(sizeof(epoint));
	result= epoint_init();
	ecurve_mult(a, b, result);
	return result;
}

big PointX(epoint* point)
{
	big x = mirvar(0);
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(y);
	return x;
}

big PointY(epoint* point)
{
	big x = mirvar(0);
	big y = mirvar(0);
	epoint_get(point, x, y);
	mirkill(x);
	return y;
}

big Xor2(big x, big y)
{
	big result = mirvar(0);
	unsigned char xString[1000];
	unsigned char yString[1000];
	int lengthX = big_to_bytes(0, x, xString, FALSE);
	int lengthY = big_to_bytes(0, y, yString, FALSE);
	
	if (lengthX < lengthY)
	{
		for (int i = 0; i < lengthX; i++)
		{
			yString[lengthY - 1 - i] ^= xString[lengthX - 1 - i];
		}
		bytes_to_big(lengthY, yString, result);
	}
	else
	{
		for (int i = 0; i < lengthY; i++)
		{
			xString[lengthX - 1 - i] ^= yString[lengthY - 1 - i];
		}
		bytes_to_big(lengthX, xString, result);
	}

	return result;
}

big Add2(big x, big y)
{
	big result = mirvar(0);
	add(x, y, result);
	return result;
}

big Sub2(big x, big y)
{
	big result = mirvar(0);
	subtract(x, y, result);
	return result;
}

big Multiply2(big x, big y)
{
	big result = mirvar(0);
	multiply(x, y, result);
	return result;
}

big Divide2(big x, big y)
{
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	big z1 = mirvar(0);
	copy(x, x1);
	copy(y, y1);
	divide(x1, y1, z1);
	mirkill(x1);
	mirkill(y1);
	return z1;
}

big Mod2(big x, big y)
{
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	big z1 = mirvar(0);
	copy(x, x1);
	copy(y, y1);
	powmod(x1, mirvar(1), y1, z1);
	mirkill(x1);
	mirkill(y1);
	return z1;
}

big Pow2(big x, int y)
{
	big x1 = mirvar(0);
	big y1 = mirvar(0);
	copy(x, x1);
	for (int i = 0; i < y; i++)
	{
		y1=Multiply2(x1, x1);
	}
	mirkill(x1);
	return y1;
}

char* BigToHexChars2(big x)
{
	mip->IOBASE = 16;
	char *str = (char*)malloc(sizeof(char)*Max);
	cotstr(x, str);
	return str;
}