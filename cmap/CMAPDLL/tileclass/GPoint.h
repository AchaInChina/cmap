#pragma once
#include "head.h"
extern class GSize;

class EXPORTS_CLASS GPoint
{
public:
	GPoint(void);
	~GPoint(void);
	GPoint(long x, long y);
	GPoint(GSize sz);

public:
	long X;
	long Y;
	bool isEmpty;
	bool IsEmpty(){ return isEmpty; };
	long XGet()	{ return X;	};
	void XSet(long value)	{ X = value; };
	long YGet(){ return Y; };
	void YSet(long value){ Y = value;};
	GPoint operator+(GSize sz);
	GPoint operator-(GSize sz);
	bool operator==(const GPoint& right)const;
	bool operator!=( GPoint right);
	bool operator<( GPoint right)const;

	GPoint Add(GPoint pt, GSize sz);
	GPoint Subtract(GPoint pt, GSize sz);
	bool Equals(GPoint obj);
	void Offset(long dx, long dy);
	void Offset(GPoint p);
	void OffsetNegative(GPoint p);
	std::string ToString();
};

