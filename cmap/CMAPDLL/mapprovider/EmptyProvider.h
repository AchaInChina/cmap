#pragma once
#include "MapProvider.h"
#include "head.h"

extern class PureProjection;
extern class PureImage;

class EXPORTS_CLASS EmptyProvider:public MapProvider
{
public:
	EmptyProvider(void);
	~EmptyProvider(void);

	vector<MapProvider*> overlays;
	static EmptyProvider* Instance;
	vector<MapProvider*>& Overlays();
	static EmptyProvider* InstanceGet();

	PureImage GetTileImage(GPoint pos, int zoom);
	string Name() ;
	string Id() ;
	PureProjection* Projection();
	static void InstanceGener();
	static void InstanceDelete();
};

