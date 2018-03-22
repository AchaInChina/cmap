#include "Tile.h"

Tile::Tile(void)
{
	this->NotEmpty = false;
	zoom = 0;
	OverlaysCount = 0;
	OverlaysIndex = 0;
}


Tile::~Tile(void)
{
}

Tile::Tile(int zoom, GPoint pos)
{
	this->NotEmpty = true;
	this->zoom = zoom;
	this->pos = pos;
	this->overlays.clear();
	this->OverlaysCount = 0;
	this->OverlaysIndex = -1;
}

PureImage Tile::OverlaysGet()
{
	int size = 0;
	long i = 0;
	if (this->OverlaysIndex != -1)
	{
		i = this->OverlaysIndex;
	}
	for ( size = overlays.size(); i < size; i++)
	{
		this->OverlaysIndex++;
		return overlays[i];
	}
	this->OverlaysIndex =0;
	OverlaysGet();
}

void Tile::AddOverlay(PureImage i)
{
	overlays.push_back(i);
}

bool Tile::HasAnyOverlays()
{
	return overlays.size()>0;
}

int Tile::ZoomGet()
{
	return this->zoom;
}

void Tile::ZoomSet( int value)
{
	this->zoom = value;
}

GPoint Tile::PosGet()
{
	return this->pos;
}

void Tile::PosSet(GPoint value)
{
	this->pos = value;
}

bool Tile::operator ==(Tile m2)
{
	return this->pos == m2.pos && this->zoom == m2.zoom;
}

bool Tile::operator !=(Tile m2)
{
	return this->pos != m2.pos;
}

bool Tile::Equals(Tile m2)
{
	return this->pos == m2.pos && this->zoom == m2.zoom;
}