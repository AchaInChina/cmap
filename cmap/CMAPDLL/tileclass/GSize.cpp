#include "GSize.h"
#include "GPoint.h"

GSize::GSize(void)
{
	width = 0;
	height = 0;
}


GSize::~GSize(void)
{
}

GSize::GSize(GPoint pt)
{
	width = pt.X;
	height = pt.Y;
}

GSize::GSize(long Width, long Height)
{
	width = Width;
	height = Height;
}

GSize GSize::operator +(GSize sz2)
{
	return Add(*this, sz2);
}

GSize GSize::operator -(GSize sz2)
{
	return Subtract(*this, sz2);
}

bool GSize::operator ==(GSize sz2)
{
	return this->width == sz2.width && this->height == sz2.height;
}
bool GSize::operator !=(GSize sz2)
{
	return !(*this == sz2);
}

GSize GSize::Add(GSize sz1, GSize sz2)
{
	return GSize(sz1.width + sz2.width, sz1.height + sz2.height);
}
GSize GSize::Subtract(GSize sz1, GSize sz2)
{
	return GSize(sz1.width - sz2.width, sz1.height - sz2.height);
}
bool GSize::Equals(GSize obj)
{
	return (obj.width == this->width) &&
		(obj.height == this->height);
}
std::string GSize::ToString()
{
	char buff[1024] ={0};
	sprintf(buff, "Width= %d , Height= %d",this->width, this->height);
	return std::string(buff);
}