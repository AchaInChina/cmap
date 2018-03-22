#include "GaoDeMapProvider.h"
#include "PureImage.h"
#include "PureProjection.h"
#include "MercatorProjection.h"

GaoDeMapProvider* Instance = NULL;

GaoDeMapProvider::GaoDeMapProvider(void)
{
	MaxZoom = NULL; 
	RefererUrl = "http://www.amap.com/";
	char buff[1024] = {0};
	sprintf(buff, "©{0} 地图源：高德, ©{0} 硕腾, ©{0} 版权所有");
	Copyright = string(buff);
	name = "AMap";
	id = string("AMap");

	overlays.push_back(this);
}

bool GaoDeMapProvider::operator==( GaoDeMapProvider right)
{
	return this->id == right.id;
}

GaoDeMapProvider::~GaoDeMapProvider(void)
{

}

PureProjection* GaoDeMapProvider::Projection()
{
	return MercatorProjection::InstanceGet();
}

vector<MapProvider*>& GaoDeMapProvider::Overlays()
{
	return overlays;
}

string GaoDeMapProvider::Name() 
{
	return name;
}
string GaoDeMapProvider::Id() 
{
	return id;
}

PureImage GaoDeMapProvider::GetTileImage(GPoint pos, int zoom)
{
	string url = MakeTileImageUrl(pos, zoom, LanguageStrGet());
	return GetTileImageUsingHttp(url);
}

string GaoDeMapProvider::MakeTileImageUrl(GPoint pos, int zoom, string language)
{
	char buff[1024] = {0};
	sprintf(buff, "http://webrd01.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=7&x=%d&y=%d&z=%d", pos.X, pos.Y, zoom);
	string url(buff);
	return url;
}

GaoDeMapProvider* GaoDeMapProvider::InstanceGet()
{
	//static GaoDeMapProvider instance;
	return Instance;
}

void GaoDeMapProvider::InstanceDelete()
{
	delete Instance;
}

void GaoDeMapProvider::InstanceGener()
{
	Instance = new GaoDeMapProvider();
}