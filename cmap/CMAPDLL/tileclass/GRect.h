#pragma once
extern class GPoint;
extern class GSize;
#include "head.h"
class EXPORTS_CLASS GRect
{
public:
	GRect(void);
	~GRect(void);
public:
	GRect Empty();
	long x;
	long y;
	long width;
	long height;
	GRect(long x, long y, long width, long height);
	GRect(GPoint location, GSize size);
	GRect FromLTRB(int left, int top, int right, int bottom);
	GPoint LocationGet();
	void LocationSet(GPoint value);

	bool IsEmpty();
	long Bottom();
	long Right();
	long Top();
	long Left();
	long HeightGet();
	void HeightSet(long value);
	long WidthGet();
	void WidthSet(long value);
	long YGet();
	void YSet(long value);
	long XGet();
	void XSet(long value);
	GSize SizeGet();
	void SizeSet(GSize value);
	GPoint LeftBottom();
	GPoint RightTop();
	GPoint RightBottom();

	bool Equals(GRect obj);
	bool operator==(GRect right);
	bool operator!=(GRect right);
	bool Contains(long x, long y);
	bool Contains(GPoint pt);
	bool Contains(GRect rect);
	void Inflate(long width, long height);
	void Inflate(GSize size);
	GRect Inflate(GRect rect, long x, long y);
	void Intersect(GRect rect);
	GRect Intersect(GRect a, GRect b);
	bool IntersectsWith(GRect rect);
	GRect Union(GRect a, GRect b);
	void Offset(GPoint pos);
	void OffsetNegative(GPoint pos);
	void Offset(long x, long y);
	string ToString();
};

