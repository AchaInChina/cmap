#pragma once
#include "Interface.h"
#include "head.h"
#include "MapGraph.h"
#include "MapOverlay.h"


enum CURSOR{
	Arrow,
	Ibeam,
	Wait,
	CrossHair,
	WaitArrow,
	SizeNWSE,
	SizeNESW,
	SizeWE,
	SizeNS,
	SizeAll,
	No,
	Hand,
};



class EXPORTS_CLASS MapControl:public Interface
{
public:
	MapControl(void);
	~MapControl(void);

	int m_HParent;
	MapCore Core;
	MapGraph Graph;
	//MapOverlay testoverlay;

	bool HoldInvalidation;
	void Invalidate();
	void Refresh(int value);

	int Init(int value);
	bool IsRotated();
	void DrawMap(ReDrawSt value);
	void ReloadMap();
	PointLatLng FromLocalToLatLng(int x, int y);
	GPoint FromLatLngToLocal(PointLatLng point);
	GPoint ApplyRotationInversion(int x, int y);
	int LevelsKeepInMemmoryGet();
	void LevelsKeepInMemmorySet(int value);

	void UpdateMarkerLocalPosition(MapMarker* marker);
	void UpdateRouteLocalPosition(MapRoute* route);
	void UpdatePolygonLocalPosition(MapPolygon* polygon);
	bool WithinVisibleExtents(double xMin, double xMax, double yMin, double yMax);

	void RestoreCursorOnLeave();
	void SetCursorHandOnEnter();


	bool isMouseOverMarker;
	int  overObjectCount;
	SDL_Cursor* cursorBefore;

	SDL_Cursor* SetCursor(SDL_Cursor*);
	SDL_Cursor* GetCursor();
	SDL_Cursor* CreatCursor(CURSOR);
	void ChangeCursor(CURSOR);
	void RollBackCursor();

	void DeleteCursor(SDL_Cursor*);
	bool IsMouseOverMarkerGet();
	void IsMouseOverMarkerSet(bool value);

	bool isMouseOverRoute;
	bool IsMouseOverOverRouteGet();
	void IsMouseOverOverRouteSet(bool value);

	bool isMouseOverPolygon;
	bool IsMouseOverOverPolygonGet();
	void IsMouseOverOverPolygonSet(bool value);

	bool MarkersEnabledGet();
	void MarkersEnabledSet(bool value);

	bool RoutesEnabledGet();
	void RoutesEnabledSet(bool value);

	bool PolygonEnabledGet();
	void PolygonEnabledSet(bool value);

	ObservableCollection<MapOverlay*> Overlays;
	bool Overlays_CollectionChanged(void* sender, NotifyCollectionChangedEventArgs<MapOverlay*> e);
	void OnPaintOverlays();
	void ForceUpdateOverlays();

	int zoom;
	int maxzoom;
	int minzoom;
	int ZoomGet();
	void ZoomSet(int value);
	int MaxZoomGet();
	void MaxZoomSet(int value);
	int MinZoomGet();
	void MinZoomSet(int value);

	bool IsDragging();

	PointLatLng PositionGet();
	void PositionSet(PointLatLng value);

	MapProvider* MapProviderGet();
	void MapProviderSet(MapProvider* value);

	void OnSizeChanged(int w, int h);

	bool IsStartedGet();
	void IsStartedSet(bool value);

	bool isSelected;
	RectLatLng selectedArea;
	PointLatLng selectionStart;
	PointLatLng selectionEnd;
	RectLatLng SelectedAreaGet();
	void SelectedAreaSet(RectLatLng value);
	bool SetZoomToFitRect(RectLatLng rect);


	MOUSE_BUTTON dragButton;

	//委托定义//
	CDelegate<MapMarker*, int> OnMarkerEnter;
	CDelegate<MapMarker*, int> OnMarkerLeave;
	CDelegate<MapMarker*, MouseEventArg> OnMarkerClick;

	CDelegate<MapRoute*, int> OnRouteEnter;
	CDelegate<MapRoute*, int> OnRouteLeave;
	CDelegate<MapRoute*, MouseEventArg> OnRouteClick;

	CDelegate<MapPolygon*, int> OnPolygonEnter;
	CDelegate<MapPolygon*, int> OnPolygonLeave;
	CDelegate<MapPolygon*, MouseEventArg> OnPolygonClick;

	CDelegate< int, int> OnMapDrag;
	CDelegate< int, int> OnMapWheel;
	CDelegate< PointLatLng, int> OnPositionChange;
	CDelegate< RectLatLng, bool> OnSelectionChange;
	CDelegate< int, int> OnMapSizeChange;

	CDelegate< MouseEventArg, int> OnMouseDown;
	CDelegate< MouseEventArg, int> OnMouseUp;
	CDelegate< MouseEventArg, int> OnMouseMove;
	CDelegate< MouseEventArg, int> OnMouseClick;
	CDelegate< MouseEventArg, int> OnMouseDClick;
	CDelegate< MouseEventArg, int> OnMouseWheel;

	CDelegate< int, int> OnMouseEnter;
	CDelegate< int, int> OnMouseLeave;
	CDelegate< int, int> OnMouseFocusGain;
	CDelegate< int, int> OnMouseFocusLost;

	void Release(); //释放当前控件占用的资源

	static void LibInit(); //这是初始化整个库，
	static void LibClear(); //程序退出时，释放库占用的资源
};



