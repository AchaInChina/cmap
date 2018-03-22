#pragma once
#include "MapCore.h"
#include "PointLatLng.h"
#include "head.h"

class EXPORTS_CLASS Interface
{
public:
	Interface(void);
	~Interface(void);

	PointLatLng Position;
	GPoint PositionPixel;
	string CacheLocation;
	bool isDragging;
	RectLatLng ViewArea;
	MapProvider* mapProvider;
	bool CanDragMap;

	PointLatLng PositionGet();
	void PositionSet(PointLatLng value);

	GPoint PositionPixelGet();
	void PositionPixelSet(GPoint value);

	string CacheLocationGet();
	void CacheLocationSet(string value);

	bool IsDraggingGet();
	void IsDraggingSet(bool value);

	RectLatLng ViewAreaGet();
	void ViewAreaSet(RectLatLng value);

	MapProvider* mapProviderGet();
	void mapProviderSet(MapProvider* value);

	bool CanDragMapGet();
	void CanDragMapSet(bool value);


	virtual void ReloadMap() = 0;
	virtual PointLatLng FromLocalToLatLng(int x, int y) = 0;
	virtual GPoint FromLatLngToLocal(PointLatLng point) = 0;
};

