#include "EmptyProvider.h"
#include "PureImage.h"
#include "PureProjection.h"
#include "MercatorProjection.h"

EmptyProvider* EmptyProvider::Instance = NULL;

EmptyProvider::EmptyProvider(void)
{
}

EmptyProvider::~EmptyProvider(void)
{
	//if (Instance != NULL)
	//{
	//	delete Instance;
	//}
	//Instance = NULL;
}

PureImage EmptyProvider::GetTileImage(GPoint pos, int zoom)
{
	return PureImage();
}

string EmptyProvider::Name()
{
	return string();
}

string EmptyProvider::Id()
{
	return string();
}

PureProjection* EmptyProvider::Projection()
{
	return MercatorProjection::InstanceGet();
}

EmptyProvider* EmptyProvider::InstanceGet()
{
	return Instance;
}

vector<MapProvider*>& EmptyProvider::Overlays()
{
	return overlays;
}

void EmptyProvider::InstanceDelete()
{
	delete Instance ;
}

void EmptyProvider::InstanceGener()
{
	Instance = new EmptyProvider();
}