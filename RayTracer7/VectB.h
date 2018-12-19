#pragma once
#ifndef _VECTB_H
#define _VECTB_H



class VectB
{

	bool x, y, z;
public:
	VectB();
	VectB(bool, bool, bool);
	~VectB();

	bool getVectBX() { return x; }
	bool getVectBY() { return y; }
	bool getVectBZ() { return z; }

	



private:

};

VectB::VectB()
{
	x = 0;
	y = 0;
	z = 0;
}

VectB::VectB(bool i, bool j, bool k)
{
	x = i;
	y = j;
	z = k;
}

VectB::~VectB()
{
}

#endif // !_VectB_H
