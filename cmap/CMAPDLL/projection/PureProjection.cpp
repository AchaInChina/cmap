#include "PureProjection.h"
#include "PointLatLng.h"
#include "GPoint.h"
#include "GSize.h"
#include "RectLatLng.h"

double PureProjection::Proje_PI = 4*atan(1.0);
double PureProjection::HALF_PI = (PureProjection::Proje_PI * 0.5);
double PureProjection::TWO_PI = (PureProjection::Proje_PI * 2.0);
double PureProjection::EPSLoN = 1.0e-10;
double PureProjection::Max_VAL = 4;
double PureProjection::MaxLONG = 2147483647;
double PureProjection::DBLLONG = 4.61168601e18;
double PureProjection::R2D = 180 / PureProjection::Proje_PI;
double PureProjection::D2R = PureProjection::Proje_PI / 180;

PureProjection::PureProjection(void)
{
	for(int i = 0; i < 33; i++)
	{
		map<PointLatLng, GPoint> tempmap1;
		FromLatLngToPixelCache.push_back(tempmap1);
		map<GPoint, PointLatLng> tempmap2;
		FromPixelToLatLngCache.push_back(tempmap2);
	}
}

PureProjection::~PureProjection(void)
{
}

GPoint PureProjection::FromLatLngToPixel(PointLatLng p, int zoom)
{
	return FromLatLngToPixel(p, zoom, false);
}

GPoint PureProjection::FromLatLngToPixel(PointLatLng p, int zoom, bool useCache)
{
	if(useCache)
	{
		GPoint ret = GPoint();
		//if(!FromLatLngToPixelCache[zoom].TryGetValue(p, out ret))
		//{
		//	ret = FromLatLngToPixel(p.Lat, p.Lng, zoom);
		//	FromLatLngToPixelCache[zoom].Add(p, ret);

		//	// for reverse cache
		//	if(!FromPixelToLatLngCache[zoom].ContainsKey(ret))
		//	{
		//		FromPixelToLatLngCache[zoom].Add(ret, p);
		//	}

		//	Debug.WriteLine("FromLatLngToPixelCache[" + zoom + "] added " + p + " with " + ret);
		//}
		return ret;
	}
	else
	{
		return FromLatLngToPixel(p.LatGet(), p.LngGet(), zoom);
	}
}

PointLatLng PureProjection::FromPixelToLatLng(GPoint p, int zoom)
{
	return FromPixelToLatLng(p, zoom, false);
}

PointLatLng PureProjection::FromPixelToLatLng(GPoint p, int zoom, bool useCache)
{
	if(useCache)
	{
		PointLatLng ret = PointLatLng();
		//if(!FromPixelToLatLngCache[zoom].TryGetValue(p, out ret))
		//{
		//	ret = FromPixelToLatLng(p.X, p.Y, zoom);
		//	FromPixelToLatLngCache[zoom].Add(p, ret);

		//	// for reverse cache
		//	if(!FromLatLngToPixelCache[zoom].ContainsKey(ret))
		//	{
		//		FromLatLngToPixelCache[zoom].Add(ret, p);
		//	}

		//	Debug.WriteLine("FromPixelToLatLngCache[" + zoom + "] added " + p + " with " + ret);
		//}
		return ret;
	}
	else
	{
		return FromPixelToLatLng(p.X, p.Y, zoom);
	}
}

GPoint PureProjection::FromPixelToTileXY(GPoint p)
{
	return GPoint((long)(p.X / TileSize().WidthGet()), (long)(p.Y / TileSize().HeightGet()));
}

GPoint PureProjection::FromTileXYToPixel(GPoint p)
{
	return GPoint((p.X * TileSize().WidthGet()), (p.Y * TileSize().HeightGet()));
}

GSize PureProjection::GetTileMatrixSizeXY(int zoom)
{
	GSize sMin = GetTileMatrixMinXY(zoom);
	GSize sMax = GetTileMatrixMaxXY(zoom);

	return GSize(sMax.WidthGet() - sMin.WidthGet() + 1, sMax.HeightGet() - sMin.HeightGet() + 1);
}

long PureProjection::GetTileMatrixItemCount(int zoom)
{
	GSize s = GetTileMatrixSizeXY(zoom);
	return (s.WidthGet() * s.HeightGet());
}

GSize PureProjection::GetTileMatrixSizePixel(int zoom)
{
	GSize s = GetTileMatrixSizeXY(zoom);
	return GSize(s.WidthGet() * TileSize().WidthGet(), s.HeightGet() * TileSize().HeightGet());
}

vector<GPoint> PureProjection::GetAreaTileList(RectLatLng rect, int zoom, int padding)
{
	vector<GPoint> ret ;

	GPoint topLeft = FromPixelToTileXY(FromLatLngToPixel(rect.LocationTopLeftGet(), zoom));
	GPoint rightBottom = FromPixelToTileXY(FromLatLngToPixel(rect.LocationRightBottomGet(), zoom));

	for(long x = (topLeft.X - padding); x <= (rightBottom.X + padding); x++)
	{
		for(long y = (topLeft.Y - padding); y <= (rightBottom.Y + padding); y++)
		{
			GPoint p(x, y);
			vector<GPoint>::iterator iter = find(ret.begin(), ret.end(), p);
			if (iter == ret.end())
			{
				if(p.X >= 0 && p.Y >= 0)
				{
					ret.push_back(p);
				}
			}
		}
	}
	return ret;
}

double PureProjection::GetGroundResolution(int zoom, double latitude)
{
	return (cos(latitude * (Proje_PI / 180)) * 2 * Proje_PI * Axis()) / GetTileMatrixSizePixel(zoom).WidthGet();
}

RectLatLng PureProjection::Bounds()
{
	return RectLatLng::FromLTRB(-180, 90, 180, -90);
}

double PureProjection::DegreesToRadians(double deg)
{
	return (D2R * deg);
}

double PureProjection::RadiansToDegrees(double rad)
{
	return (R2D * rad);
}

double PureProjection::Sign(double x)
{
	if(x < 0.0)
		return (-1);
	else
		return (1);
}

double PureProjection::AdjustLongitude(double x)
{
	long count = 0;
	while(true)
	{
		if(abs(x) <= Proje_PI)
			break;
		else
			if(((long)abs(x / Proje_PI)) < 2)
				x = x - (Sign(x) * TWO_PI);
			else
				if(((long)abs(x / TWO_PI)) < MaxLONG)
				{
					x = x - (((long)(x / TWO_PI)) * TWO_PI);
				}
				else
					if(((long)abs(x / (MaxLONG * TWO_PI))) < MaxLONG)
					{
						x = x - (((long)(x / (MaxLONG * TWO_PI))) * (TWO_PI * MaxLONG));
					}
					else
						if(((long)abs(x / (DBLLONG * TWO_PI))) < MaxLONG)
						{
							x = x - (((long)(x / (DBLLONG * TWO_PI))) * (TWO_PI * DBLLONG));
						}
						else
							x = x - (Sign(x) * TWO_PI);
		count++;
		if(count > Max_VAL)
			break;
	}
	return (x);
}

void PureProjection::SinCos(double val, double &sinx, double &cosx)
{
	sinx = sin(val);
	cosx = cos(val);
}

double PureProjection::e0fn(double x)
{
	return (1.0 - 0.25 * x * (1.0 + x / 16.0 * (3.0 + 1.25 * x)));
}

double PureProjection::e1fn(double x)
{
	return (0.375 * x * (1.0 + 0.25 * x * (1.0 + 0.46875 * x)));
}

double PureProjection::e2fn(double x)
{
	return (0.05859375 * x * x * (1.0 + 0.75 * x));
}

double PureProjection::e3fn(double x)
{
	return (x * x * x * (35.0 / 3072.0));
}

double PureProjection::mlfn(double e0, double e1, double e2, double e3, double phi)
{
	return (e0 * phi - e1 * sin(2.0 * phi) + e2 * sin(4.0 * phi) - e3 * sin(6.0 * phi));
}

long PureProjection::GetUTMzone(double lon)
{
	return ((long)(((lon + 180.0) / 6.0) + 1.0));
}

double PureProjection::Clip(double n, double minValue, double maxValue)
{
	return Min((Max(n, minValue)), maxValue);
}

double PureProjection::GetDistance(PointLatLng p1, PointLatLng p2)
{
	double dLat1InRad = p1.LatGet() * (Proje_PI / 180);
	double dLong1InRad = p1.LngGet() * (Proje_PI / 180);
	double dLat2InRad = p2.LatGet() * (Proje_PI / 180);
	double dLong2InRad = p2.LngGet() * (Proje_PI / 180);
	double dLongitude = dLong2InRad - dLong1InRad;
	double dLatitude = dLat2InRad - dLat1InRad;
	double a = pow(sin(dLatitude / 2), 2) + cos(dLat1InRad) * cos(dLat2InRad) * pow(sin(dLongitude / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double dDistance = (Axis() / 1000.0) * c;
	return dDistance;
}

double PureProjection::GetDistanceInPixels(GPoint point1, GPoint point2)
{
	double a = (double)(point2.X - point1.X);
	double b = (double)(point2.Y - point1.Y);

	return sqrt(a * a + b * b);
}

double PureProjection::GetBearing(PointLatLng p1, PointLatLng p2)
{
	auto latitude1 = DegreesToRadians(p1.LatGet());
	auto latitude2 = DegreesToRadians(p2.LatGet());
	auto longitudeDifference = DegreesToRadians(p2.LngGet() - p1.LngGet());

	auto y = sin(longitudeDifference) * cos(latitude2);
	auto x = cos(latitude1) * sin(latitude2) - sin(latitude1) * cos(latitude2) * cos(longitudeDifference);

	return fmod( (RadiansToDegrees(atan2(y, x)) + 360) , 360);
}

void PureProjection::FromGeodeticToCartesian(double Lat, double Lng, double Height, double& X, double& Y, double& Z)
{
	Lat = (Proje_PI / 180) * Lat;
	Lng = (Proje_PI / 180) * Lng;

	double B = Axis() * (1.0 - Flattening());
	double ee = 1.0 - (B / Axis()) * (B / Axis());
	double N = (Axis() / sqrt(1.0 - ee * sin(Lat) * sin(Lat)));

	X = (N + Height) * cos(Lat) * cos(Lng);
	Y = (N + Height) * cos(Lat) * sin(Lng);
	Z = (N * (B / Axis()) * (B / Axis()) + Height) * sin(Lat);
}

void PureProjection::FromCartesianTGeodetic(double X, double Y, double Z, double& Lat, double& Lng)
{
	double E = Flattening() * (2.0 - Flattening());
	Lng = atan2(Y, X);

	double P = sqrt(X * X + Y * Y);
	double Theta = atan2(Z, (P * (1.0 - Flattening())));
	double st = sin(Theta);
	double ct = cos(Theta);
	Lat = atan2(Z + E / (1.0 - Flattening()) * Axis() * st * st * st, P - E * Axis() * ct * ct * ct);

	Lat /= (Proje_PI / 180);
	Lng /= (Proje_PI / 180);
}

bool PureProjection::operator !=(PureProjection& right)
{
	return this->name == right.name;
}