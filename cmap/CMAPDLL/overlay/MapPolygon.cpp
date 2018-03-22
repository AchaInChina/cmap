#include "MapPolygon.h"
#include "MapOverlay.h"
#include "MapControl.h"
#include "MapGraph.h"
#include "poly2tri.h"
using namespace p2t;
#include "triangle.h"


MapPolygon::MapPolygon(void)
{
	iniValue();
}


MapPolygon::~MapPolygon(void)
{
	if (path)
	{	
		delete[] path;
		path = NULL;
	}
	if (ps)
	{
		delete[] ps;
		ps = NULL;
	}
}

MapPolygon::MapPolygon(vector<PointLatLng> points, string name):BaseMapRoute(points,name)
{
	iniValue();
}

void MapPolygon::iniValue()
{
	visible = true;
	width = 2;
	backcolor.r = 128;
	backcolor.g = 0;
	backcolor.b = 0;
	backcolor.a = 32;
	linecolor.r = 0;
	linecolor.g = 0;
	linecolor.b = 255;
	linecolor.a = 255;
	path = NULL;
	ps = NULL;
	IsHitTestVisible = true;
	isMouseOver = false;
	isempty = false;
	tooltip.PolygonSet(this);
	isInsideWindow = false;
}

MapOverlay* MapPolygon::OverlayGet()
{
	return overlay;
}

void MapPolygon::OverlaySet(MapOverlay* value)
{
	overlay = value;
}

bool MapPolygon::IsVisibleGet()
{
	return visible;
}

void MapPolygon::IsVisibleSet(bool value)
{
	if (value != visible)
	{
		visible = value;
		if (OverlayGet() != NULL && OverlayGet()->ControlGet() != NULL)
		{
			if (visible)
			{
				OverlayGet()->ControlGet()->UpdatePolygonLocalPosition(this);
			}
			else
			{
				if (OverlayGet()->ControlGet()->IsMouseOverOverPolygonGet())
				{
					OverlayGet()->ControlGet()->IsMouseOverOverPolygonSet(false);
				}
			}
			if (!OverlayGet()->ControlGet()->HoldInvalidation)
			{
				OverlayGet()->ControlGet()->Invalidate();
			}
		}
	}
}

bool MapPolygon::IsMouseOverGet()
{
	return isMouseOver;
}

void MapPolygon::IsMouseOverSet(bool value)
{
	isMouseOver = value;
}

template <class C> void FreeClear( C & cntr ) {
	for ( typename C::iterator it = cntr.begin(); 
		it != cntr.end(); ++it ) {
			delete * it;
	}
	cntr.clear();
}

void MapPolygon::UpdateGraphicsPath()
{
	if (path)
	{
		delete[] path;
		path = NULL;
	}
	if (ps)
	{
		delete[] ps;
		ps = NULL;
	}

	if (LocalPoints.size() <= 2)
	{
		return;
	}
	path = new Vertex[ LocalPoints.size() ];

	Vector2dVector a;

	int height = OverlayGet()->ControlGet()->Graph.height;
	int widthincrement = width / 2;
	for (int i = 0; i < LocalPoints.size(); i ++)
	{
		CopyToArray( &path[i], linecolor, LocalPoints[i].X, height - LocalPoints[i].Y, 1);
		a.push_back( Vector2d(LocalPoints[i].X, height - LocalPoints[i].Y));
	}
	Vector2dVector result;
	Triangulate::Process(a,result);
	ps = new Vertex[ result.size() ];
	ps_size = result.size();

	vector< Vector2d >::const_iterator iter = result.begin();
	int iterindex = 0;
	while(iter != result.end())
	{
		CopyToArray( &ps[iterindex ], backcolor, iter->GetX(), iter->GetY(), 1);
		iterindex++;
		iter++;
	}
}

bool MapPolygon::IsInside(int x, int y)
{
	GetPsInPolygon Juged;
	Juged.polySides = LocalPoints.size();
	Juged.x = x;
	Juged.y	= y;

	if (LocalPoints.size() > 2)
	{
		for (int i = 0; i < LocalPoints.size(); i ++)
		{
			Juged.polyX[i] = LocalPoints[i].X;
			Juged.polyY[i] = LocalPoints[i].Y;
		}
		if (Juged.pointInPolygon())
		{
			return true;
		}
	}
	return false;
}

void MapPolygon::OnRender(MapGraph & g)
{
	if (IsVisibleGet())
	{
		if (LocalPoints.size() > 2)
		{
			glInterleavedArrays(GL_C4UB_V3F, 0, ps);
			glDrawArrays( GL_TRIANGLES, 0, ps_size);
			glLineWidth(width);
			glInterleavedArrays( GL_C4UB_V3F, 0, path);
			glDrawArrays( GL_LINE_LOOP, 0, LocalPoints.size());
		}
	}
}

