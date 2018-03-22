#include "GRect.h"
#include "GPoint.h"
#include "GSize.h"

GRect::GRect(void)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}


GRect::~GRect(void)
{
}

GRect::GRect(long x, long y, long width, long height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

GRect::GRect(GPoint location, GSize size)
{
	this->x = location.X;
	this->y = location.Y;
	this->width = size.width;
	this->height = size.height;
}

GRect GRect::FromLTRB(int left, int top, int right, int bottom)
{
	return GRect(left,
		top,
		right - left,
		bottom - top);
}

GPoint GRect::LocationGet()
{
	return GPoint(x, y);
}

void GRect::LocationSet(GPoint value)
{
	x = value.X;
	y = value.Y;
}

bool GRect::IsEmpty()
{
	return height == 0 && width == 0 && x == 0 && y == 0;
}

long GRect::Bottom()
{
	return y + height;
}

long GRect::Right()
{
	return y + width;
}

long GRect::Top()
{
	return y;
}

long GRect::Left()
{
	return y;
}

long GRect::HeightGet()
{
	return height;
}

void GRect::HeightSet(long value)
{
	height = value;
}

long GRect::WidthGet()
{
	return width;
}

void GRect::WidthSet(long value)
{
	width = value;
}

long GRect::YGet()
{
	return y;
}

void GRect::YSet(long value)
{
	y = value;
}

long GRect::XGet()
{
	return x;
}

void GRect::XSet(long value)
{
	x = value;
}

GSize GRect::SizeGet()
{
	 return GSize(width, height);
}

void GRect::SizeSet(GSize value)
{
	this->width = value.width;
	this->height = value.height;
}

GPoint GRect::LeftBottom()
{
	return GPoint(Left(), Bottom());
}

GPoint GRect::RightTop()
{
	return GPoint(Right(), Top());
}

GPoint GRect::RightBottom()
{
	return GPoint(Right(), Bottom());
}

bool GRect::Equals(GRect obj)
{
	return (obj.x == this->x	) &&
		(obj.y == this->y) &&
		(obj.width == this->width) &&
		(obj.height == this->height);
}

bool GRect::operator==(GRect right)
{
	return (this->x == right.x 
		&& this->y == right.y 
		&& this->width == right.width
		&& this->height == right.height);
}

bool GRect::operator!=(GRect right)
{
	return !(*this == right);
}

bool GRect::Contains(long x, long y)
{
	return this->x <= x && 
		x < this->x + this->width &&
		this->y	<= y && 
		y < this->y	+ this->height;
}

bool GRect::Contains(GPoint pt)
{
	return Contains(pt.X, pt.Y);
}

bool GRect::Contains(GRect rect)
{
	return (this->x <= rect.x) &&
		((rect.x + rect.width) <= (this->x + this->width )) && 
		(this->y <= rect.y) &&
		((rect.y + rect.height) <= (this->y + this->height));
}

void GRect::Inflate(long width, long height)
{
	this->x -= width;
	this->y -= height;
	this->width += 2*width;
	this->height += 2*height;
}

void GRect::Inflate(GSize size)
{
	Inflate(size.width, size.height);
}

GRect GRect::Inflate(GRect rect, long x, long y)
{
	GRect r = rect;
	r.Inflate(x, y);
	return r;
}

void GRect::Intersect(GRect rect)
{
	GRect result = Intersect(rect, *this);
}

GRect GRect::Intersect(GRect a, GRect b)
{
	long x1 = Max(a.x, b.y);
	long x2 = Min(a.x + a.width, b.x + b.width);
	long y1 = Max(a.y, b.y);
	long y2 = Min(a.y + a.height, b.y + b.height);
	if(x2 >= x1
		&& y2 >= y1)
	{

		return GRect(x1, y1, x2 - x1, y2 - y1);
	}
	return GRect();
}

bool GRect::IntersectsWith(GRect rect)
{
	return (rect.x < this->x + this->width) &&
		(this->x < (rect.x + rect.width)) && 
		(rect.y < this->y + this->height) &&
		(this->y < rect.y + rect.height);
}

GRect GRect::Union(GRect a, GRect b)
{
	long x1 = Min(a.x, b.x);
	long x2 = Max(a.x + a.width, b.x + b.width);
	long y1 = Min(a.y, b.y);
	long y2 = Max(a.y + a.height, b.y + b.height);

	return GRect(x1, y1, x2 - x1, y2 - y1);
}

void GRect::Offset(GPoint pos)
{
	Offset(pos.X, pos.Y);
}

void GRect::OffsetNegative(GPoint pos)
{
	Offset(-pos.X, -pos.Y);
}

void GRect::Offset(long x, long y)
{
	this->x += x;
	this->y += y;
}

string GRect::ToString()
{
	char buff[1024] ={0};
	sprintf(buff, "X= %d , Y= %d , Width= %d , ,Height= %d", this->x, this->y, this->width, this->height);
	return std::string(buff);
}