#pragma once

extern class PointLatLng;
#include "head.h"
class EXPORTS_CLASS SizeLatLng
{
public:
	SizeLatLng(void);
	~SizeLatLng(void);
public:
	static SizeLatLng Empty;
	double heightLat;
	double widthLng;
	SizeLatLng(const SizeLatLng &size);
	SizeLatLng(PointLatLng pt);
	SizeLatLng(double heightLat, double widthLng);
	SizeLatLng operator+(SizeLatLng sz2);
	SizeLatLng operator-(SizeLatLng sz2);
	bool operator==(SizeLatLng sz2);
	bool operator!=(SizeLatLng sz2);

	SizeLatLng Add(SizeLatLng sz1, SizeLatLng sz2);
	SizeLatLng Subtract(SizeLatLng sz1, SizeLatLng sz2);
	bool IsEmpty();
	double WidthLngGet();
	void WidthLngSet(double value);
	double HeightLatGet();
	void HeightLatSet(double value);
	bool Equals(SizeLatLng obj);
	PointLatLng ToPointLatLng();
	string ToString();
};

