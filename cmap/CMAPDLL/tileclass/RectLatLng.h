#pragma once
#include "head.h"

extern class PointLatLng;
extern class SizeLatLng;
class EXPORTS_CLASS RectLatLng
{
public:
	RectLatLng(void);
	~RectLatLng(void);
public:
	bool NotEmpty;
	double lng;
	double lat;
	double widthLng;
	double heightLat;
	RectLatLng(double lat, double lng, double widthLng, double heightLat);
	RectLatLng(PointLatLng location, SizeLatLng size);
	static RectLatLng FromLTRB(double leftLng, double topLat, double rightLng, double bottomLat);
	PointLatLng LocationTopLeftGet();
	void LocationTopLeftSet(PointLatLng value);
	PointLatLng LocationRightBottomGet();
	PointLatLng LocationMiddle();
	SizeLatLng SizeGet();
	void SizeSet(SizeLatLng value);
	double LngGet();
	void LngSet(double value);
	double LatGet();
	void LatSet(double value);
	double WidthLngGet();
	void WidthLngSet(double value);
	double HeightLatGet();
	void HeightLatSet(double value);
	double LeftGet();
	double Top();
	double Right();
	double Bottom();
	bool IsEmpty();
	bool Equals(RectLatLng obj);

	bool operator ==( RectLatLng right);
	bool operator !=( RectLatLng right);
	bool Contains(double lat, double lng);
	bool Contains(PointLatLng pt);
	bool Contains(RectLatLng rect);
	void Inflate(double lat, double lng);
	void Inflate(SizeLatLng size);
	RectLatLng Inflate(RectLatLng rect, double lat, double lng);
	void Intersect(RectLatLng rect);
	RectLatLng Intersect(RectLatLng a, RectLatLng b);
	bool IntersectsWith(RectLatLng a);
	RectLatLng Union(RectLatLng a, RectLatLng b);

	void Offset(PointLatLng pos);
	void Offset(double lat, double lng);
	string ToString();
};

