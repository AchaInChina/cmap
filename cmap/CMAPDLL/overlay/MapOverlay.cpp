#include "MapOverlay.h"
#include "MapControl.h"
#include "MapGraph.h"
#include "SDL.h"
#include "SDL_image.h"
#include "PointLatLng.h"
#include "MapMarker.h"
#include "MapRoute.h"
#include "MapPolygon.h"


MapOverlay::MapOverlay(void)
{
	control = NULL;
	isVisibile = true;
	CreateEvents();
}


MapOverlay::~MapOverlay(void)
{
}

int MapOverlay::Init(MapControl* value)
{
	return 0;
}

MapControl* MapOverlay::ControlGet()
{
	return control;
}

void MapOverlay::ControlSet(MapControl* value)
{
	control = value;
}

bool MapOverlay::IsVisibileGet()
{
	return isVisibile;
}

void MapOverlay::IsVisibileSet(bool value)
{
	if (isVisibile != value)
	{
		isVisibile = value;
		if (control != NULL)
		{
			if (isVisibile)
			{
				control->HoldInvalidation = true;
				ForceUpdate();
				control->Refresh(0);
			}
			else
			{
				control->RestoreCursorOnLeave();
				if(!control->HoldInvalidation)
				{
					control->Invalidate();
				}
			}
		}
	}
}


void MapOverlay::CreateEvents()
{
	//注册委托，将当前的指针和回调函数注册到委托器中
	Markers.CollectionChanged.DelegateAdd(this, &MapOverlay::Markers_CollectionChanged);
	Routes.CollectionChanged.DelegateAdd(this, &MapOverlay::Routes_CollectionChanged);
	Polygon.CollectionChanged.DelegateAdd(this, &MapOverlay::Polygon_CollectionChanged);
}

void MapOverlay::ClearEvents()
{
	Markers.CollectionChanged.DelegateDel(this, &MapOverlay::Markers_CollectionChanged);
	Routes.CollectionChanged.DelegateDel(this, &MapOverlay::Routes_CollectionChanged);
	Polygon.CollectionChanged.DelegateDel(this, &MapOverlay::Polygon_CollectionChanged);
}


void MapOverlay::Clear()
{
	Markers.ClearItems();
	Routes.ClearItems();
	Polygon.ClearItems();
}

bool MapOverlay::Markers_CollectionChanged(void* sender, NotifyCollectionChangedEventArgs<MapMarker*> e)
{
	if (e._newItems.size() != 0)
	{
		vector<MapMarker*>::iterator iter = e._newItems.begin();
		while( iter!= e._newItems.end())
		{
			MapMarker* tmp = *iter;
			tmp->overlay = this;
			if (control != NULL)
			{
				control->UpdateMarkerLocalPosition(tmp);
			}
			iter++;
		}
	}

	if (control != NULL)
	{
		if (e.ActionGet() == NotifyCollectionChangedAction::Remove || e.ActionGet() == NotifyCollectionChangedAction::Reset)
		{
			if (control->IsMouseOverMarkerGet())
			{
				control->IsMouseOverMarkerSet(false);
				control->RestoreCursorOnLeave();
			}
		}

		if (!control->HoldInvalidation)
		{
			control->Invalidate();
		}
	}

	return true;
}

bool MapOverlay::Routes_CollectionChanged( void* sender, NotifyCollectionChangedEventArgs<MapRoute*> e)
{
	if (e._newItems.size() != 0)
	{
		vector<MapRoute*>::iterator iter = e._newItems.begin();
		while( iter!= e._newItems.end())
		{
			MapRoute* tmp = *iter;
			tmp->overlay = this;
			if (control != NULL)
			{
				control->UpdateRouteLocalPosition(tmp);
			}
			iter++;
		}
	}

	if (control != NULL)
	{
		if (e.ActionGet() == NotifyCollectionChangedAction::Remove || e.ActionGet() == NotifyCollectionChangedAction::Reset)
		{
			if (control->IsMouseOverOverRouteGet())
			{
				control->IsMouseOverOverRouteSet(false);
				control->RestoreCursorOnLeave();
			}
		}

		if (!control->HoldInvalidation)
		{
			control->Invalidate();
		}
	}
	return true;
}

bool MapOverlay::Polygon_CollectionChanged( void* sender, NotifyCollectionChangedEventArgs<MapPolygon*> e)
{
	if (e._newItems.size() != 0)
	{
		vector<MapPolygon*>::iterator iter = e._newItems.begin();
		while( iter!= e._newItems.end())
		{
			MapPolygon* tmp = *iter;
			tmp->overlay = this;
			if (control != NULL)
			{
				control->UpdatePolygonLocalPosition(tmp);
			}
			iter++;
		}
	}

	if (control != NULL)
	{
		if (e.ActionGet() == NotifyCollectionChangedAction::Remove || e.ActionGet() == NotifyCollectionChangedAction::Reset)
		{
			if (control->IsMouseOverOverPolygonGet())
			{
				control->IsMouseOverOverPolygonSet(false);
				control->RestoreCursorOnLeave();
			}
		}

		if (!control->HoldInvalidation)
		{
			control->Invalidate();
		}
	}
	return true;
}

/// updates local positions of objects
void MapOverlay::ForceUpdate()
{
	if (control != NULL)
	{
		vector<MapMarker*>::iterator iter = Markers.Items.begin();
		while( iter!= Markers.Items.end())
		{
			MapMarker* tmp = *iter;
			if (tmp->IsVisibleGet())
			{
				control->UpdateMarkerLocalPosition(tmp);
			}
			iter++;
		}

		INT64 time1 = GetTickCount();
		vector<MapRoute*>::iterator routeIter = Routes.Items.begin();
		while( routeIter != Routes.Items.end())
		{
			MapRoute* tmp = *routeIter;
			if (tmp->IsVisibleGet())
			{
				control->UpdateRouteLocalPosition(tmp);
			}
			routeIter++;
		}
		INT64 time2 = GetTickCount();
		printf("UpdateRouteLocalPosition use time is %d\n", time2 - time1);

		vector<MapPolygon*>::iterator poligoniter = Polygon.Items.begin();
		while( poligoniter != Polygon.Items.end())
		{
			MapPolygon* tmp = *poligoniter;
			if (tmp->IsVisibleGet())
			{
				control->UpdatePolygonLocalPosition(tmp);
			}
			poligoniter++;
		}
	}
}

/// renders objects/routes/polygons
void MapOverlay::OnRender(MapGraph & g)
{
	if (ControlGet() != NULL)
	{
		if (ControlGet()->RoutesEnabledGet())
		{
			vector<MapRoute*>::iterator iter = Routes.Items.begin();
			while( iter!= Routes.Items.end())
			{
				MapRoute* m = *iter;
				if (m->IsVisibleGet() && m->isInsideWindow)
				{
					m->OnRender(g);
				}
				iter++;
			}
		}

		if (ControlGet()->PolygonEnabledGet())
		{
			vector<MapPolygon*>::iterator iter = Polygon.Items.begin();
			while( iter!= Polygon.Items.end())
			{
				MapPolygon* m = *iter;
				if (m->IsVisibleGet() && m->isInsideWindow)
				{
					m->OnRender(g);
				}
				iter++;
			}
		}

		if (ControlGet()->MarkersEnabledGet())
		{
			vector<MapMarker*>::iterator iter = Markers.Items.begin();
			while( iter!= Markers.Items.end())
			{
				MapMarker* m = *iter;
				if (m->IsVisibleGet() && m->isInsideWindow )
				{
					m->OnRender(g);
				}
				iter++;
			}
		}

		if (ControlGet()->RoutesEnabledGet())
		{
			vector<MapRoute*>::iterator iter = Routes.Items.begin();
			while( iter!= Routes.Items.end())
			{
				MapRoute* m = *iter;
				if (m->IsVisibleGet())
				{
					if (m->tooltiptext.size() > 0 && m->isMouseOver)
					{
						m->tooltip.OnRender(g);
					}
				}
				iter++;
			}
		}

		if (ControlGet()->PolygonEnabledGet())
		{
			vector<MapPolygon*>::iterator iter = Polygon.Items.begin();
			while( iter!= Polygon.Items.end())
			{
				MapPolygon* m = *iter;
				if (m->IsVisibleGet() )
				{
					if (m->tooltiptext.size() > 0 && m->isMouseOver)
					{
						m->tooltip.OnRender(g);
					}
				}
				iter++;
			}
		}

		if (ControlGet()->MarkersEnabledGet())
		{
			vector<MapMarker*>::iterator iter = Markers.Items.begin();
			while( iter!= Markers.Items.end())
			{
				MapMarker* m = *iter;
				if (m->IsVisibleGet() || m->isInsideWindow )
				{
					if (m->tooltiptext.size() > 0 && m->isMouseOver)
					{
						m->tooltip.OnRender(g);
					}
				}
				iter++;
			}
		}
	}
}

bool MapOverlay::isEmpty()
{
	return (Markers.Items.size() == 0);
}