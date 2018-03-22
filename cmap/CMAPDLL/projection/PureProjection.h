#pragma once
#include "head.h"
#include "GPoint.h"
#include "PointLatLng.h"

extern class GSize;
extern class RectLatLng;


class EXPORTS_CLASS PureProjection
{
public:
	PureProjection(void);
	~PureProjection(void);
public:
	vector<map<PointLatLng, GPoint>> FromLatLngToPixelCache;
	vector<map<GPoint, PointLatLng>> FromPixelToLatLngCache;

	string name;
	bool operator !=(PureProjection& right);

	virtual GSize TileSize() =0;
	virtual double Axis() = 0;
	virtual double Flattening() =0;
	virtual GPoint FromLatLngToPixel(double lat, double lng, int zoom) = 0;
	GPoint FromLatLngToPixel(PointLatLng p, int zoom);
	GPoint FromLatLngToPixel(PointLatLng p, int zoom, bool useCache);
	virtual PointLatLng FromPixelToLatLng(long x, long y, int zoom) = 0;
	PointLatLng FromPixelToLatLng(GPoint p, int zoom);
	PointLatLng FromPixelToLatLng(GPoint p, int zoom, bool useCache);
	virtual GPoint FromPixelToTileXY(GPoint p);
	virtual GPoint FromTileXYToPixel(GPoint p);
	virtual GSize GetTileMatrixMinXY(int zoom) = 0;
	virtual GSize GetTileMatrixMaxXY(int zoom) = 0;
	virtual GSize GetTileMatrixSizeXY(int zoom);
	long GetTileMatrixItemCount(int zoom);
	virtual GSize GetTileMatrixSizePixel(int zoom);
	vector<GPoint> GetAreaTileList(RectLatLng rect, int zoom, int padding);
	virtual double GetGroundResolution(int zoom, double latitude);
	virtual RectLatLng Bounds();
	
	static double Proje_PI;
	static double HALF_PI;
	static double TWO_PI;
	static double EPSLoN;
	static double Max_VAL;
	static double MaxLONG;
	static double DBLLONG;
	static double R2D;
	static double D2R;

	static double DegreesToRadians(double deg);
	static double RadiansToDegrees(double rad);
	static double Sign(double x);
	static double AdjustLongitude(double x);
	static void SinCos(double val, double &sin, double &cos);
	static double e0fn(double x);
	static double e1fn(double x);
	static double e2fn(double x);
	static double e3fn(double x);
	static double mlfn(double e0, double e1, double e2, double e3, double phi);
	static long GetUTMzone(double lon);
	static double Clip(double n, double minValue, double maxValue);

	double GetDistance(PointLatLng p1, PointLatLng p2);
	double GetDistanceInPixels(GPoint point1, GPoint point2);
	double GetBearing(PointLatLng p1, PointLatLng p2);
	void FromGeodeticToCartesian(double Lat, double Lng, double Height, double& X, double& Y, double& Z);
	void FromCartesianTGeodetic(double X, double Y, double Z, double& Lat, double& Lng);
};

