#include "MapMarker.h"
#include "MapControl.h"
#include "MapOverlay.h"
#include "MapGraph.h"
#include "GSize.h"

void MapMarker::initValue()
{
	overlay = NULL;
	visible = true;
	isInsideWindow = false;
	IsHitTestVisible = true;
	isMouseOver = false;
	tooltip.MarkerSet(this);
}

MapMarker::MapMarker(void)
{
	initValue();
}

MapMarker::MapMarker(PointLatLng pos)
{
	initValue();
	this->position = pos;
}


MapMarker::~MapMarker(void)
{
}

MapOverlay* MapMarker::OverlayGet()
{
	return overlay;
}

void MapMarker::OverlaySet(MapOverlay* value)
{
	overlay = value;
}

PointLatLng MapMarker::PositionGet()
{
	return position;
}

void MapMarker::PositionSet(PointLatLng value)
{
	if (position != value)
	{
		position = value;
		if (IsVisibleGet())
		{
			if (overlay != NULL && overlay->ControlGet() != NULL)
			{
				overlay->control->UpdateMarkerLocalPosition(this);
			}
		}
	}
}

bool MapMarker::IsVisibleGet()
{
	return visible;
}

void MapMarker::IsVisibleSet(bool value)
{
	if (visible != value)
	{
		visible = value;
		if (OverlayGet() != NULL && OverlayGet()->ControlGet() != NULL )
		{
			if (visible)
			{
				OverlayGet()->ControlGet()->UpdateMarkerLocalPosition(this);
			}
			else
			{
				if (OverlayGet()->ControlGet()->isMouseOverMarker)
				{
					OverlayGet()->ControlGet()->IsMouseOverMarkerSet(false);
					OverlayGet()->ControlGet()->RestoreCursorOnLeave();
				}
				if (!OverlayGet()->ControlGet()->HoldInvalidation)
				{
					OverlayGet()->ControlGet()->Invalidate();
				}
			}
		}
	}
}

GPoint MapMarker::OffsetGet()
{
	return offset;
}

void MapMarker::OffsetSet(GPoint value)
{
	if (offset != value)
	{
		offset = value;
		if(visible)
		{
			if(overlay != NULL && overlay->ControlGet() != NULL)
			{
				overlay->ControlGet()->UpdateMarkerLocalPosition(this);
			}
		}
	}
}

SDL_Rect MapMarker::LocalPositionGet()
{
	return area;
}

void MapMarker::LocalPositionSet(SDL_Rect value)
{
	if (area.x !=  value.x || area.y !=  value.y || area.w !=  value.w || area.h !=  value.h)
	{
		area.x =  value.x ;
		area.y =  value.y ;
		area.w =  value.w ;
		area.h =  value.h ;
		if (overlay != NULL && overlay->control != NULL)
		{
			if (!overlay->control->HoldInvalidation)
			{
				overlay->control->Invalidate();
			}
		}
	}
}

GPoint MapMarker::ToolTipPosition()
{
	GPoint ret = GPoint(area.x, area.y);
	ret.Offset(-offset.X, -offset.Y);
	return ret;
}

GSize MapMarker::SizeGet()
{
	return GSize(area.w, area.h);
}

void MapMarker::SizeGet(GSize value)
{
	area.w = value.width;
	area.h = value.height;
}

SDL_Rect MapMarker::LocalArea()
{
	return area;
}

bool MapMarker::IsMouseOverGet()
{
	return isMouseOver;
}

void MapMarker::IsMouseOverSet(bool value)
{
	isMouseOver = value;
}

void MapMarker::OnRender(MapGraph & g)
{
}

bool MapMarker::isEmpty()
{
	if (position.IsEmpty())
		return true;
	else
		return false;
}

bool MapMarker::AreaContains(int x, int y)
{
	GetPsInPolygon Juged;
	Juged.polySides = 4;
	Juged.x = x;
	Juged.y	= y;

	Juged.polyX[0] = (double)area.x;
	Juged.polyY[0] = (double)area.y;
	Juged.polyX[1] = (double)(area.x + area.w);
	Juged.polyY[1] = (double)area.y;
	Juged.polyX[2] = (double)(area.x + area.w);
	Juged.polyY[2] = (double)(area.y + area.h);
	Juged.polyX[3] = (double)area.x;
	Juged.polyY[3] = (double)(area.y + area.h);

	return Juged.pointInPolygon();
	//return false;
}

