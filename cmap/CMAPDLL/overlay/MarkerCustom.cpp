#include "MarkerCustom.h"
#include "MapGraph.h"

MarkerCustom::MarkerCustom(void)
{
}


MarkerCustom::~MarkerCustom(void)
{
}

MarkerCustom::MarkerCustom(ImageConver bitmap, PointLatLng ps)
{
	pic = bitmap;
	PositionSet(ps);
}

void MarkerCustom::OnRender(MapGraph & g)
{
	SDL_Rect ps;
	ps.x = LocalPositionGet().x;
	ps.y = LocalPositionGet().y;
	
	g.DrawImage(pic, ps.x , ps.y , pic.w, pic.h);
}

int MarkerCustom::HeigthGet()
{
	return pic.h;
}
int MarkerCustom::WideGet()
{
	return pic.w;
}