#pragma once
extern class GPoint;
#include "head.h"
class EXPORTS_CLASS GSize
{
public:
	GSize(void);
	~GSize(void);
	GSize(GPoint pt);
	explicit GSize(long Width, long Height);
	GSize operator +(GSize sz2);
	GSize operator -(GSize sz2);
	bool operator ==(GSize sz2);
	bool operator !=(GSize sz2);
public:
	long width;
	long height;
	bool IsEmpty(){ return width == 0 && height == 0; };
	long WidthGet()	{ return width;	};
	void WidthSet(long value)	{ width = value; };
	long HeightGet(){ return height; };
	void HeightSet(long value){ height = value;};
	GSize Add(GSize sz1, GSize sz2);
	GSize Subtract(GSize sz1, GSize sz2);
	bool Equals(GSize obj);
	std::string ToString();
};


