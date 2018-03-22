#pragma once
#include "head.h"
#include "ObservableCollection.h"

#include "MapRoute.h"
#include "MapPolygon.h"
#include "MapControl.h"
#include "MapGraph.h"

extern class MapMarker;
extern class MapRoute;
extern class MapPolygon;


class EXPORTS_CLASS MapOverlay
{
public:
	MapOverlay(void);
	~MapOverlay(void);

	MapControl* control;
	MapControl* ControlGet();
	void ControlSet(MapControl* value);
	int Init(MapControl* value);

	bool isVisibile;
	bool IsVisibileGet();
	void IsVisibileSet(bool value);

	string Id;

	//list of markers, should be thread safe
	ObservableCollection<MapMarker*> Markers;

	// list of routes, should be thread safe
	ObservableCollection<MapRoute*> Routes;

	// list of routes, should be thread safe
	ObservableCollection<MapPolygon*> Polygon;

	void CreateEvents();
	void ClearEvents();
	void Clear();

	void ForceUpdate();
	void OnRender(MapGraph & g);

	bool Markers_CollectionChanged(void* sender, NotifyCollectionChangedEventArgs<MapMarker*> e);
	bool Routes_CollectionChanged( void* sender, NotifyCollectionChangedEventArgs<MapRoute*> e);
	bool Polygon_CollectionChanged( void* sender, NotifyCollectionChangedEventArgs<MapPolygon*> e);

	bool isEmpty();
};

