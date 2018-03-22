#pragma once
#include "head.h"
#include "GPoint.h"

extern class MapGraph;
extern struct SDL_Surface;
extern class MapMarker;
extern class MapRoute;
extern class MapPolygon;

class EXPORTS_CLASS MapToolTip
{
public:
	MapToolTip(void);
	~MapToolTip(void);
	MapToolTip(MapMarker* value);

	MapMarker* pMarker;
	MapRoute* pRoute;
	MapPolygon* pPolygon;
	SDL_Surface* pSurface;

	int style;
	MapColor m_fontColor;
	MapColor m_BackColor;

	void iniValue();
	MapMarker* MarkerGet();
	void MarkerSet(MapMarker* value);
	void RouteSet(MapRoute* value);
	void PolygonSet(MapPolygon* value);

	GPoint Offset;

	void OnRender(MapGraph & g);
};

