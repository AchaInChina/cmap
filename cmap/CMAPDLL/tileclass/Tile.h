#pragma once
#include "head.h"
#include "GPoint.h"
#include "PureImage.h"


class EXPORTS_CLASS Tile
{
public:
	Tile(void);
	~Tile(void);
public:
	GPoint pos;
	int zoom;
	vector<PureImage> overlays;
	long OverlaysCount;
	bool NotEmpty;
	int OverlaysIndex;
	Tile(int zoom, GPoint pos);
	PureImage OverlaysGet();
	void AddOverlay(PureImage i);
	bool HasAnyOverlays();
	int ZoomGet();
	void ZoomSet(int value);
	GPoint PosGet();
	void PosSet(GPoint value);
	bool operator ==(Tile m2);
	bool operator !=(Tile m2);
	bool Equals(Tile m2);
};

