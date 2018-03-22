#pragma once
#include "head.h"
#include "GPoint.h"
#include "SDL.h"
#include "PointLatLng.h"
#include "MapToolTip.h"

extern class GSize;
extern class MapOverlay;
extern class MapGraph;

class EXPORTS_CLASS MapMarker
{
public:
	MapMarker(void);
	MapMarker(PointLatLng pos);
	virtual ~MapMarker(void);

	MapOverlay* overlay;
	MapOverlay* OverlayGet();
	void OverlaySet(MapOverlay* value);

	PointLatLng position;
	PointLatLng PositionGet();
	void PositionSet(PointLatLng value);

	bool visible;
	bool IsVisibleGet();
	void IsVisibleSet(bool value);

	GPoint offset;
	GPoint OffsetGet();
	void OffsetSet(GPoint value);

	SDL_Rect area;
	SDL_Rect LocalPositionGet();
	void LocalPositionSet(SDL_Rect value);
	GSize SizeGet();
	void SizeGet(GSize value);

	GPoint ToolTipPosition();
	SDL_Rect LocalArea();
	bool AreaContains(int x, int y);

	bool isInsideWindow;
	bool IsHitTestVisible;
	bool isMouseOver;
	bool IsMouseOverGet();
	void IsMouseOverSet(bool value);

	bool isEmpty();

	virtual void OnRender(MapGraph & g);

	virtual int WideGet() = 0;
	virtual int HeigthGet() = 0;

	void initValue();
	MapToolTip tooltip;
	string tooltiptext;
};

