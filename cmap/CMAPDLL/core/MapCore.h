#pragma once
#include "PointLatLng.h"
#include "RectLatLng.h"
#include "GPoint.h"
#include "GSize.h"
#include "GRect.h"
#include "TileMatrix.h"
#include "LoadTask.h"
#include "head.h"
#include "CacheProvider.h"

extern class CMyThreadPool;
extern class CMyMutex;
extern class MapProvider;
extern class MapControl;

enum MouseWheelZoomType
{
	/// <summary>
	/// zooms map to current mouse position and makes it map center
	/// </summary>
	MousePositionAndCenter,

	/// <summary>
	/// zooms to current mouse position, but doesn't make it map center,
	/// google/bing style ;}
	/// </summary>
	MousePositionWithoutCenter,

	/// <summary>
	/// zooms map to current view center
	/// </summary>
	ViewCenter,        
};

#define THREADPOOLLSIZE 5

class EXPORTS_CLASS MapCore
{
public:
	MapCore(void);
	~MapCore(void);
public:
	PointLatLng position;
	GPoint positionPixel;

	GPoint renderOffset;
	GPoint centerTileXYLocation;
	GPoint centerTileXYLocationLast;
	GPoint dragPoint;
	GPoint compensationOffset;

	GPoint mouseDown;
	GPoint mouseCurrent;
	GPoint mouseLastZoom;
	bool MouseWheelZoomEnabled;
	PointLatLng LastLocationInBounds;
	bool VirtualSizeEnabled;
	GSize sizeOfMapArea;
	GSize minOfTiles;
	GSize maxOfTiles;

	GRect tileRect;
	GRect tileRectBearing;
	float bearing;
	bool IsRotated;

	bool fillEmptyTiles;

	TileMatrix Matrix;

	vector<DrawTile> tileDrawingList;
	list<LoadTask> tileLoadQueue;
	CMyMutex tileDrawingListLock;
	CMyMutex tileLoadQueueLock;
	CMyThreadPool* threadpool;
	CMyMutex logMx;
	FILE *filename;

	CMyMutex dragLock;

	static int ProcessLoadTask(LPVOID n);

	static const int GThreadPoolSize = 5;

	time_t LastTileLoadStart;
	time_t LastTileLoadEnd;

	time_t lastFullReDraw;
	time_t FullReDrawTime;

	int loadWaitCount ;
	volatile int okZoom ;
	volatile int skipOverZoom ;

	bool IsStarted;
	int zoom;

	double scaleX ;
	double scaleY ;

	int maxZoom;
	int minZoom;
	int Width;
	int Height;

	int LevelsKeepInMemmory;
	bool RaiseEmptyTileError;
	vector<LoadTask> FailedLoads;

	int pxRes100m;  // 100 meters
	int pxRes1000m;  // 1km  
	int pxRes10km; // 10km
	int pxRes100km; // 100km
	int pxRes1000km; // 1000km
	int pxRes5000km; // 5000km

	MouseWheelZoomType MouseWheelZoomTypeOwn;
	bool IsDragging;
	bool MouseWheelZooming;
	bool updatingBounds;

	bool zoomToArea;
	bool PolygonsEnabled;
	bool RoutesEnabled ;
	bool MarkersEnabled;
	bool CanDragMap;
	int RetryLoadTile;
	string SystemType;
	static int instances ;
	int vWidth;
	int vHeight;
	static const int WaitForTileLoadThreadTimeout = 5 * 1000 * 60;


	MapProvider* Provider;
	MapControl * pMapCtr;
	CacheProvider pCacher;
	

	int Init(MapControl * value);
	MapProvider* ProviderGet();
	void ProviderSet(MapProvider* value);

	int ZoomGet();
	void ZoomSet(int value);
	GPoint PositionPixel();
	PointLatLng PositionGet();
	void PositionSet(PointLatLng value);
	void GoToCurrentPosition();
	void Drag(GPoint pt);
	void UpdateCenterTileXYLocation();
	void UpdateBounds();
	PointLatLng FromLocalToLatLng(long x, long y);
	GPoint FromLatLngToLocal(PointLatLng latlng);
	void GoToCurrentPositionOnZoom();
	void UpdateGroundResolution();
	void OnMapSizeChanged(int width, int height);
	bool SetZoomToFitRect(RectLatLng rect);
	int GetMaxZoomToFitRect(RectLatLng rect);
	void ReloadMap();
	void BeginDrag(GPoint pt);
	void EndDrag();

	//void RefreshOverlay();
	void RefreshAll(int value);

	void Quit();

	static void ResourceInit();
	static void ResourceRelease();
};


