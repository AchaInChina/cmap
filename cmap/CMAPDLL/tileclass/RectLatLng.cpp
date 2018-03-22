#include "RectLatLng.h"
#include "PointLatLng.h"
#include "SizeLatLng.h"

RectLatLng::RectLatLng(void)
{
	NotEmpty = false;
	lng = 0;;
	lat = 0;;
	widthLng = 0;
	heightLat = 0;
}


RectLatLng::~RectLatLng(void)
{
}

RectLatLng::RectLatLng(double lat, double lng, double widthLng, double heightLat)
{
	this->lng = lng;
	this->lat = lat;
	this->widthLng = widthLng;
	this->heightLat = heightLat;
	NotEmpty = true;
}

RectLatLng::RectLatLng(PointLatLng location, SizeLatLng size)
{
	this->lng = location.LngGet();
	this->lat = location.LatGet();
	this->widthLng = size.WidthLngGet();
	this->heightLat = size.HeightLatGet();
	NotEmpty = true;
}

RectLatLng RectLatLng::FromLTRB(double leftLng, double topLat, double rightLng, double bottomLat)
{
	return RectLatLng(topLat, leftLng, rightLng - leftLng, topLat - bottomLat);
}

PointLatLng RectLatLng::LocationTopLeftGet()
{
	return PointLatLng(this->lat, this->lng);
}

void RectLatLng::LocationTopLeftSet(PointLatLng value)
{
	this->lng = value.LngGet();
	this->lat = value.LatGet();
}

PointLatLng RectLatLng::LocationRightBottomGet()
{
	PointLatLng ret =  PointLatLng(this->lat, this->lng);
	ret.Offset(heightLat, widthLng);
	return ret;
}

PointLatLng RectLatLng::LocationMiddle()
{
	PointLatLng ret =  PointLatLng(this->lat, this->lng);
	ret.Offset(heightLat / 2, widthLng / 2);
	return ret;
}

SizeLatLng RectLatLng::SizeGet()
{
	return SizeLatLng(this->heightLat, this->widthLng);
}

void RectLatLng::SizeSet(SizeLatLng value)
{
	this->widthLng = value.widthLng;
	this->heightLat = value.heightLat;
}

double RectLatLng::LngGet()
{
	return this->lng;
}

void RectLatLng::LngSet(double value)
{
	this->lng = value;
}

double RectLatLng::LatGet()
{
	return this->lat;
}

void RectLatLng::LatSet(double value)
{
	this->lat = value;
}

double RectLatLng::WidthLngGet()
{
	return this->widthLng;
}

void RectLatLng::WidthLngSet(double value)
{
	this->widthLng = value;
}

double RectLatLng::HeightLatGet()
{
	return this->heightLat;
}

void RectLatLng::HeightLatSet(double value)
{
	this->heightLat = value;
}

double RectLatLng::LeftGet()
{
	return this->lng;
}

double RectLatLng::Top()
{
	return this->lat;
}

double RectLatLng::Right()
{
	return (this->LngGet() + this->WidthLngGet());
}

double RectLatLng::Bottom()
{
	return (this->LatGet() - this->HeightLatGet());
}

bool RectLatLng::IsEmpty()
{
	return !NotEmpty;
}

bool RectLatLng::Equals(RectLatLng obj)
{
	RectLatLng ef = (RectLatLng)obj;
	return ((((ef.LngGet() == this->LngGet()) && (ef.LatGet() == this->LatGet())) && (ef.WidthLngGet() == this->WidthLngGet())) && (ef.HeightLatGet() == this->HeightLatGet()));
}

bool RectLatLng::operator ==( RectLatLng right)
{
	return ((((this->LngGet() == right.LngGet()) && (this->LatGet() == right.LatGet())) && (this->WidthLngGet() == right.WidthLngGet())) && (this->HeightLatGet()== right.HeightLatGet()));
}

bool RectLatLng::operator !=( RectLatLng right)
{
	return !(*this == right);
}

bool RectLatLng::Contains(double lat, double lng)
{
	return ((((this->LngGet() <= lng) && (lng < (this->LngGet() + this->WidthLngGet()))) && (this->LatGet() >= lat)) && (lat > (this->LatGet() - this->HeightLatGet())));
}

bool RectLatLng::Contains(PointLatLng pt)
{
	return this->Contains(pt.LatGet(), pt.LngGet());
}

bool RectLatLng::Contains(RectLatLng rect)
{
	return ((((this->LngGet() <= rect.LngGet()) && ((rect.LngGet() + rect.WidthLngGet()) <= (this->LngGet() + this->WidthLngGet()))) && (this->LatGet() >= rect.LatGet())) && ((rect.LatGet() - rect.HeightLatGet()) >= (this->LatGet() - this->HeightLatGet())));
}

void RectLatLng::Inflate(double lat, double lng)
{
	this->lng -= lng;
	this->lat += lat;
	this->widthLng += 2.0 * lng;
	this->heightLat += 2.0 * lat;
}

void RectLatLng::Inflate(SizeLatLng size)
{
	this->Inflate(size.HeightLatGet(), size.WidthLngGet());
}

RectLatLng RectLatLng::Inflate(RectLatLng rect, double lat, double lng)
{
	RectLatLng ef = rect;
	ef.Inflate(lat, lng);
	return ef;
}

void RectLatLng::Intersect(RectLatLng rect)
{
	RectLatLng ef = Intersect(rect, *this);
	this->lng = ef.LngGet();
	this->lat = ef.LatGet();
	this->widthLng = ef.WidthLngGet();
	this->heightLat = ef.HeightLatGet();
}

RectLatLng RectLatLng::Intersect(RectLatLng a, RectLatLng b)
{
	double lng = Max(a.LngGet(), b.LngGet());
	double num2 = Min((double)(a.LngGet() + a.WidthLngGet()), (double)(b.LngGet() + b.WidthLngGet()));

	double lat = Max(a.LatGet(), b.LatGet());
	double num4 = Min((double)(a.LatGet() + a.HeightLatGet()), (double)(b.LatGet() + b.HeightLatGet()));

	if((num2 >= lng) && (num4 >= lat))
	{
		return RectLatLng(lat, lng, num2 - lng, num4 - lat);
	}
	
	return RectLatLng();
}

bool RectLatLng::IntersectsWith(RectLatLng a)
{
	return this->LeftGet() < a.Right() && this->Top() > a.Bottom() && this->Right() > a.LeftGet() && this->Bottom() < a.Top();
}

RectLatLng RectLatLng::Union(RectLatLng a, RectLatLng b)
{
	return FromLTRB(
		Min(a.LeftGet(), b.LeftGet()),
		Max(a.Top(), b.Top()),
		Max(a.Right(), b.Right()),
		Min(a.Bottom(), b.Bottom()));
}

void RectLatLng::Offset(PointLatLng pos)
{
	Offset(pos.LatGet(), pos.LngGet());
}

void RectLatLng::Offset(double lat, double lng)
{
	this->lng += lng;
	this->lat -= lat;
}

string RectLatLng::ToString()
{
	char buff[1024] ={0};
	sprintf(buff, "Lat= %d , Lng= %d , WidthLng= %d , HeightLat= %d",this->lat, this->lng, this->widthLng, this->heightLat);
	return std::string(buff);
}