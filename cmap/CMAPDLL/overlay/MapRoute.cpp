#include "MapRoute.h"
#include "MapOverlay.h"
#include "MapControl.h"
#include "MapGraph.h"

#include "geos.h"
#include "DelaunayTriangulationBuilder.h"

void MapRoute::iniValue()
{
	visible = true;
	width = 4;
	color.r = 128;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	path = NULL;
	IsHitTestVisible = true;
	isMouseOver = false;
	isempty = false;
	tooltip.RouteSet(this);
	isInsideWindow = false;
}

MapRoute::MapRoute(void)
{
	iniValue();
}

MapRoute::~MapRoute(void)
{
	if (path)
	{	
		delete[] path;
		path = NULL;
	}
}

MapRoute::MapRoute(string name):BaseMapRoute(name)
{
	iniValue();
}

MapRoute::MapRoute(vector<PointLatLng> points, string name):BaseMapRoute(points,name)
{
	iniValue();
}

MapOverlay* MapRoute::OverlayGet()
{
	return overlay;
}

void MapRoute::OverlaySet(MapOverlay* value)
{
	overlay = value;
}

bool MapRoute::IsVisibleGet()
{
	return visible;
}

void MapRoute::IsVisibleSet(bool value)
{
	if (value != visible)
	{
		visible = value;
		if (OverlayGet() != NULL && OverlayGet()->ControlGet() != NULL)
		{
			if (visible)
			{
				OverlayGet()->ControlGet()->UpdateRouteLocalPosition(this);
			}
			else
			{
				if (OverlayGet()->ControlGet()->IsMouseOverOverRouteGet())
				{
					 OverlayGet()->ControlGet()->IsMouseOverOverRouteSet(false);
				}
			}
			if (!OverlayGet()->ControlGet()->HoldInvalidation)
			{
				OverlayGet()->ControlGet()->Invalidate();
			}
		}
	}
}

void MapRoute::UpdateGraphicsPath()
{
	if (path)
	{
		delete[] path;
		path = NULL;
	}
	if (LocalPoints.size() <= 1)
	{
		return;
	}

	//CoordinateArraySequence cs;
	//for (int i = 0; i < LocalPoints.size(); i ++)
	//{
	//	cs.add(Coordinate( LocalPoints[i].X, LocalPoints[i].Y));
	//}
	//geos::geom::LineString* line = factory.createLineString(cs);
	//geos::geom::Geometry* tmppath =  line->buffer(width/2);

	//path = new Vertex[ tmppath->getNumPoints() ];

	//for (int i = 0; i < tmppath->getNumPoints(); i++)
	//{
	//	tmppath->getBoundary();
	//	CopyToArray(&path[i], color, p1.X + Xt, height - (p1.Y - Yt), 1);
	//}

	path = new Vertex[ (LocalPoints.size() - 1)*4];
	int height = OverlayGet()->ControlGet()->Graph.height;
	int widthincrement = width / 2;
	for (int i = 1; i < LocalPoints.size(); i ++)
	{
		GPoint p1, p2;
		if (LocalPoints[ i - 1 ].X <= LocalPoints[i].X)
		{
			p1 = LocalPoints[ i - 1 ];
			p2 = LocalPoints[ i ];
		}
		else
		{
			p1 = LocalPoints[ i ];
			p2 = LocalPoints[ i - 1 ];
		}
		
		double K = 0.0;
		int Xt = 0;
		int Yt = 0;
		double l = widthincrement;

		if (p2.X == p1.X)
		{
			K = 1;
			Xt = widthincrement;
			Yt = 0;
		}
		else
		{
			K = (double)(p2.Y - p1.Y) / (p2.X - p1.X);
			Xt = abs(K * sqrt( (l*l) / (K*K + 1) ));
			Yt = sqrt( (l*l) / (K*K + 1) );
		}

		if (widthincrement <= 1)
		{
			Xt += 1;
			Yt += 1;
		}

		if (K >= 0)
		{
			CopyToArray(&path[(i-1)*4 +0], color, p1.X + Xt, height - (p1.Y - Yt), 1);
			CopyToArray(&path[(i-1)*4 +1], color, p1.X - Xt, height - (p1.Y + Yt), 1);
			CopyToArray(&path[(i-1)*4 +2], color, p2.X - Xt, height - (p2.Y + Yt), 1);
			CopyToArray(&path[(i-1)*4 +3], color, p2.X + Xt, height - (p2.Y - Yt), 1);
		}
		else
		{
			CopyToArray( &path[(i-1)*4 +0], color, p1.X - Xt, height - (p1.Y - Yt), 1);
			CopyToArray( &path[(i-1)*4 +1], color, p1.X + Xt, height - (p1.Y + Yt), 1);
			CopyToArray( &path[(i-1)*4 +2], color, p2.X + Xt, height - (p2.Y + Yt), 1);
			CopyToArray( &path[(i-1)*4 +3], color, p2.X - Xt, height - (p2.Y - Yt), 1);
		}
	}
}

void MapRoute::OnRender(MapGraph & g)
{
	if (IsVisibleGet())
	{
		if (LocalPoints.size() > 1)
		{
			for (int i = 0; i < LocalPoints.size() - 1; i ++)
			{
				glInterleavedArrays( GL_C4UB_V3F, 0, path);
				glDrawArrays( GL_QUADS, i*4, 4);
			}
		}
	}
}

bool MapRoute::IsMouseOverGet()
{
	return isMouseOver;
}

void MapRoute::IsMouseOverSet(bool value)
{
	isMouseOver = value;
}

bool MapRoute::IsInside(int x, int y)
{
	GetPsInPolygon Juged;
	Juged.polySides = 4;
	Juged.x = x;
	Juged.y	= y;
	if (path == NULL)
	{
		return false;
	}

	if (LocalPoints.size() >= 2)
	{
		for (int i = 1; i < LocalPoints.size(); i ++)
		{
			Juged.polyX[0] = path[ (i-1)*4 + 0 ].x;
			Juged.polyY[0] = path[ (i-1)*4 + 0 ].y;
			Juged.polyX[1] = path[ (i-1)*4 + 1 ].x;
			Juged.polyY[1] = path[ (i-1)*4 + 1 ].y;
			Juged.polyX[2] = path[ (i-1)*4 + 2 ].x;
			Juged.polyY[2] = path[ (i-1)*4 + 2 ].y;
			Juged.polyX[3] = path[ (i-1)*4 + 3 ].x;
			Juged.polyY[3] = path[ (i-1)*4 + 3 ].y;
			if (Juged.pointInPolygon())
			{
				return true;
			}
		}
	}
	return false;
}
