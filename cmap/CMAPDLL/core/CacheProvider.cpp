#include "CacheProvider.h"
#include "GPoint.h"
#include "PureImage.h"
#include "PureProjection.h"
#include "MapProvider.h"

CacheProvider::CacheProvider(void)
{

}


CacheProvider::~CacheProvider(void)
{
}

int CacheProvider::Init()
{
	UseCache = true;
	db.OpenDateBase("test.db");
	db.OpenTable("AMap");
	return 0;
}

PureImage CacheProvider::GetImageFrom(MapProvider * provider, GPoint pos, int zoom)
{
	PureImage img;
	if (UseCache)
	{
		char buff[1024] = {0};
		sprintf(buff, "%d-%d-%d", pos.X, pos.Y, zoom);
		//lock.Lock();
		string data = db.GetKeyValue(buff);
		//lock.Unlock();
		if (data.length() == 0)
		{
			img = provider->GetTileImage(pos, zoom);
			//lock.Lock();
			db.InsertKeyValue(buff, img.Data);
			//lock.Unlock();
		}
		else
		{
			img.Data = data;
		}
		
	}
	else
	{
		img = provider->GetTileImage(pos, zoom);
	}

	return img;
}
