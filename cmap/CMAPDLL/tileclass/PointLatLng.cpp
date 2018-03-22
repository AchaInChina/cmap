#include "PointLatLng.h"
#include "SizeLatLng.h"

PointLatLng::PointLatLng(void)
{
	lat = 0;
	lng = 0;
	isEmpty = true;
}


PointLatLng::~PointLatLng(void)
{
}

PointLatLng::PointLatLng(double lat, double lng)
{
	this->lat = lat;
	this->lng = lng;
	isEmpty = false;
}

bool PointLatLng::IsEmpty()
{
	return isEmpty;
}

double PointLatLng::LatGet()
{
	return lat;
}

void PointLatLng::LatSet(double lat)
{
	this->lat = lat;
	isEmpty = false;
}

double PointLatLng::LngGet()
{
	return lng;
}

void PointLatLng::LngSet(double lng)
{
	this->lng = lng;
	isEmpty = false;
}

PointLatLng::PointLatLng(SizeLatLng sz)
{
	PointLatLng(sz.heightLat, sz.widthLng);
}

PointLatLng PointLatLng::operator +(SizeLatLng sz)
{
	return Add(*this, sz);
}

PointLatLng PointLatLng::operator -(SizeLatLng sz)
{
	return Subtract(*this, sz);
}

bool PointLatLng::operator ==( PointLatLng right)
{
	return ((this->lng == right.lng) && (this->lat == right.lat));
}

PointLatLng PointLatLng::Add(PointLatLng pt, SizeLatLng sz)
{
	return PointLatLng(pt.lat - sz.heightLat, pt.lng + sz.widthLng);
}

PointLatLng PointLatLng::Subtract(PointLatLng pt, SizeLatLng sz)
{
	return PointLatLng(pt.lat + sz.heightLat, pt.lng - sz.widthLng);
}

bool PointLatLng::operator !=(PointLatLng right)
{
	return !(*this == right);
}

bool PointLatLng::Equals(PointLatLng obj)
{
	return (((obj.lng == this->lng) && (obj.lat == this->lat)));
}

void PointLatLng::Offset(PointLatLng pos)
{
	this->Offset(pos.lat, pos.lng);
}

void PointLatLng::Offset(double lat, double lng)
{
	this->lng += lng;
	this->lat -= lat;
}

string PointLatLng::ToString()
{
	char buff[1024] ={0};
	sprintf(buff, "Lat= %lf , Lng= %lf",this->lat, this->lng);
	return string(buff);
}
