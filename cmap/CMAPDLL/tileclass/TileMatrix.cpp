#include "TileMatrix.h"
#include "DrawTile.h"
#include "Tile.h"
#include "GPoint.h"

TileMatrix::TileMatrix(void)
{
	for(int i = 0; i < 33; i++)
	{
		map<GPoint, Tile> tempmap;
		Levels.push_back(tempmap);
	}
}


TileMatrix::~TileMatrix(void)
{
}


void TileMatrix::ClearAllLevels()
{
	Lock.Lock();
	vector <map<GPoint, Tile>>::iterator iter;
	for (iter = Levels.begin(); iter != Levels.end(); iter++)
	{
		iter->clear();
	}
	Lock.Unlock();
}

void TileMatrix::ClearLevel(int zoom)
{
	Lock.Lock();
	if (zoom < Levels.size())
	{
		Levels[zoom].clear();
	}
	Lock.Unlock();
}

void TileMatrix::ClearLevelAndPointsNotIn(int zoom, vector<DrawTile> & list)
{
	Lock.Lock();
	if ( zoom < Levels.size() )
	{
		map<GPoint, Tile> l = Levels[zoom];
		for (map<GPoint, Tile>::iterator iter = l.begin(); iter != l.end(); iter ++)
		{
			bool existflag = false;
			for (vector<DrawTile>::iterator it = list.begin(); it != list.end(); it++)
			{
				if (it->PosXY == iter->first)
				{
					existflag = true;
				}
			}
			if (existflag == false)
			{
				tmp.push_back(*iter);
			}
		}
		for ( vector<pair<GPoint,Tile>>::iterator pairtIt = tmp.begin(); pairtIt != tmp.end(); pairtIt ++)
		{
			map<GPoint, Tile>::iterator toDelet = l.find(pairtIt->first);
			l.erase(toDelet);
		}
		tmp.clear();
	}
	Lock.Unlock();
}

void TileMatrix::ClearLevelsBelove(int zoom)
{
	Lock.Lock();
	if (zoom -1 < Levels.size())
	{
		for (int i = zoom -1;  i >= 0; i --)
		{
			map<GPoint, Tile>& l = Levels[i];
			l.clear();
		}
	}
	Lock.Unlock();
}

void TileMatrix::ClearLevelsAbove(int zoom)
{
	Lock.Lock();
	if(zoom + 1 < Levels.size())
	{
		for (int i = zoom +1;  i < Levels.size(); i ++)
		{
			map<GPoint, Tile>& l = Levels[i];
			l.clear();
		}
	}
	Lock.Unlock();
}

Tile TileMatrix::GetTileWithNoLock(int zoom, GPoint p)
{
	Tile ret;
	map<GPoint, Tile>::iterator t  = Levels[zoom].find(p);
	if (t != Levels[zoom].end())
	{
		ret = t->second;
	}
	return ret;
}

Tile TileMatrix::GetTileWithReadLock(int zoom, GPoint p)
{
	Tile  ret;
	//pthread_cleanup_push( LockCleanUp::CleanUpLock, &(Lock));
	Lock.Lock();
	ret =  GetTileWithNoLock(zoom, p);
	Lock.Unlock();
	//pthread_cleanup_pop(0);
	return ret;
}

void TileMatrix::SetTile(Tile t)
{
	//pthread_cleanup_push( LockCleanUp::CleanUpLock, &(Lock));
	Lock.Lock();
	if (t.zoom < Levels.size())
	{
		Levels[t.zoom][t.pos] = t;
	}
	Lock.Unlock();
	//pthread_cleanup_pop(0);
}