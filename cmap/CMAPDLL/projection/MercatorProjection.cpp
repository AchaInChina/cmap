#include "MercatorProjection.h"
#include "RectLatLng.h"
#include "GPoint.h"
#include "PointLatLng.h"


double MercatorProjection::MinLatitude = -85.05112878;
double MercatorProjection::MaxLatitude = 85.05112878;
double MercatorProjection::MinLongitude = -180;
double MercatorProjection::MaxLongitude = 180;


MercatorProjection* MercatorProjection::Instance = NULL;

MercatorProjection::MercatorProjection(void)
{
	tileSize = GSize(256, 256);
	name = string("MercatorProjection");
}


MercatorProjection::~MercatorProjection(void)
{

}

MercatorProjection* MercatorProjection::InstanceGet()
{
	return Instance;
}

RectLatLng MercatorProjection::Bounds()
{
	return RectLatLng::FromLTRB(MinLongitude, MaxLatitude, MaxLongitude, MinLatitude);
}

GSize MercatorProjection::TileSize()
{
	return tileSize;
}

double MercatorProjection::Axis()
{
	return 6378137;
}

double MercatorProjection::Flattening()
{
	return (1.0 / 298.257223563);
}

GPoint MercatorProjection::FromLatLngToPixel(double lat, double lng, int zoom)
{
	GPoint ret = GPoint();

	lat = Clip(lat, MinLatitude, MaxLatitude);
	lng = Clip(lng, MinLongitude, MaxLongitude);

	double x = (lng + 180) / 360;
	double sinLatitude = sin(lat * Proje_PI / 180);
	double y = 0.5 - log((1 + sinLatitude) / (1 - sinLatitude)) / (4 * Proje_PI);

	GSize s = GetTileMatrixSizePixel(zoom);
	long mapSizeX = s.WidthGet();
	long mapSizeY = s.HeightGet();

	ret.X = (long)Clip(x * mapSizeX + 0.5, 0, mapSizeX - 1);
	ret.Y = (long)Clip(y * mapSizeY + 0.5, 0, mapSizeY - 1);

	return ret;
}

PointLatLng MercatorProjection::FromPixelToLatLng(long x, long y, int zoom)
{
	PointLatLng ret = PointLatLng();

	GSize s = GetTileMatrixSizePixel(zoom);
	double mapSizeX = s.WidthGet();
	double mapSizeY = s.HeightGet();

	double xx = (Clip(x, 0, mapSizeX - 1) / mapSizeX) - 0.5;
	double yy = 0.5 - (Clip(y, 0, mapSizeY - 1) / mapSizeY);

	ret.LatSet(90 - 360 * atan(exp(-yy * 2 * Proje_PI)) / Proje_PI);
	ret.LngSet(360 * xx);

	return ret;
}

GSize MercatorProjection::GetTileMatrixMinXY(int zoom)
{
	return GSize(0, 0);
}

GSize MercatorProjection::GetTileMatrixMaxXY(int zoom)
{
	long xy = (1 << zoom);
	return  GSize(xy - 1, xy - 1);
}

void MercatorProjection::InstanceDelete()
{
	delete Instance ;
}

void MercatorProjection::InstanceGener()
{
	Instance = new MercatorProjection();
}