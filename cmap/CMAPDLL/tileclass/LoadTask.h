#pragma once
#include "head.h"
#include "GPoint.h"


class EXPORTS_CLASS LoadTask
{
public:
	LoadTask(void);
	~LoadTask(void);
public:
	GPoint Pos;
	int Zoom;
	LoadTask( const LoadTask &value);
	LoadTask(GPoint pos, int zoom);
	string ToString();
	bool Equals(LoadTask other);
	bool operator ==(LoadTask right);
};

