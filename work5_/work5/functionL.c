#include"functionL.h"
#include"miracl.h"
#include"mirdef.h"

void  bTd(int *a, int length)
{
	int j = 0;
	int i;
	for (i = 0; i < 64; i++)
		a[i] = 0;
	while (length > 0)
	{
		a[j] = length % 2;
		length = length / 2;
		j++;
	}
}

void CF(unsigned int *V, big B)
{

	int i;
	unsigned int turn;
	unsigned int W[68];
	unsigned int w[64];
	unsigned int a[8];
	unsigned int SS1, SS2, TT1, TT2;
	unsigned int T, G;
	big one;
	big x;
	big input;
	big m;
	big A[8];							//令A,B,C,D,E,F,G,H为字寄存器,
	input = mirvar(1);
	one = mirvar(1);
	x = mirvar(1);
	m = mirvar(1);
	for (i = 0; i < 8; i++)
	{
		a[i] = 0;
	}
	for (i = 0; i < 68; i++)
	{
		W[i] = 0;
	}
	for (i = 0; i < 64; i++)
	{
		w[i] = 0;
	}
	for (i = 0; i < 8; i++)
	{
		A[i] = mirvar(1);
	}
	expb2(32, x);						//2的32次方 
	copy(B, input);
	//将消息分组B(i)划分为16个字W0, W1, · · · , W15
	for (i = 0; i < 16; i++)
	{

		powmod(input, one, x, m);		//求模取余放到m中
		W[15 - i] = *m->w;
		sftbit(input, -32, input);		//右移32位 	
	}
	//b,16-67
	for (i = 16; i < 68; i++)
	{
		turn = P1(W[i - 16] ^ W[i - 9] ^ (Rol(W[i - 3], 15)));
		W[i] = turn ^ (Rol(W[i - 13], 7)) ^ W[i - 6];
	}
	//c,0-63
	for (i = 0; i < 64; i++)
	{
		w[i] = W[i] ^ W[i + 4];
	}
	//ABCDEFGH
	for (i = 0; i < 8; i++)
	{
		a[i] = V[i];
	}
	for (i = 0; i < 64; i++)
	{
		turn = Rol(TT(i), i);
		SS1 = Rol(((Rol(a[0], 12)) + a[4] + turn), 7);
		SS2 = SS1 ^ (Rol(a[0], 12));
		T = FF(i, a[0], a[1], a[2]);
		TT1 = T + a[3] + SS2 + w[i];
		G = GG(i, a[4], a[5], a[6]);
		TT2 = G + a[7] + SS1 + W[i];
		a[3] = a[2];
		a[2] = Rol(a[1], 9);
		a[1] = a[0];
		a[0] = TT1;
		a[7] = a[6];
		a[6] = Rol(a[5], 19);
		a[5] = a[4];
		a[4] = P0(TT2);
	}
	for (i = 0; i < 8; i++)
	{
		V[i] = V[i] ^ a[i];
	}
}

unsigned int P1(unsigned m)
{
	m = m ^ (Rol(m, 15)) ^ (Rol(m, 23));
	return m;
}

unsigned int TT(int j)
{
	unsigned int a = 0X79cc4519;
	unsigned int b = 0X7a879d8a;
	if (j >= 0 && j <= 15)
	{
		return a;
	}
	else
	{
		return b;
	}
}

unsigned int FF(int i, unsigned int a, unsigned int b, unsigned int c)
{
	if (i >= 0 && i <= 15)
	{
		return a ^ b^c;
	}
	else
	{
		return (a&b) | (a&c) | (b&c);
	}
}

unsigned int GG(int i, unsigned int a, unsigned int b, unsigned int c)
{
	if (i >= 0 && i <= 15)
	{
		return a ^ b^c;
	}
	else
	{
		return (a&b) | (~a&c);
	}
}

unsigned int P0(unsigned int TT2)
{
	return TT2 ^ (Rol(TT2, 9)) ^ (Rol(TT2, 17));
}

unsigned int Rol(unsigned int n, int m)
{
	n = ((n) << m) | (n >> (32 - m));
	return n;

}
