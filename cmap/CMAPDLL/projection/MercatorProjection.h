#pragma once
#include "head.h"
#include "PureProjection.h"
#include "GSize.h"

extern class PureProjection;

class EXPORTS_CLASS MercatorProjection : public PureProjection
{
public:
	MercatorProjection(void);
	~MercatorProjection(void);

	static MercatorProjection* Instance;
	static MercatorProjection* InstanceGet();
	static void InstanceDelete();
	static void InstanceGener();

	static double MinLatitude ;
	static double MaxLatitude ;
	static double MinLongitude ;
	static double MaxLongitude ;

	RectLatLng Bounds();
	GSize TileSize();
	GSize tileSize;
	double Axis();
	double Flattening();
	GPoint FromLatLngToPixel(double lat, double lng, int zoom);
	PointLatLng FromPixelToLatLng(long x, long y, int zoom);
	GSize GetTileMatrixMinXY(int zoom);
	GSize GetTileMatrixMaxXY(int zoom);


};

