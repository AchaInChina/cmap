#pragma once
#include "BaseMapRoute.h"
#include "MapToolTip.h"

extern class MapGraph;
extern class MapOverlay;
extern class MapToolTip;

class EXPORTS_CLASS MapRoute:public BaseMapRoute
{
public:
	MapRoute(void);
	~MapRoute(void);
	MapRoute(string name);
	MapRoute(vector<PointLatLng> points, string name);

	MapOverlay* overlay;
	MapOverlay* OverlayGet();
	void OverlaySet(MapOverlay* value);

	bool visible;
	bool IsVisibleGet();
	void IsVisibleSet(bool value);


	void UpdateGraphicsPath();

	int width;
	MapColor color;
	Vertex* path;
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

