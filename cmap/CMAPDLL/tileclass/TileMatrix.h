#pragma once
#include "head.h"

#include "MyMutex.h"
#include "Tile.h"
#include "DrawTile.h"
#include "GPoint.h"

class EXPORTS_CLASS TileMatrix
{
public:
	TileMatrix(void);
	~TileMatrix(void);
public:
	vector<map<GPoint, Tile>> Levels;
	vector<pair<GPoint, Tile>> tmp;
	CMyMutex Lock;
	void ClearAllLevels();
	void ClearLevel(int zoom);
	void ClearLevelAndPointsNotIn(int zoom, vector<DrawTile> &list);
	void ClearLevelsBelove(int zoom);
	void ClearLevelsAbove(int zoom);
	Tile GetTileWithNoLock(int zoom, GPoint p);
	Tile GetTileWithReadLock(int zoom, GPoint p);
	void SetTile(Tile t);
};


