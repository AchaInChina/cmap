#include "GPoint.h"
#include "GSize.h"

GPoint::GPoint(void)
{
	X = 0;
	Y = 0;
	isEmpty = true;
}


GPoint::~GPoint(void)
{
}

GPoint::GPoint(long x, long y)
{
	this->X = x;
	this->Y = y;
	isEmpty = false;
}

bool GPoint::operator==(const GPoint& right)const
{
	return (this->X == right.X) && (this->Y == right.Y);
}

bool GPoint::operator!=( GPoint right)
{
	return !(*this == right);
}

bool GPoint::Equals(GPoint obj)
{
	return (obj.X == this->X) &&
		(obj.Y == this->Y);
}

void GPoint::Offset(long dx, long dy)
{
	X += dx;
	Y += dy;
}
void GPoint::Offset(GPoint p)
{
	Offset(p.X, p.Y);
}
void GPoint::OffsetNegative(GPoint p)
{
	Offset(-p.X, -p.Y);
}
std::string GPoint::ToString()
{
	char buff[1024] ={0};
	sprintf(buff, "X= %d , Y= %d",this->X, this->Y);
	return std::string(buff);
}

GPoint GPoint::Add( GPoint pt, GSize sz)
{
	return GPoint(pt.X + sz.width, pt.Y + sz.height);
}
GPoint::GPoint(GSize sz)
{
	this->X = sz.width;
	this->Y = sz.height;
	isEmpty = false;
}

GPoint GPoint::Subtract(GPoint pt, GSize sz)
{
	return GPoint(pt.X - sz.width, pt.Y - sz.height);
}

GPoint GPoint::operator+(GSize sz)
{
	return Add(*this, sz);
}
GPoint GPoint::operator-(GSize sz)
{
	return Subtract(*this, sz);
}

bool GPoint::operator<( GPoint right)const
{
	if (X < right.X)
	{
		return true;
	}
	else if (X == right.X)
	{
		return (Y < right.Y);
	}

	return false;
}

