#include "ObservableCollection.h"



int SimpleMonitor::_busyCount = 0;

void SimpleMonitor::Enter()
{
	_busyCount++;
}

void SimpleMonitor::Out()
{
	_busyCount--;
}

bool SimpleMonitor::Busy()
{
	return (this->_busyCount > 0);
}





//template class ObservableCollection<MapPolygon*>;
//template class ObservableCollection<MapMarker*>;
//template class ObservableCollection<MapRoute*>;
//template class ObservableCollection<MapOverlay*>;
//
//template class NotifyCollectionChangedEventArgs<MapPolygon*>;
//template class NotifyCollectionChangedEventArgs<MapMarker*>;
//template class NotifyCollectionChangedEventArgs<MapRoute*>;
//template class NotifyCollectionChangedEventArgs<MapOverlay*>;


//void Gentemplate()
//{
//	ObservableCollection<MapPolygon*> tmpMapPolygon;
//	tmpMapPolygon.GenObject();
//	ObservableCollection<MapMarker*> tmpMapMarker;
//	tmpMapMarker.GenObject();
//	ObservableCollection<MapRoute*> tmpMapRoute;
//	tmpMapRoute.GenObject();
//	ObservableCollection<MapOverlay*> tmpMapOverlay;
//	tmpMapOverlay.GenObject();
//
//	NotifyCollectionChangedEventArgs<MapPolygon*> tmpMapPolygonArgs(Add);
//	tmpMapPolygonArgs.GenObject();
//	NotifyCollectionChangedEventArgs<MapMarker*> tmpMapMarkerArgs(Add);
//	tmpMapMarkerArgs.GenObject();
//	NotifyCollectionChangedEventArgs<MapRoute*> tmpMapRouteArgs(Add);
//	tmpMapRouteArgs.GenObject();
//	NotifyCollectionChangedEventArgs<MapOverlay*> tmpMapOverlayArgs(Add);
//	tmpMapOverlayArgs.GenObject();
//}