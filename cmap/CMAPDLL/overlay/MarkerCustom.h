#pragma once
#ifndef _MARKERCUSTOM_H_
#define _MARKERCUSTOM_H_

#include "MapMarker.h"
#include "ImageConver.h"
extern class MapGraph;

enum MarkerCustomType
{
	SMALL
};

class EXPORTS_CLASS MarkerCustom : public MapMarker
{
public:
	MarkerCustom(void);
	~MarkerCustom(void);
	MarkerCustom(ImageConver bitmap, PointLatLng ps);
	ImageConver pic;

	virtual int HeigthGet() override;
	virtual int WideGet() override;

	void OnRender(MapGraph & g);
};

#endif