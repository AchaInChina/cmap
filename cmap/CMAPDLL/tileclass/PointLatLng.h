#pragma once
#include "head.h"
extern class SizeLatLng;
class EXPORTS_CLASS PointLatLng
{
public:
	PointLatLng(void);
	~PointLatLng(void);
public:
	double lat;
	double lng;
	bool isEmpty;
	PointLatLng(double lat, double lng);
	PointLatLng(SizeLatLng sz);
	bool IsEmpty();
	double LatGet();
	void LatSet(double lat);
	double LngGet();
	void LngSet(double lng);
	PointLatLng operator +(SizeLatLng sz);
	PointLatLng operator -(SizeLatLng sz);
	bool operator ==( PointLatLng right);
	bool operator !=(PointLatLng right);

	PointLatLng Add(PointLatLng pt, SizeLatLng sz);
	PointLatLng Subtract(PointLatLng pt, SizeLatLng sz);
	bool Equals(PointLatLng obj);
	void Offset(PointLatLng pos);
	void Offset(double lat, double lng);
	string ToString();
};

