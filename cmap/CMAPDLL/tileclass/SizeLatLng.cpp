#include "SizeLatLng.h"
#include "PointLatLng.h"
#include "head.h"
SizeLatLng::SizeLatLng(void)
{
	heightLat = 0;
	widthLng = 0;
}

SizeLatLng::~SizeLatLng(void)
{
}

SizeLatLng::SizeLatLng(const SizeLatLng &size)
{
	this->widthLng = size.widthLng;
	this->heightLat = size.heightLat;
}

SizeLatLng::SizeLatLng(PointLatLng pt)
{
	heightLat = pt.lat;
	widthLng = pt.lng;
}

SizeLatLng::SizeLatLng(double heightLat, double widthLng)
{
	this->heightLat = heightLat;
	this->widthLng = widthLng;
}

SizeLatLng SizeLatLng::Add(SizeLatLng sz1, SizeLatLng sz2)
{
	return SizeLatLng(sz1.heightLat + sz2.heightLat, sz1.widthLng + sz2.widthLng);
}

SizeLatLng SizeLatLng::Subtract(SizeLatLng sz1, SizeLatLng sz2)
{
	return SizeLatLng(sz1.heightLat - sz2.heightLat, sz1.widthLng - sz2.widthLng);
}

bool SizeLatLng::operator==(SizeLatLng sz2)
{
	return ((this->widthLng == sz2.widthLng) && (this->heightLat == sz2.heightLat));
}

bool SizeLatLng::operator!=(SizeLatLng sz2)
{
	return !(*this == sz2);
}

SizeLatLng SizeLatLng::operator+(SizeLatLng sz2)
{
	return Add(*this, sz2);
}

SizeLatLng SizeLatLng::operator-(SizeLatLng sz2)
{
	return Subtract(*this, sz2);
}

bool SizeLatLng::IsEmpty()
{
	return ((abs(this->widthLng) <= DBL_EPSILON) && (abs(this->heightLat) <= DBL_EPSILON));
}

double SizeLatLng::WidthLngGet()
{
	return widthLng;
}

void SizeLatLng::WidthLngSet(double value)
{
	widthLng = value;
}

double SizeLatLng::HeightLatGet()
{
	return heightLat;
}

void SizeLatLng::HeightLatSet(double value)
{
	heightLat = value;
}

bool SizeLatLng::Equals(SizeLatLng obj)
{
	return (((obj.widthLng == this->widthLng) && (obj.heightLat == this->heightLat)));
}

PointLatLng SizeLatLng::ToPointLatLng()
{
	return PointLatLng(*this);
}

string SizeLatLng::ToString()
{
	char buff[1024] ={0};
	sprintf(buff, "WidthLng= %lf , HeightLng= %lf",this->widthLng, this->heightLat);
	return string(buff);
}