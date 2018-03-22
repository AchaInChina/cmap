#include "MapToolTip.h"
#include "MapMarker.h"
#include "MapRoute.h"
#include "MapPolygon.h"

#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL.h"
#include "MapGraph.h"
#include "ImageConver.h"

MapToolTip::MapToolTip(void)
{
	iniValue();
}


MapToolTip::~MapToolTip(void)
{
}

MapToolTip::MapToolTip(MapMarker* value)
{
	iniValue();
	pMarker = value;
}

void MapToolTip::iniValue()
{
	pMarker = NULL;
	pRoute = NULL;
	pPolygon =NULL;
	pSurface = NULL;
	style = TTF_STYLE_NORMAL;

	m_fontColor = MapColor(0,0,0);
	m_BackColor = MapColor(240, 248, 255, 222);

	Offset.X = 14;
	Offset.Y = -44;
}

MapMarker* MapToolTip::MarkerGet()
{
	return pMarker;
}

void MapToolTip::MarkerSet(MapMarker* value)
{
	pMarker = value;
}

void MapToolTip::RouteSet(MapRoute* value)
{
	pRoute = value;
}
void MapToolTip::PolygonSet(MapPolygon* value)
{
	pPolygon = value;
}

void MapToolTip::OnRender(MapGraph & g)
{
	if (pMarker)
	{
		ImageConver dssurfacetmp = g.m_font.getCompleteSurface((char *)pMarker->tooltiptext.c_str(), m_fontColor, m_BackColor);
		if (dssurfacetmp.pSurface )
		{
			int x0 = pMarker->LocalPositionGet().x;
			int y0 = pMarker->LocalPositionGet().y;
			int x1 = pMarker->LocalPositionGet().x + Offset.X;
			int y1 = pMarker->LocalPositionGet().y + Offset.Y;
			int x2 = x1 + dssurfacetmp.w; 
			int y2 = y1;
			int x3 = x2;
			int y3 = y2 - dssurfacetmp.h;
			int x4 = x1;
			int y4 = y1 - dssurfacetmp.h;
			g.DrawImage(dssurfacetmp, pMarker->LocalPositionGet().x + Offset.X, pMarker->LocalPositionGet().y- dssurfacetmp.h + Offset.Y, dssurfacetmp.w, dssurfacetmp.h);
			g.DrawLine( x0, y0, x1 + (10 / 4), y1 - (10 / 4), 2, 25, 25, 112, 140);
			g.DrawLine( x1 - 1 + 10, y1 + 1, x2 + 1 - 10, y2 + 1, 2, 25, 25, 112, 140);
			g.DrawLine( x2 + 1, y2 + 1 - 10, x3 + 1, y3 - 1 + 10, 2, 25, 25, 112, 140);
			g.DrawLine( x3 + 1 - 10, y3 - 1, x4 - 1 + 10, y4 - 1, 2, 25, 25, 112, 140);
			g.DrawLine( x4 - 1, y4 - 1 + 10, x1 - 1, y1 + 1 - 10, 2, 25, 25, 112, 140);
			g.DrawArc(x1 + 10 - 1, y1 - 10 + 1, 10, 2, 180.0, 90.0, 25, 25, 112, 140);
			g.DrawArc(x2 - 10 + 1, y2 - 10 + 1, 10, 2, 90.0, 90.0, 25, 25, 112, 140);
			g.DrawArc(x3 - 10 + 1, y3 + 10 - 1, 10, 2, 0.0, 90.0, 25, 25, 112, 140);
			g.DrawArc(x4 + 10 - 1, y4 + 10 - 1, 10, 2, 270.0, 90.0, 25, 25, 112, 140);
		}
		
	}
	if (pRoute)
	{
		if (pRoute->Points.size())
		{
			ImageConver dssurfacetmp = g.m_font.getCompleteSurface((char *)pRoute->tooltiptext.c_str(), m_fontColor, m_BackColor);
			if (dssurfacetmp.pSurface )
			{
				int x0 = pRoute->LocalPositionGetFirst().x;
				int y0 = pRoute->LocalPositionGetFirst().y;
				int x1 = pRoute->LocalPositionGetFirst().x + Offset.X;
				int y1 = pRoute->LocalPositionGetFirst().y + Offset.Y;
				int x2 = x1 + dssurfacetmp.w; 
				int y2 = y1;
				int x3 = x2;
				int y3 = y2 - dssurfacetmp.h;
				int x4 = x1;
				int y4 = y1 - dssurfacetmp.h;
				g.DrawImage(dssurfacetmp, pRoute->LocalPositionGetFirst().x + Offset.X, pRoute->LocalPositionGetFirst().y- dssurfacetmp.h + Offset.Y, dssurfacetmp.w, dssurfacetmp.h);
				g.DrawLine( x0, y0, x1 + (10 / 4), y1 - (10 / 4), 2, 25, 25, 112, 140);
				g.DrawLine( x1 - 1 + 10, y1 + 1, x2 + 1 - 10, y2 + 1, 2, 25, 25, 112, 140);
				g.DrawLine( x2 + 1, y2 + 1 - 10, x3 + 1, y3 - 1 + 10, 2, 25, 25, 112, 140);
				g.DrawLine( x3 + 1 - 10, y3 - 1, x4 - 1 + 10, y4 - 1, 2, 25, 25, 112, 140);
				g.DrawLine( x4 - 1, y4 - 1 + 10, x1 - 1, y1 + 1 - 10, 2, 25, 25, 112, 140);
				g.DrawArc(x1 + 10 - 1, y1 - 10 + 1, 10, 2, 180.0, 90.0, 25, 25, 112, 140);
				g.DrawArc(x2 - 10 + 1, y2 - 10 + 1, 10, 2, 90.0, 90.0, 25, 25, 112, 140);
				g.DrawArc(x3 - 10 + 1, y3 + 10 - 1, 10, 2, 0.0, 90.0, 25, 25, 112, 140);
				g.DrawArc(x4 + 10 - 1, y4 + 10 - 1, 10, 2, 270.0, 90.0, 25, 25, 112, 140);
			}
		}
	}
	if (pPolygon)
	{
		if (pPolygon->Points.size())
		{
			ImageConver dssurfacetmp = g.m_font.getCompleteSurface((char *)pPolygon->tooltiptext.c_str(), m_fontColor, m_BackColor);
			if (dssurfacetmp.pSurface )
			{
				int x0 = pPolygon->LocalPositionGetFirst().x;
				int y0 = pPolygon->LocalPositionGetFirst().y;
				int x1 = pPolygon->LocalPositionGetFirst().x + Offset.X;
				int y1 = pPolygon->LocalPositionGetFirst().y + Offset.Y;
				int x2 = x1 + dssurfacetmp.w; 
				int y2 = y1;
				int x3 = x2;
				int y3 = y2 - dssurfacetmp.h;
				int x4 = x1;
				int y4 = y1 - dssurfacetmp.h;
				g.DrawImage(dssurfacetmp, pPolygon->LocalPositionGetFirst().x + Offset.X, pPolygon->LocalPositionGetFirst().y- dssurfacetmp.h + Offset.Y, dssurfacetmp.w, dssurfacetmp.h);
				g.DrawLine( x0, y0, x1 + (10 / 4), y1 - (10 / 4), 2, 25, 25, 112, 140);
				g.DrawLine( x1 - 1 + 10, y1 + 1, x2 + 1 - 10, y2 + 1, 2, 25, 25, 112, 140);
				g.DrawLine( x2 + 1, y2 + 1 - 10, x3 + 1, y3 - 1 + 10, 2, 25, 25, 112, 140);
				g.DrawLine( x3 + 1 - 10, y3 - 1, x4 - 1 + 10, y4 - 1, 2, 25, 25, 112, 140);
				g.DrawLine( x4 - 1, y4 - 1 + 10, x1 - 1, y1 + 1 - 10, 2, 25, 25, 112, 140);
				g.DrawArc(x1 + 10 - 1, y1 - 10 + 1, 10, 2, 180.0, 90.0, 25, 25, 112, 140);
				g.DrawArc(x2 - 10 + 1, y2 - 10 + 1, 10, 2, 90.0, 90.0, 25, 25, 112, 140);
				g.DrawArc(x3 - 10 + 1, y3 + 10 - 1, 10, 2, 0.0, 90.0, 25, 25, 112, 140);
				g.DrawArc(x4 + 10 - 1, y4 + 10 - 1, 10, 2, 270.0, 90.0, 25, 25, 112, 140);
			}
		}
	}
}

