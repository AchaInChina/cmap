#pragma once
#include "MapProvider.h"
#include "head.h"
extern class PureProjection;

class EXPORTS_CLASS GaoDeMapProvider : public MapProvider
{
public:
	GaoDeMapProvider(void);
	~GaoDeMapProvider(void);

	vector<MapProvider*> overlays;
	PureProjection* Projection();
	vector<MapProvider*>& Overlays();
	string Name() ;
	string Id() ;
	PureImage GetTileImage(GPoint pos, int zoom);
	string MakeTileImageUrl(GPoint pos, int zoom, string language);
	bool operator==( GaoDeMapProvider right);
	static GaoDeMapProvider* InstanceGet();
	static void	InstanceGener();
	static void InstanceDelete();
};

