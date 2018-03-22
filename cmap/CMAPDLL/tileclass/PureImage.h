#pragma once
#include "head.h"
class EXPORTS_CLASS PureImage
{
public:
	PureImage(void);
	~PureImage(void);
public:
	string Data;
	bool IsParent;
	long Ix;
	long Xoff;
	long Yoff;
};

