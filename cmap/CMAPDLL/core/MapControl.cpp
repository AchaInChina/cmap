#include "MapControl.h"
#include "MercatorProjection.h"
#include "EmptyProvider.h"
#include "GaoDeMapProvider.h"
#include "LoadTask.h"
#include "SDL.h"
#include "SDL_image.h"
#include "head.h"
#include "MapGraph.h"
#include "MapCore.h"
#include "LoadTask.h"

#include "MapMarker.h"
#include "MapRoute.h"
#include "MapPolygon.h"
#include "ImageConver.h"

#include "geos.h"
#include "DelaunayTriangulationBuilder.h"



void MapControl::LibInit()
{
	MercatorProjection::InstanceGener();
	EmptyProvider::InstanceGener();
	GaoDeMapProvider::InstanceGener();
	MapCore::ResourceInit();
	MapGraph::ResourceGener();
	ImageConver::GenerSM();
}
void MapControl::LibClear()
{
	MercatorProjection::InstanceDelete();
	EmptyProvider::InstanceDelete();
	GaoDeMapProvider::InstanceDelete();
	MapCore::ResourceRelease();
	MapGraph::ResourceRelease();
	ImageConver::ReleaseSM();
}

void MapControl::Release()
{
	Overlays.ClearItems();
	Graph.Quit();
	Core.Quit();
}

MapControl::MapControl(void)
{
}

int MapControl::Init(int value)
{
	mapProvider = NULL;
	isDragging = false;
	HoldInvalidation = false;
	isMouseOverMarker = false;
	isMouseOverRoute = false;
	isMouseOverPolygon = false;
	overObjectCount = 0;
	isDragging = false;
	zoom = 0;
	isSelected = false;
	m_HParent = value;
	dragButton = MOUSE_LEFT;



	Graph.Init(this);
	Core.Init(this);

	cursorBefore =NULL;
	//SetCursor(CreatCursor(Hand));


	Graph.ReDrawMode = 3;
	if ( Graph.ReDrawMode == 0)
	{
		Graph.SetEventWatch(MapGraph::AsysGraphEventPro, this);
	}
	else if ( Graph.ReDrawMode == 2)
	{
		Graph.SetEventWatch(MapGraph::AsysGraphEventProNoDraw, this);
	}
	else if ( Graph.ReDrawMode == 3)
	{
		Graph.SetEventWatch(MapGraph::AsysGraphEventProNoDraw, this);
	}		
	Overlays.CollectionChanged.DelegateAdd(this, &MapControl::Overlays_CollectionChanged);
	return 0;
}


MapControl::~MapControl(void)
{
	//GaoDeMapProvider::InstanceDelete();
	//MercatorProjection::InstanceDelete();
}

SDL_Cursor* MapControl::SetCursor(SDL_Cursor* value)
{
	SDL_Cursor* tmp = SDL_GetCursor();
	if (value)
	{
		SDL_SetCursor(value);
	}
	return tmp;
}

SDL_Cursor* MapControl::GetCursor()
{
	SDL_Cursor* tmp = SDL_GetCursor();
	return tmp;
}

SDL_Cursor* MapControl::CreatCursor(CURSOR value)
{
	SDL_Cursor* tmp = NULL;
	if (value == Arrow)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	}
	if (value == Ibeam)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	}
	if (value == Wait)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
	}
	if (value == CrossHair)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	}
	if (value == WaitArrow)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
	}
	if (value == SizeNWSE)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	}
	if (value == SizeNESW)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	}
	if (value == SizeWE)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	}
	if (value == SizeNS)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	}
	if (value == SizeAll)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	}
	if (value == No)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
	}
	if (value == Hand)
	{
		tmp = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	}
	return tmp;
}

void MapControl::DeleteCursor(SDL_Cursor* value)
{
	if (value)
	{
		SDL_FreeCursor(value);
	}
}

void MapControl::ChangeCursor(CURSOR value)
{
	if (cursorBefore)
		DeleteCursor(cursorBefore);
	cursorBefore = SetCursor(CreatCursor(value));
}

void MapControl::RollBackCursor()
{
	if (cursorBefore)
	{
		cursorBefore = SetCursor(cursorBefore);
		DeleteCursor(cursorBefore);
		cursorBefore = NULL;
	}
	else
	{
		cursorBefore = SetCursor(CreatCursor(Arrow));
		DeleteCursor(cursorBefore);
		cursorBefore = NULL;
	}
}

int printcout = 0;

void MapControl::DrawMap(ReDrawSt value)
{
	SDL_Surface *surface;
	Core.tileDrawingListLock.Lock();
	UINT64 time1 = GetTickCount();
	//全部更新
	time_t now = GetTickCount();
	time_t span = now - Core.lastFullReDraw;
	if (value.code == 0)
	{
		if ( span <= 111 || span <= Core.FullReDrawTime)
		{
			Core.tileDrawingListLock.Unlock();
			return;
		}
	}
	if (Graph.ReDrawMode == 0)
	{
		if (value.code == 1)
		{
			if ( span <= Core.FullReDrawTime )
			{
				Core.tileDrawingListLock.Unlock();
				return;
			}
		}
	}
	else if (Graph.ReDrawMode == 1)
	{
		if (value.code == 1)
		{
			if ( span <= Core.FullReDrawTime )
			{
				Core.tileDrawingListLock.Unlock();
				return;
			}
		}
	}
	else
	{
		if ( value.code == 1 )
		{
			if ( span <= 0 )
			{
				Core.tileDrawingListLock.Unlock();
				return;
			}
		}
		if ( value.code == 2 )
		{
			if ( span <= 0 )
			{
				Core.tileDrawingListLock.Unlock();
				return;
			}
		}
	}
	
	Graph.MakeCurrent();
	Graph.ManualUpdateStart();
	Graph.ClearScreen();
	Core.lastFullReDraw = now;

	vector<DrawTile>::iterator iter;
	for (iter = Core.tileDrawingList.begin(); iter	!= Core.tileDrawingList.end(); iter++)
	{
		bool found = false;
		auto tilePoint = iter;
		Core.tileRect.LocationSet(tilePoint->PosPixel);
		Core.tileRect.OffsetNegative(Core.compensationOffset);
		Tile t = Core.Matrix.GetTileWithReadLock(Core.ZoomGet(), tilePoint->PosXY);
		if(t.NotEmpty)
		{
			vector<PureImage>::iterator iterImage;
			for (iterImage = t.overlays.begin(); iterImage != t.overlays.end(); iterImage++)
			{
				auto img = iterImage;
				int length = img->Data.size();
				if (!found)
					found = true;
				if (!img->IsParent)
				{
					ImageConver realimage( img->Data, BMP);
					Graph.DrawImage(realimage, Core.tileRect.x + Core.renderOffset.X, Core.tileRect.y + Core.renderOffset.Y, realimage.w, realimage.h);
				}
			}
		}
#if 1
		else if(Core.ProviderGet()->Projection()->name == "MercatorProjection")
		{
			int zoomOffset = 1;
			Tile parentTile = Tile();
			long Ix = 0;
			while (!parentTile.NotEmpty && zoomOffset < Core.zoom && zoomOffset <= LevelsKeepInMemmoryGet())
			{
				Ix = (long)pow((float)2, zoomOffset);
				parentTile = Core.Matrix.GetTileWithReadLock(Core.zoom - zoomOffset++, GPoint((int)(tilePoint->PosXY.X / Ix), (int)(tilePoint->PosXY.Y / Ix)));
			}

			if (parentTile.NotEmpty)
			{
				long Xoff = abs(tilePoint->PosXY.X - (parentTile.pos.X * Ix));
				long Yoff = abs(tilePoint->PosXY.Y - (parentTile.pos.Y * Ix));

				vector<PureImage>::iterator iterImage;
				for (iterImage = parentTile.overlays.begin(); iterImage != parentTile.overlays.end(); iterImage ++)
				{
					auto img = iterImage;
					if (!found)
						found = true;
					if (!img->IsParent)
					{
						ImageConver realimage( img->Data, BMP);
						SDL_Rect rect;
						rect.x = (Xoff * ( 256 / Ix));
						rect.y = (Yoff * (256 / Ix));
						rect.w = ( 256 / Ix);
						rect.h = (256 / Ix);
						ImageConver realimagecutafter(realimage.Cut(rect.x, rect.y, rect.w, rect.h));
						Graph.DrawImageScaled(realimagecutafter, Core.tileRect.x + Core.renderOffset.X, Core.tileRect.y + Core.renderOffset.Y, 256, 256);
					}
				}
			}
		}
#endif
		if (!found)
		{
			ImageConver empty( GRAY, 256, 256);
			Graph.DrawImage(empty, Core.tileRect.x + Core.renderOffset.X, Core.tileRect.y + Core.renderOffset.Y, empty.w, empty.h);
		}
	}
	UINT64 time3 = GetTickCount();
	OnPaintOverlays();
	UINT64 time2 = GetTickCount();
	printf("draw OnPaintOverlays use time is %d\n",time2 - time3);
	printf("draw full map use time is %d\n",time2 - time1);
	Core.FullReDrawTime = time2 - time1;
	
	Graph.ManualUpdateEnd();
	Core.tileDrawingListLock.Unlock();
}

void MapControl::ReloadMap()
{
	Core.ReloadMap();
}

PointLatLng MapControl::FromLocalToLatLng(int x, int y)
{
	return Core.FromLocalToLatLng(x, y);
}

GPoint MapControl::FromLatLngToLocal(PointLatLng point)
{
	GPoint ret;
	ret = Core.FromLatLngToLocal(point);
	return ret;
}

bool MapControl::IsRotated()
{
	return Core.IsRotated;
}

GPoint MapControl::ApplyRotationInversion(int x, int y)
{
	GPoint ret = GPoint(x, y);

	//if (IsRotated)
	//{
	//	System.Drawing.Point[] tt = new System.Drawing.Point[] { new System.Drawing.Point(x, y) };
	//	rotationMatrixInvert.TransformPoints(tt);
	//	var f = tt[0];

	//	ret.X = f.X;
	//	ret.Y = f.Y;
	//}

	return ret;
}

int MapControl::LevelsKeepInMemmoryGet()
{
	return Core.LevelsKeepInMemmory;
}

void MapControl::LevelsKeepInMemmorySet(int value)
{
	Core.LevelsKeepInMemmory = value;
}

bool MapControl::WithinVisibleExtents(double xMin, double xMax, double yMin, double yMax)
{
	bool result = !(xMin > Core.Width || xMax < 0 || yMin > Core.Height || yMax < 0);
	return result;
};

void MapControl::UpdateMarkerLocalPosition(MapMarker* marker)
{
#if 0
	marker->isInsideWindow = true;
	GPoint p = FromLatLngToLocal(marker->position);
	SDL_Rect localps;
	localps.x = p.X + marker->offset.X;
	localps.y = p.Y + marker->offset.Y;
	localps.w = marker->WideGet();
	localps.h = marker->HeigthGet();
	marker->LocalPositionSet(localps);
#else
	SDL_Rect localps ;
	GPoint p = FromLatLngToLocal(marker->position);
	localps.x = p.X + marker->offset.X;
	localps.y = p.Y + marker->offset.Y;
	localps.w = marker->WideGet();
	localps.h = marker->HeigthGet();
	marker->LocalPositionSet(localps);

#if 1

	marker->isInsideWindow = WithinVisibleExtents( localps.x , localps.x + localps.w, localps.y, localps.y + localps.h);

#else
	CoordinateArraySequence cs;
	cs.add(Coordinate((double)localps.x, (double)(Core.Height - localps.y)));
	cs.add(Coordinate((double)(localps.x + localps.w), (double)(Core.Height - localps.y)));
	cs.add(Coordinate((double)(localps.x + localps.w), (double)(Core.Height - localps.y - localps.h)));
	cs.add(Coordinate((double)localps.x, (double)(Core.Height - localps.y - localps.h)));
	cs.add(Coordinate((double)localps.x, (double)(Core.Height - localps.y)));
	LinearRing * lr = factory.createLinearRing(cs);
	geos::geom::Polygon *poly=factory.createPolygon(lr,NULL);

	const geos::geom::Envelope* tmpEn = poly->getEnvelopeInternal();

	marker->isInsideWindow = WithinVisibleExtents( tmpEn->getMinX(), tmpEn->getMaxX(), tmpEn->getMinY(), tmpEn->getMaxY());

	factory.destroyGeometry(poly);
#endif
	

#endif
}

void MapControl::UpdateRouteLocalPosition(MapRoute* route)
{
	route->isInsideWindow = false;
	route->LocalPoints.clear();
	route->MinX = 0.0;
	route->MaxX = 0.0;
	route->MinY = 0.0;
	route->MaxY = 0.0;
	for (int i = 0; i < route->Points.size(); i ++)
	{
		GPoint p = FromLatLngToLocal(route->Points[i]);
		route->LocalPoints.push_back(p);
		if (i == 0)
		{
			route->MinX = p.X;
			route->MaxX = p.X;
			route->MinY = p.Y;
			route->MaxY = p.Y;
		}
		else
		{
			if (route->MinX > p.X)
				route->MinX = p.X;
			if (route->MaxX < p.X)
				route->MaxX = p.X;
			if (route->MinY > p.Y)
				route->MinY = p.Y;
			if (route->MaxY < p.Y)
				route->MaxY = p.Y;
		}
	}
#if 1

	//route->isInsideWindow = true;
	//route->UpdateGraphicsPath();

	route->isInsideWindow = WithinVisibleExtents( route->MinX, route->MaxX, route->MinY, route->MaxY);
	if (route->isInsideWindow)
	{
		route->UpdateGraphicsPath();
	}

#else
	CoordinateArraySequence cs;
	for (int i = 0; i < route->LocalPoints.size(); i ++)
	{
		cs.add(Coordinate(route->LocalPoints[i].X,route->LocalPoints[i].Y));
	}
	geos::geom::LineString* line = factory.createLineString(cs);

	const geos::geom::Envelope* tmpEn = line->getEnvelopeInternal();

	route->isInsideWindow = WithinVisibleExtents( tmpEn->getMinX(), tmpEn->getMaxX(), tmpEn->getMinY(), tmpEn->getMaxY());

	if (route->isInsideWindow)
	{
		route->UpdateGraphicsPath();
	}

	factory.destroyGeometry(line);

#endif
	
}

void MapControl::UpdatePolygonLocalPosition(MapPolygon* polygon)
{
	polygon->LocalPoints.clear();
	polygon->MinX = 0.0;
	polygon->MaxX = 0.0;
	polygon->MinY = 0.0;
	polygon->MaxY = 0.0;
	for (int i = 0; i < polygon->Points.size(); i ++)
	{
		GPoint p = FromLatLngToLocal(polygon->Points[i]);
		polygon->LocalPoints.push_back(p);
		if (i == 0)
		{
			polygon->MinX = p.X;
			polygon->MaxX = p.X;
			polygon->MinY = p.Y;
			polygon->MaxY = p.Y;
		}
		else
		{
			if (polygon->MinX > p.X)
				polygon->MinX = p.X;
			if (polygon->MaxX < p.X)
				polygon->MaxX = p.X;
			if (polygon->MinY > p.Y)
				polygon->MinY = p.Y;
			if (polygon->MaxY < p.Y)
				polygon->MaxY = p.Y;
		}
	}
#if 1

	//polygon->isInsideWindow = true;
	//polygon->UpdateGraphicsPath();

	polygon->isInsideWindow = WithinVisibleExtents( polygon->MinX, polygon->MaxX, polygon->MinY, polygon->MaxY);
	if (polygon->isInsideWindow)
	{
		polygon->UpdateGraphicsPath();
	}

#else
	CoordinateArraySequence cs;
	for (int i = 0; i < polygon->LocalPoints.size(); i ++)
	{
		cs.add(Coordinate(polygon->LocalPoints[i].X,polygon->LocalPoints[i].Y));
	}
	cs.add(Coordinate(polygon->LocalPoints[0].X,polygon->LocalPoints[0].Y));
	LinearRing * lr = factory.createLinearRing(cs);
	geos::geom::Polygon *poly=factory.createPolygon(lr,NULL);

	const geos::geom::Envelope* tmpEn = poly->getEnvelopeInternal();

	polygon->isInsideWindow = WithinVisibleExtents( tmpEn->getMinX(), tmpEn->getMaxX(), tmpEn->getMinY(), tmpEn->getMaxY());

	if (polygon->isInsideWindow)
	{
		polygon->UpdateGraphicsPath();
	}

	factory.destroyGeometry(poly);

#endif
	
}

void MapControl::Invalidate()
{
	Core.RefreshAll(0);
}

void MapControl::Refresh(int value)
{
	HoldInvalidation = false;

	//lock (Core.invalidationLock)
	//{
	//	Core.lastInvalidation = DateTime.Now;
	//}
	Core.RefreshAll(value);
}


void MapControl::RestoreCursorOnLeave()
{
	if (overObjectCount <= 0 )
	{
		overObjectCount = 0;
		RollBackCursor();
	}
}

void MapControl::SetCursorHandOnEnter()
{
	if (overObjectCount <= 0 )
	{
		overObjectCount = 0;

		ChangeCursor(Hand);
	}
}

bool MapControl::IsMouseOverMarkerGet()
{
	return isMouseOverMarker;
}

void MapControl::IsMouseOverMarkerSet(bool value)
{
	isMouseOverMarker = value;
	overObjectCount += value ? 1: -1;
}

bool MapControl::IsMouseOverOverRouteGet()
{
	return isMouseOverRoute;
}

void MapControl::IsMouseOverOverRouteSet(bool value)
{
	isMouseOverRoute = value;
	overObjectCount += value ? 1: -1;
}

bool MapControl::IsMouseOverOverPolygonGet()
{
	return isMouseOverPolygon;
}

void MapControl::IsMouseOverOverPolygonSet(bool value)
{
	isMouseOverPolygon = value;
	overObjectCount += value ? 1: -1;
}

bool MapControl::MarkersEnabledGet()
{
	return Core.MarkersEnabled;
}

void MapControl::MarkersEnabledSet(bool value)
{
	Core.MarkersEnabled = value;
}

bool MapControl::RoutesEnabledGet()
{
	return Core.RoutesEnabled;
}

void MapControl::RoutesEnabledSet(bool value)
{
	Core.RoutesEnabled = value;
}

bool MapControl::PolygonEnabledGet()
{
	return Core.PolygonsEnabled;
}

void MapControl::PolygonEnabledSet(bool value)
{
	Core.PolygonsEnabled = value;
}

void MapControl::OnPaintOverlays()
{
	vector<MapOverlay*>::iterator iter = Overlays.Items.begin();
	while(iter != Overlays.Items.end())
	{
		if ((*iter)->IsVisibileGet())
		{
			(*iter)->OnRender(Graph);
		}
		iter++;
	}
}

bool MapControl::Overlays_CollectionChanged(void* sender, NotifyCollectionChangedEventArgs<MapOverlay*> e)
{
	if (e._newItems.size() != 0)
	{
		vector<MapOverlay*>::iterator iter = e._newItems.begin();
		while(iter != e._newItems.end())
		{
			(*iter)->control = this;
			iter++;
		}

		if (Core.IsStarted && !HoldInvalidation)
		{
			Invalidate();
		}
	}
	return true;
}

void MapControl::ForceUpdateOverlays()
{
	HoldInvalidation = true;
	vector<MapOverlay*>::iterator iter = Overlays.Items.begin();
	while(iter != Overlays.Items.end())
	{
		if ( (*iter)->IsVisibileGet() )
		{
			(*iter)->ForceUpdate();
		}
		iter++;
	}
	Refresh(1);
}

int MapControl::ZoomGet()
{
	return zoom;
}

void MapControl::ZoomSet(int value)
{
	if (zoom != value)
	{
		if (value > MaxZoomGet())
		{
			zoom = MaxZoomGet();
		}
		else if (value < MinZoomGet())
		{
			zoom = MinZoomGet();
		}
		else
		{
			zoom = value;
		}
		Core.ZoomSet(zoom);

		if (Core.IsStarted && !IsDragging())
		{
			ForceUpdateOverlays();
		}
	}
}

int MapControl::MaxZoomGet()
{
	return maxzoom;
}

void MapControl::MaxZoomSet(int value)
{
	maxzoom = value;
	Core.maxZoom = value;

}


int MapControl::MinZoomGet()
{
	return minzoom;
}

void MapControl::MinZoomSet(int value)
{
	minzoom	= value;
	Core.minZoom = value;

}

bool MapControl::IsDragging()
{
	return isDragging;
}

PointLatLng MapControl::PositionGet()
{
	return Core.PositionGet();
}

void MapControl::PositionSet(PointLatLng value)
{
	Core.PositionSet(value);
	Position = value;
	if (Core.IsStarted)
	{
		ForceUpdateOverlays();
	}
}

MapProvider* MapControl::MapProviderGet()
{
	return Core.Provider;
}

void MapControl::MapProviderSet(MapProvider* value)
{
	if (Core.ProviderGet() == NULL )
	{
		Core.ProviderSet(value);
	}
	else
	{
		if (!Core.Provider->Equals(value))
		{
			Core.ProviderSet(value);
		}
	}
	OnSizeChanged(Core.Width, Core.Height);
	IsStartedSet(true);
}

void MapControl::OnSizeChanged(int w, int h)
{
	Core.Width = w;
	Core.Height = h;
	Core.OnMapSizeChanged(w, h);

	if (Core.IsStarted)
	{
		ForceUpdateOverlays();
	}
}

bool MapControl::IsStartedGet()
{
	return Core.IsStarted;
}

void MapControl::IsStartedSet(bool value)
{
	Core.IsStarted = value;
}

RectLatLng MapControl::SelectedAreaGet()
{
	return selectedArea;
}

void MapControl::SelectedAreaSet(RectLatLng value)
{
	selectedArea = value;
	if (Core.IsStarted)
	{
		Invalidate();
	}
}

bool MapControl::SetZoomToFitRect(RectLatLng rect)
{
	int maxZoom = Core.GetMaxZoomToFitRect(rect);
	if (maxZoom > 0)
	{
		if (maxZoom > MaxZoomGet())
		{
			maxZoom = MaxZoomGet();
		}

		if (ZoomGet() != maxZoom)
		{
			ZoomSet(maxZoom);
		}
		return true;
	}

	 return false;
}


