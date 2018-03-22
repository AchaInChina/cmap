#pragma once

#include "SqlWarpper.h"
#include "MyMutex.h"

extern class MapProvider;
extern class GPoint;
extern class PureImage;

class EXPORTS_CLASS CacheProvider
{
public:
	CacheProvider(void);
	~CacheProvider(void);

	SqlWarpper db;
	bool UseCache;
	CMyMutex lock;

	int Init();
	PureImage GetImageFrom(MapProvider * provider, GPoint pos, int zoom);
};

