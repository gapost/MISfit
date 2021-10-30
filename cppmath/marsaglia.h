#ifndef MARSAGLIA_H
#define MARSAGLIA_H

/* The  MWC generator concatenates two 16-bit
  multiply-with-carry generators, x(n)=36969x(n-1)+carry,
  y(n)=18000y(n-1)+carry  mod 2^16, has  period  about
  2^60 and seems to pass all tests of randomness. A favorite
  stand-alone generator---faster than KISS, which contains it.*/

#ifndef WIN32
#define __forceinline inline
#endif

class CRngMWC
{
	unsigned int z,w;
public:
	CRngMWC() 
	{ 
		SetSeed(); 
	}
	~CRngMWC() {}
	void SetSeed(unsigned int az=362436069, unsigned int aw=521288629)
	{
		z = az;
		w = aw;
	}
	__forceinline unsigned int NextLong()
	{
		z = 36969*(z&65535)+(z>>16);
		w = 18000*(w&65535)+(w>>16);
		return ((z<<16) + (w&65535));
	}
	__forceinline double NextFloat()
	{
		return NextLong()*2.328306437e-10;
	}
};

#endif

