#include "MapCore.h"
#include "MapGraph.h"
#include "MapControl.h"
#include "curl.h"
#include "MercatorProjection.h"
#include "EmptyProvider.h"
#include "DrawTile.h"
#include "MyThreadPool.h"
#include "MyThread.h"
#include "MyImageGetTask.h"
#include "SqlWarpper.h"


int MapCore::instances = 0;

MapCore::MapCore(void):Provider(NULL)
{
	LastTileLoadStart = time(NULL);
	LastTileLoadEnd = time(NULL);
	lastFullReDraw = GetTickCount();
	FullReDrawTime = 0;
	MouseWheelZoomTypeOwn = MouseWheelZoomType::MousePositionAndCenter;
	MouseWheelZoomEnabled = true;
	LastLocationInBounds = PointLatLng();
	VirtualSizeEnabled = false;
	bearing = 0;
	IsRotated = false;
	fillEmptyTiles = true;
	IsStarted = false;
	scaleX = 1;
	scaleY = 1;
	maxZoom = 2;
	minZoom = 2;
	IsDragging = false;
	zoomToArea = true;
	PolygonsEnabled = true;
	RoutesEnabled = true;
	MarkersEnabled = true;
	CanDragMap = true;
	RetryLoadTile = 0;
	LevelsKeepInMemmory = 5;
	vWidth = 800;
	vHeight = 400;
	MouseWheelZooming = false;
	RaiseEmptyTileError = false;
	updatingBounds = false;
	zoom = 0;
	loadWaitCount = 0;
	okZoom = 0;
	skipOverZoom = 0;
	Provider = NULL;
	filename = NULL;
	threadpool = NULL;
}


MapCore::~MapCore(void)
{

}

void MapCore::Quit()
{
	if (threadpool)
	{
		threadpool->destroyThreadPool();
		delete threadpool;
		threadpool = NULL;

	}

	if (filename)
	{
		fclose(filename);
	}
}

MapProvider* MapCore::ProviderGet()
{
	return Provider;
}

int MapCore::ZoomGet()
{
	return zoom;
}

void MapCore::ZoomSet(int value)
{
	if(zoom != value && !IsDragging)
	{
		zoom = value;

		minOfTiles = Provider->Projection()->GetTileMatrixMinXY(value);
		maxOfTiles = Provider->Projection()->GetTileMatrixMaxXY(value);

		positionPixel = Provider->Projection()->FromLatLngToPixel(PositionGet(), value);

		if(IsStarted)
		{
			//每次改变缩放级别的时候，都要清除当前加载队列中的任务。
			tileLoadQueueLock.Lock();
			tileLoadQueue.clear();
			tileLoadQueueLock.Unlock();

			//内存中保留的缩放级别的范围
			Matrix.ClearLevelsBelove(zoom - LevelsKeepInMemmory);
			Matrix.ClearLevelsAbove(zoom +  LevelsKeepInMemmory);

			{
				FailedLoads.clear();
				RaiseEmptyTileError = true;
			}

			GoToCurrentPositionOnZoom();
			UpdateBounds();

			if (pMapCtr->OnMapWheel.DelegateSize() > 0)
			{
				pMapCtr->OnMapWheel.DelegateDo(0, 0);
			}
		}
	}
}

void MapCore::ProviderSet(MapProvider* value)
{
	if ( Provider == NULL || !Provider->Equals(value))
	{
		bool diffProjection = ( Provider == NULL || (Provider->Projection() != value->Projection()) );
		Provider = value;
		if (!Provider->IsInitializedGet())
		{
			Provider->IsInitializedSet(true);
			Provider->OnInitialized();
		};
		if (diffProjection)
		{
			PureProjection* tmp = Provider->Projection();
			tileRect = GRect(GPoint(), Provider->Projection()->TileSize());
			tileRectBearing = tileRect;
			if (IsRotated)
			{
				tileRectBearing.Inflate(1,1);
			}
			minOfTiles = Provider->Projection()->GetTileMatrixMinXY(ZoomGet());
			maxOfTiles = Provider->Projection()->GetTileMatrixMaxXY(ZoomGet());
			positionPixel = Provider->Projection()->FromLatLngToPixel(PositionGet(), ZoomGet());
		}

		Provider->MaxZoom = maxZoom;
		Provider->MinZoom = minZoom;

		if(IsStarted)
		{
			if (diffProjection)
			{
				OnMapSizeChanged(Width, Height);
			}

			ReloadMap();
			zoomToArea = true;

			if( !Provider->Area.Contains(PositionGet()))
			{
				SetZoomToFitRect(Provider->Area);
				zoomToArea = false;
			}
		}
	}
}

GPoint MapCore::PositionPixel()
{
	return positionPixel;
}

void MapCore::PositionSet(PointLatLng value)
{
	position = value;
	positionPixel = Provider->Projection()->FromLatLngToPixel(value, ZoomGet());
	if(IsStarted)
	{
		if(!IsDragging)
		{
			GoToCurrentPosition();
		}

		if (pMapCtr->OnPositionChange.DelegateSize() > 0)
		{
			pMapCtr->OnPositionChange.DelegateDo(position, 0);
		}
	}
}

PointLatLng MapCore::PositionGet()
{
	return position;
}

void MapCore::GoToCurrentPosition()
{
	compensationOffset = PositionPixel();

	// reset stuff
	renderOffset = GPoint();
	dragPoint = GPoint();

	GPoint d = GPoint(Width / 2, Height / 2);
	this->Drag(d);
}

void MapCore::Drag(GPoint pt)
{
	//dragLock.Lock();
	renderOffset.X = pt.X - dragPoint.X;
	renderOffset.Y = pt.Y - dragPoint.Y;
	UpdateCenterTileXYLocation();

	if(centerTileXYLocation != centerTileXYLocationLast)
	{
		centerTileXYLocationLast = centerTileXYLocation;
		UpdateBounds();
	}

	if(IsDragging)
	{
		LastLocationInBounds = PositionGet();
		PositionSet( FromLocalToLatLng((int)Width / 2, (int)Height / 2) );

		if(pMapCtr->OnMapDrag.DelegateSize() > 0)
		{
			pMapCtr->OnMapDrag.DelegateDo(0, 0);
		}
	}

	//dragLock.Unlock();
}

PointLatLng MapCore::FromLocalToLatLng(long x, long y)
{
	GPoint p = GPoint(x, y);
	p.OffsetNegative(renderOffset);
	p.Offset(compensationOffset);
	return Provider->Projection()->FromPixelToLatLng(p, ZoomGet());
}

GPoint MapCore::FromLatLngToLocal(PointLatLng latlng)
{
	GPoint pLocal = Provider->Projection()->FromLatLngToPixel(latlng, ZoomGet());
	pLocal.Offset(renderOffset);
	pLocal.OffsetNegative(compensationOffset);
	return pLocal;
}

void MapCore::GoToCurrentPositionOnZoom()
{
	compensationOffset = PositionPixel();

	// reset stuff
	renderOffset = GPoint();
	dragPoint = GPoint();

	if(MouseWheelZooming)
	{
		if(MouseWheelZoomTypeOwn != MouseWheelZoomType::MousePositionWithoutCenter)
		{
			GPoint pt = GPoint(-(positionPixel.X - Width / 2), -(positionPixel.Y - Height / 2));
			pt.Offset(compensationOffset);
			renderOffset.X = pt.X - dragPoint.X;
			renderOffset.Y = pt.Y - dragPoint.Y;
		}
		else // without centering
		{
			renderOffset.X = -positionPixel.X - dragPoint.X;
			renderOffset.Y = -positionPixel.Y - dragPoint.Y;
			renderOffset.Offset(mouseLastZoom);
			renderOffset.Offset(compensationOffset);
		}
	}
	else // use current map center
	{
		mouseLastZoom = GPoint();

		GPoint pt = GPoint(-(positionPixel.X - Width / 2), -(positionPixel.Y - Height / 2));
		pt.Offset(compensationOffset);
		renderOffset.X = pt.X - dragPoint.X;
		renderOffset.Y = pt.Y - dragPoint.Y;
	}
	UpdateCenterTileXYLocation();
}

void MapCore::UpdateCenterTileXYLocation()
{
	PointLatLng center = FromLocalToLatLng(Width / 2, Height / 2);
	GPoint centerPixel = Provider->Projection()->FromLatLngToPixel(center, ZoomGet());
	centerTileXYLocation = Provider->Projection()->FromPixelToTileXY(centerPixel);
}


void MapCore::UpdateBounds()
{
	if(!IsStarted || Provider == NULL)
	{
		return;
	}
	updatingBounds = true;

	tileDrawingListLock.Lock();
	tileDrawingList.clear();
	for(long i = (int)floor(-sizeOfMapArea.WidthGet() * scaleX), countI = (int)ceil(sizeOfMapArea.WidthGet() * scaleX); i <= countI; i++)
	{
		for (long j = (int)floor(-sizeOfMapArea.HeightGet() * scaleY), countJ = (int)ceil(sizeOfMapArea.HeightGet() * scaleY); j <= countJ; j++)
		{
			GPoint p = centerTileXYLocation;
			p.X += i;
			p.Y += j;

			if(p.X >= minOfTiles.WidthGet() && p.Y >= minOfTiles.HeightGet() && p.X <= maxOfTiles.WidthGet() && p.Y <= maxOfTiles.HeightGet())
			{
				DrawTile dt = DrawTile();
				{
					dt.PosXY = p,
					dt.PosPixel =  GPoint(p.X * tileRect.WidthGet(), p.Y * tileRect.HeightGet()),
					dt.DistanceSqr = (centerTileXYLocation.X - p.X) * (centerTileXYLocation.X - p.X) + (centerTileXYLocation.Y - p.Y) * (centerTileXYLocation.Y - p.Y);
				};
				vector<DrawTile>::iterator iter;
				iter = find(tileDrawingList.begin(), tileDrawingList.end(), dt);
				if(iter == tileDrawingList.end())
				{
					tileDrawingList.push_back(dt);
				}
			}
		}
	}
	sort(tileDrawingList.begin(), tileDrawingList.end(), lesscmp);
	vector<DrawTile>::iterator iter1;
	for (iter1 = tileDrawingList.begin(); iter1 != tileDrawingList.end(); iter1++)
	{
		LoadTask task = LoadTask(iter1->PosXY, ZoomGet());
		list<LoadTask>::iterator iter2;
		tileLoadQueueLock.Lock();
		if (tileLoadQueue.size()==0)
		{
			tileLoadQueue.push_front(task);
		} 
		else
		{
			iter2 = find(tileLoadQueue.begin(), tileLoadQueue.end(), task);
			if (iter2 == tileLoadQueue.end())
			{
				tileLoadQueue.push_front(task);
			}
		}
		tileLoadQueueLock.Unlock();
	}
	tileDrawingListLock.Unlock();
	printf("cur zoom is %d\n",ZoomGet());
	RefreshAll(0);

	loadWaitCount = 0;
	LastTileLoadStart = time(NULL);

	static int taskId = 0;

	MyImageGetTask* taskUrl = new MyImageGetTask(taskId++);
	taskUrl->pValue = this;
	threadpool->addTask(taskUrl, PRIORITY::HIGH);
	printf("now taskId is %d \n", taskId);


	updatingBounds = false;
}


int MapCore::ProcessLoadTask(LPVOID n)
{
	MapCore* pParent = (MapCore*)n;
	LoadTask task;
	long lastTileLoadTimeMs;
	bool stop = false;

	while (!stop && pParent->IsStarted)
	{
		pParent->LastTileLoadEnd = time(NULL);
		lastTileLoadTimeMs = pParent->LastTileLoadEnd - pParent->LastTileLoadStart;
		pParent->UpdateGroundResolution();

		//pthread_cleanup_push( LockCleanUp::CleanUpLock, &(pParent->tileLoadQueueLock));
		pParent->tileLoadQueueLock.Lock();
		if (pParent->tileLoadQueue.size() == 0)
		{
			//在源码中是如果在进入子线程中，发现当前请求队列中无内容，则执行该操作//
			pParent->tileLoadQueueLock.Unlock();
			//pParent->RefreshAll(2);
			//pthread_cleanup_pop(0);
			return 0;
		}
		task = pParent->tileLoadQueue.back();
		pParent->tileLoadQueue.pop_back();
		pParent->tileLoadQueueLock.Unlock();
		//pthread_cleanup_pop(0);

		Tile m = pParent->Matrix.GetTileWithReadLock(task.Zoom, task.Pos);
		if (!m.NotEmpty)
		{
			Tile t = Tile(task.Zoom, task.Pos);
			vector<MapProvider*>::iterator iter;
			for (iter = pParent->Provider->Overlays().begin(); iter != pParent->Provider->Overlays().end(); iter++)
			{
				PureImage img;
				if (task.Zoom >= pParent->ProviderGet()->MinZoom && task.Zoom <= pParent->ProviderGet()->MaxZoom)
				{
					if (pParent->skipOverZoom == 0 || task.Zoom <= pParent->skipOverZoom)
					{
						if ((*iter)->InvertedAxisY)
						{
							img = pParent->pCacher.GetImageFrom(pParent->Provider, GPoint(task.Pos.XGet(), pParent->maxOfTiles.HeightGet() - task.Pos.YGet()), task.Zoom);
						}
						else
						{
							img = pParent->pCacher.GetImageFrom(pParent->Provider, task.Pos, task.Zoom);
						}
					}
				}

				if (img.Data.size() <= 0 && pParent->okZoom > 0 && pParent->fillEmptyTiles && !strcmp(pParent->Provider->Projection()->name.c_str(),"MercatorProjection"))
				{
					int zoomOffset = task.Zoom > pParent->okZoom ? task.Zoom - pParent->okZoom : 1;
					long Ix = 0;
					GPoint parentTile = GPoint();
					while (img.Data.size() <= 0 && zoomOffset < task.Zoom)
					{
						Ix = (long)pow((float)2, zoomOffset);
						parentTile = GPoint(task.Pos.XGet() / Ix, task.Pos.YGet() / Ix);
						img = pParent->Provider->GetTileImage(parentTile, task.Zoom);
					}

					if (img.Data.size() > 0)
					{
						long Xoff = abs(task.Pos.XGet() - (parentTile.X * Ix));
						long Yoff = abs(task.Pos.YGet() - (parentTile.Y * Ix));

						img.IsParent = true;
						img.Ix = Ix;
						img.Xoff = Xoff;
						img.Yoff = Yoff;
					}
				}
				if (img.Data.size() > 0)
				{
					t.AddOverlay(img);
				}
			}
			
			if (t.HasAnyOverlays() && pParent->IsStarted)
			{
				pParent->Matrix.SetTile(t);
				pParent->RefreshAll(0);
			}
		}
	}
	return 0;
}

void MapCore::UpdateGroundResolution()
{
	double rez = Provider->Projection()->GetGroundResolution(ZoomGet(), PositionGet().LatGet());
	pxRes100m = (int)(100.0 / rez); // 100 meters
	pxRes1000m = (int)(1000.0 / rez); // 1km  
	pxRes10km = (int)(10000.0 / rez); // 10km
	pxRes100km = (int)(100000.0 / rez); // 100km
	pxRes1000km = (int)(1000000.0 / rez); // 1000km
	pxRes5000km = (int)(5000000.0 / rez); // 5000km
}

int round(float f)
{ 
	if ((int)f+0.5>f) 
		return (int)f; 
	else 
		return (int)f + 1;   
}

void MapCore::OnMapSizeChanged(int width, int height)
{
	this->Width = width;
	this->Height = height;
	if(IsRotated)
	{
		int diag = (int)round(double(sqrt(double(Width * Width + Height * Height)) / Provider->Projection()->TileSize().WidthGet()));
		sizeOfMapArea.WidthSet(1 + (diag / 2));
		sizeOfMapArea.HeightSet(1 + (diag / 2));
	}
	else
	{
		sizeOfMapArea.WidthSet(1 + (Width / Provider->Projection()->TileSize().WidthGet()) / 2);
		sizeOfMapArea.HeightSet(1 + (Height / Provider->Projection()->TileSize().HeightGet()) / 2);
	}
	if(IsStarted)
	{
		UpdateBounds();
		GoToCurrentPosition();
	}
}

bool MapCore::SetZoomToFitRect(RectLatLng rect)
{
	int mmaxZoom = GetMaxZoomToFitRect(rect);
	if(mmaxZoom > 0)
	{
		PointLatLng center = PointLatLng(rect.LatGet() - (rect.HeightLatGet()/ 2), rect.LngGet() + (rect.WidthLngGet() / 2));
		PositionSet(center);

		if(mmaxZoom > maxZoom)
		{
			mmaxZoom = maxZoom;
		}

		if(ZoomGet() != mmaxZoom)
		{
			ZoomSet((int)mmaxZoom);
		}

		return true;
	}
	return false;
}

int MapCore::GetMaxZoomToFitRect(RectLatLng rect)
{
	{
		int zoom = minZoom;

		if(rect.HeightLatGet() == 0 || rect.WidthLngGet() == 0)
		{
			//zoom = maxZoom / 2;
			return ZoomGet();
		}
		else
		{
			for(int i = (int)zoom; i <= maxZoom; i++)
			{
				GPoint p1 = Provider->Projection()->FromLatLngToPixel(rect.LocationTopLeftGet(), i);
				GPoint p2 = Provider->Projection()->FromLatLngToPixel(rect.LocationRightBottomGet(), i);

				if(((p2.X - p1.X) <= Width + 10) && (p2.Y - p1.Y) <= Height + 10)
				{
					zoom = i;
				}
				else
				{
					break;
				}
			}
		}

		return zoom;
	}
}

void MapCore::ReloadMap()
{
	if(IsStarted)
	{
		okZoom = 0;
		skipOverZoom = 0;

		tileLoadQueueLock.Lock();
		tileLoadQueue.clear();
		tileLoadQueueLock.Unlock();

		Matrix.ClearAllLevels();


		FailedLoads.clear();
		RaiseEmptyTileError = true;


		UpdateBounds();
	}
	else
	{
		return;
	}
}

int MapCore::Init(MapControl * value)
{
	pMapCtr = value;
	filename = fopen(".\\log\\log.txt", "w");
	threadpool = new CMyThreadPool(10);
	threadpool->m_WorksOnThread = 4;
	pCacher.Init();
	return 0;
}

void MapCore::ResourceInit()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

void MapCore::ResourceRelease()
{
	curl_global_cleanup();
}

void MapCore::BeginDrag(GPoint pt)
{
	dragPoint.X = pt.X - renderOffset.X;
	dragPoint.Y = pt.Y - renderOffset.Y;
	IsDragging = true;
}

void MapCore::EndDrag()
{
	IsDragging = false;
	mouseDown = GPoint();
}

////重绘图元层
//void MapCore::RefreshOverlay()
//{
//	SDL_Event event;
//	SDL_memset(&event, 0, sizeof(event));
//	event.type = ReDrawMessage;
//	//刷新图层，不刷新底图
//	event.user.code = 0xfe;
//	SDL_PushEvent(&event);
//}

//重绘整个地图
void MapCore::RefreshAll(int value)
{
	SDL_Event event;
	SDL_memset(&event, 0, sizeof(event));
	event.type = ReDrawMessage;
	//重绘
	event.user.code = value;
	event.window.windowID = pMapCtr->Graph.windowID;

	if (SDL_PushEvent(&event) != 1)
	{
		printf("SDL_PushEvent error : %s\n", SDL_GetError());
	}
#ifdef WIN32
	PostMessage((HWND)pMapCtr->m_HParent, WM_PAINT, 0, 1);

	//InvalidateRect( (HWND)pMapCtr->Graph.m_HParent, NULL, false);
	//UpdateWindow((HWND)pMapCtr->Graph.m_HParent);

#endif
	
}