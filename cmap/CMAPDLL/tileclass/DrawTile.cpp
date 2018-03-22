#include "DrawTile.h"




DrawTile::DrawTile(void)
{
	DistanceSqr = 0;
}

DrawTile::~DrawTile(void)
{
}

string DrawTile::ToString()
{
	return  string("PosXY : ") + this->PosXY.ToString() + string("PosPixel : ") + this->PosPixel.ToString();
}

bool DrawTile::Equals(DrawTile other)
{
	return (PosXY == other.PosXY);
}

int DrawTile::CompareTo(DrawTile other)
{
	if (DistanceSqr > other.DistanceSqr)
	{
		return 1;
	} 
	else if(DistanceSqr == DistanceSqr)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

bool DrawTile::operator ==(DrawTile right)
{
	return (PosXY == right.PosXY);
}

bool DrawTile::operator <(DrawTile right)
{
	return DistanceSqr < right.DistanceSqr;
}

bool DrawTile::operator > (DrawTile right)
{
	return DistanceSqr > right.DistanceSqr;
}

bool lesscmp(DrawTile a, DrawTile b)
{
	return a < b;
}