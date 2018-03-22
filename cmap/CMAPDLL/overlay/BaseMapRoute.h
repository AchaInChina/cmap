#pragma once
#include "head.h"
#include "PointLatLng.h"
#include "GPoint.h"
#include "SDL.h"

class EXPORTS_CLASS BaseMapRoute
{
public:
	BaseMapRoute(void);
	~BaseMapRoute(void);

	vector<PointLatLng> Points;
	vector<GPoint> LocalPoints;
	string Name;
	//object Tag;
	PointLatLng From();
	PointLatLng To();
	BaseMapRoute(string name);
	BaseMapRoute(vector<PointLatLng> points, string name);
	bool isempty;
	bool isInsideWindow;
	bool isEmpty();
	double Distance();
	void CopyToArray(Vertex* DS, MapColor color, GLfloat X, GLfloat Y, GLfloat Z);
	void Clear();

	void AddPoint(PointLatLng value);
	void ClearPoint();

	double MinX, MaxX, MinY, MaxY;


	SDL_Rect LocalPositionGetFirst();
};

