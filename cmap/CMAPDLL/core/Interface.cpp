#include "Interface.h"


Interface::Interface(void)
{
}


Interface::~Interface(void)
{
}


PointLatLng Interface::PositionGet()
{
	return Position;
}
void Interface::PositionSet(PointLatLng value)
{
	Position = value;
}

GPoint Interface::PositionPixelGet()
{
	return PositionPixel;
}
void Interface::PositionPixelSet(GPoint value)
{
	PositionPixel = value;
}

string Interface::CacheLocationGet()
{
	return CacheLocation;
}
void Interface::CacheLocationSet(string value)
{
	CacheLocation = value;
}

bool Interface::IsDraggingGet()
{
	return isDragging;
}
void Interface::IsDraggingSet(bool value)
{
	isDragging = value;
}

RectLatLng Interface::ViewAreaGet()
{
	return ViewArea;
}
void Interface::ViewAreaSet(RectLatLng value)
{
	ViewArea = value;
}

MapProvider* Interface::mapProviderGet()
{
	return mapProvider;
}
void Interface::mapProviderSet(MapProvider* value)
{
	mapProvider = value;
}

bool Interface::CanDragMapGet()
{
	return CanDragMap;
}
void Interface::CanDragMapSet(bool value)
{
	CanDragMap = value;
}