#include "BaseMapRoute.h"
#include "EmptyProvider.h"
#include "MercatorProjection.h"

BaseMapRoute::BaseMapRoute(void)
{
	isempty = true;
}


BaseMapRoute::~BaseMapRoute(void)
{
}

PointLatLng BaseMapRoute::From()
{
	if (Points.size() > 0)
	{
		return Points[0];
	}
	return PointLatLng();
}

PointLatLng BaseMapRoute::To()
{
	if (Points.size() > 0)
	{
		return Points[Points.size() - 1];
	}
	return PointLatLng();
}

BaseMapRoute::BaseMapRoute(string name)
{
	Name = name;
}

BaseMapRoute::BaseMapRoute(vector<PointLatLng> points, string name)
{
	Points.assign(points.begin(), points.end());
	Name = name;
}

double BaseMapRoute::Distance()
{
	double distance = 0.0;
	if (Points.size() >= 2)
	{
		for (int i = 1; i < Points.size(); i++)
		{
			distance += EmptyProvider::InstanceGet()->Projection()->GetDistance(Points[i - 1], Points[i]);
		}
	}
	return distance;
}

void BaseMapRoute::Clear()
{
	Points.clear();
	Name = string();
}

bool BaseMapRoute::isEmpty()
{
	if (Points.size() > 0)
	{
		isempty = false;
	}
	return isempty;
}

void BaseMapRoute::CopyToArray(Vertex* DS, MapColor color, GLfloat X, GLfloat Y, GLfloat Z)
{
	DS->r = color.r;
	DS->g = color.g;
	DS->b = color.b;
	DS->a = color.a;
	DS->x = X;
	DS->y = Y;
	DS->z = Z;
}

void BaseMapRoute::AddPoint(PointLatLng value)
{
	Points.push_back(value);
}

void BaseMapRoute::ClearPoint()
{
	Points.clear();
}

SDL_Rect BaseMapRoute::LocalPositionGetFirst()
{
	SDL_Rect area;
	memset( &area, -1, sizeof(SDL_Rect));
	if (LocalPoints.size())
	{
		memset( &area, 0, sizeof(SDL_Rect));
		area.x = LocalPoints[0].X;
		area.y = LocalPoints[0].Y;
	}

	return area;
}