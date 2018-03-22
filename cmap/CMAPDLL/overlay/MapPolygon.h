#pragma once
#include "head.h"
#include "BaseMapRoute.h"
#include "MapToolTip.h"

extern class MapOverlay;
extern class MapGraph;

class EXPORTS_CLASS MapPolygon:public BaseMapRoute
{
public:
	MapPolygon(void);
	~MapPolygon(void);
	MapPolygon(vector<PointLatLng> points, string name);

	MapOverlay* overlay;
	MapOverlay* OverlayGet();
	void OverlaySet(MapOverlay* value);

	bool visible;
	bool IsVisibleGet();
	void IsVisibleSet(bool value);


	void UpdateGraphicsPath();

	
	int width;
	MapColor backcolor;
	MapColor linecolor;
	Vertex* path;
	Vertex* ps;
	int ps_size;
	bool IsHitTestVisible;
	bool isMouseOver;
	bool IsMouseOverGet();
	void IsMouseOverSet(bool value);

	void iniValue();

	void OnRender(MapGraph & g);
	bool IsInside(int x, int y);


	MapToolTip tooltip;
	string tooltiptext;
};

