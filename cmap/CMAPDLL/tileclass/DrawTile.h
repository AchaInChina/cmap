#pragma once
#include "head.h"
#include "GPoint.h"

class EXPORTS_CLASS DrawTile
{
public:
	DrawTile(void);
	~DrawTile(void);
public:
	GPoint PosXY;
	GPoint PosPixel;
	double DistanceSqr;
	string ToString();
	bool Equals(DrawTile other);
	int CompareTo(DrawTile other);
	bool operator ==(DrawTile right);
	bool operator < (DrawTile Pinfo);
	bool operator > (DrawTile Pinfo);
};

bool lesscmp(DrawTile a, DrawTile b);