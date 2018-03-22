#include "MapGraph.h"
#include "MapControl.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "ImageConver.h"
#include "MyGraphEventProTask.h"
#include "MapOverlay.h"
#include "MapMarker.h"
#include "MapRoute.h"
#include "MapPolygon.h"
#include "ImageConver.h"

extern char OGLPiexBuff[];

int MapGraph::ReferenceCount = 0;

INT64 MapGraph::clickTime = 0;

MapGraph::MapGraph(void)
{
	pMapCtr = NULL;
	DragSize.width = 4;
	DragSize.height = 4;
	manualUpdate = false;
	windowID = 0;
	ScreenSurface = NULL;
	tex = 0;
	
}


MapGraph::~MapGraph(void)
{
}


int MapGraph::InitScreen(int value)
{
	m_HParent = value;

	//加载字体//
	m_font.font = TTF_OpenFont("msyh.ttf", 14);
	if ( m_font.font == NULL ) {
		printf("Couldn't load %d pt font from %s: %s\n", 16,"msyh.ttf",SDL_GetError());
	}

	window = SDL_CreateWindowFrom((const void *) m_HParent);
	if (window == NULL)
	{
		printf("couldn't create window:%s\n", SDL_GetError());
		SDL_ClearError();
		return -1;
	}
	windowID = SDL_GetWindowID(window);
	queue<SDL_Event> tmpqueue;
	m_MsgQueue->push_back(tmpqueue);

	SDL_SetWindowTitle(window, "hello word");
	ScreenSurface = SDL_GetWindowSurface(window);
	if (ScreenSurface== NULL)
	{
		printf("couldn't create ScreenSurface:%s\n", SDL_GetError());
		SDL_ClearError();
		return -1;
	}


	SDL_GetWindowSize(window, &width, &height);
	pMapCtr->Core.Width = width;
	pMapCtr->Core.Height = height;

//	SDL_Surface *s;
//	Uint32 rmask, gmask, bmask, amask;
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//	rmask = 0xff000000;
//	gmask = 0x00ff0000;
//	bmask = 0x0000ff00;
//	amask = 0x000000ff;
//#else
//	rmask = 0x000000ff;
//	gmask = 0x0000ff00;
//	bmask = 0x00ff0000;
//	amask = 0xff000000;
//#endif
//	s = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
//	SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, 192, 192, 192, 255));
//	SDL_BlitSurface(s, NULL, ScreenSurface, NULL);
//	SDL_FreeSurface(s);
//
//	SDL_UpdateWindowSurface(window);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//renderer = SDL_CreateSoftwareRenderer(ScreenSurface);
	if (renderer == NULL)
	{
		printf("couldn't create Renderer:%s\n", SDL_GetError());
		SDL_ClearError();
		return -1;
	}
	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);


	//初始化opengl相关部分
	openglcontext = SDL_GL_CreateContext(window);
	if (!openglcontext)
	{
		printf("error SDL_GL_CreateContext: %s\n", SDL_GetError());
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("error glewInit: %s\n", glewGetErrorString(err));
	}

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	int glerr = SDL_GL_SetSwapInterval(1);
	glGenTextures(1, &tex);

	//glEnable(GL_POINT_SMOOTH);  
	//glHint(GL_POINT_SMOOTH, GL_NICEST);  

	//glEnable(GL_LINE_SMOOTH);  
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  

	//glEnable(GL_POLYGON_SMOOTH);  
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); 

	//GLint buffers,samples;
	//glGetIntegerv(GL_SAMPLE_BUFFERS,&buffers);
	//glGetIntegerv(GL_SAMPLES,&samples);
	//glEnable(GL_MULTISAMPLE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if (0)
	{
		glMatrixMode(GL_PROJECTION);  
		glLoadIdentity();  
		glOrtho(-5,5,-5,5,-5,5);  
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
		glViewport(0, 0, pMapCtr->Core.Width, pMapCtr->Core.Height); 
	}
	else
	{
		glMatrixMode(GL_PROJECTION);  
		glLoadIdentity();  
		gluOrtho2D(0, pMapCtr->Core.Width, 0, pMapCtr->Core.Height);  
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity(); 
		glViewport(0, 0, width, height);
	}
	return 0;
}

int MapGraph::UpdateScrean()
{
	if (ReDrawMode == 0)
	{
		SDL_UpdateWindowSurface(window);
	}
	else if(ReDrawMode == 1)
	{
		SDL_RenderPresent(renderer);
	}
	else if (ReDrawMode == 2)
	{
		SDL_RenderPresent(renderer);
	}
	else
	{
		if (SDL_GL_GetCurrentWindow() == window)
		{
			SDL_GL_SwapWindow(window);
		}
	}

	return 0;
}

int MapGraph::Quit()
{
	if (tex != 0)
	{
		glDeleteTextures(1, &tex);
	}
	if (m_font.font)
	{
		TTF_CloseFont(m_font.font);
		
	}
	if (ScreenSurface)
	{
		SDL_FreeSurface(ScreenSurface);
	}
	return 0;
}

int MapGraph::SetEventWatch(SDL_EventFilter filter, void* userdata)
{
	SDL_AddEventWatch(filter, userdata);
	return 0;
}

int MapGraph::ClearScreen()
{
	if (ReDrawMode == 0)
	{
		SDL_Surface *surface;
		surface = SDL_CreateRGBSurface(0, width, height, 32,
			0, 0, 0, 0);
		if(surface == NULL) {
			fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
			return -1;
		}
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 192, 192, 192));
		SDL_BlitSurface( surface, NULL, ScreenSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}
	else if( ReDrawMode == 1)
	{
		SDL_RenderClear(renderer);
	}
	else if( ReDrawMode == 2)
	{
		SDL_RenderClear(renderer);
	}
	else 
	{
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
   
	return 0;
}
int MapGraph::AsysGraphEventProNoDraw(void *userdata, SDL_Event * event)
{
	if (event->type != ReDrawMessage)
	{
		MapGraph::AsysGraphEventPro(userdata, event);
	}
	return 0;
}

int MapGraph::SysGraphDrawEvent(void *userdata, SDL_Event * event)
{
	MapControl* pParent = (MapControl*)userdata;
	switch (event->type) {
	case ReDrawMessage:
		{
			ReDrawSt value;
			value.par1 = event->user.data1;
			value.par2 = event->user.data2;
			value.code = event->user.code;
			pParent->DrawMap(value);
		}
		break;
	}
	return 0;
}

void MapGraph::OnMouseWheel(MapControl* pp, SDL_Event * event)
{
	MapControl* pParent = pp;
	printf("come in SDL_MOUSEWHEEL\n");
	if (!pParent->Core.IsDragging && !pParent->isMouseOverMarker)
	{
		pParent->Core.MouseWheelZooming = true;
		pParent->Core.position = pParent->FromLocalToLatLng(pParent->Core.mouseLastZoom.X, pParent->Core.mouseLastZoom.Y);
		if ( (pParent->ZoomGet() + event->wheel.y) <= pParent->minzoom )
		{
			pParent->ZoomSet(pParent->minzoom);
		}
		else if ( (pParent->ZoomGet() + event->wheel.y) >= pParent->maxzoom )
		{
			pParent->ZoomSet(pParent->maxzoom);
		}
		else
		{
			pParent->ZoomSet(pParent->ZoomGet() + event->wheel.y);
		}
		SDL_WarpMouseInWindow(pParent->Graph.window,pParent->Graph.width/2, pParent->Graph.height/2);
		pParent->Core.MouseWheelZooming = false;
	}
#if 0
	{
		vector<MapOverlay*>::reverse_iterator iterOverlay;
		iterOverlay = pParent->Overlays.Items.rbegin();
		while(iterOverlay != pParent->Overlays.Items.rend())
		{
			if( (*iterOverlay)->IsVisibileGet() )
			{
				//markers
				vector<MapMarker*>::iterator iterMarker;
				iterMarker = (*iterOverlay)->Markers.Items.begin();
				while( iterMarker != (*iterOverlay)->Markers.Items.end() )
				{
					if ((*iterMarker)->IsVisibleGet() && (*iterMarker)->IsHitTestVisible)
					{
						MapMarker* m = *iterMarker;
						GPoint rp = GPoint( pParent->Core.mouseCurrent.X, pParent->Core.mouseCurrent.Y);

						if (m->AreaContains(rp.X, rp.Y))
						{
							if (!m->isMouseOver)
							{
								pParent->SetCursorHandOnEnter();
								m->isMouseOver = true;
								pParent->IsMouseOverMarkerSet(true);

								if (pParent->OnMarkerEnter.DelegateSize() > 0)
								{
									pParent->OnMarkerEnter.DelegateDo(m, 0);
								}

								pParent->Invalidate();
							}
						}
						else if ( m->isMouseOver )
						{
							m->isMouseOver =  false;
							pParent->IsMouseOverMarkerSet(false);
							pParent->RestoreCursorOnLeave();

							if (pParent->OnMarkerLeave.DelegateSize() > 0)
							{
								pParent->OnMarkerLeave.DelegateDo(m, 0);
							}

							pParent->Invalidate();
						}
					}
					iterMarker++;
				}

				//route
				vector<MapRoute*>::iterator iterRoute;
				iterRoute = (*iterOverlay)->Routes.Items.begin();
				while( iterRoute != (*iterOverlay)->Routes.Items.end() )
				{
					if ((*iterRoute)->IsVisibleGet() && (*iterRoute)->IsHitTestVisible)
					{
						MapRoute* m = *iterRoute;
						GPoint rp = GPoint( pParent->Core.mouseCurrent.X, pParent->Core.mouseCurrent.Y);

						if (m->IsInside(rp.X, pParent->Graph.height - rp.Y))
						{
							if (!m->isMouseOver)
							{
								pParent->SetCursorHandOnEnter();
								m->isMouseOver = true;
								pParent->IsMouseOverOverRouteSet(true);

								if (pParent->OnRouteEnter.DelegateSize() > 0)
								{
									pParent->OnRouteEnter.DelegateDo(m, 0);
								}

								pParent->Invalidate();
							}
						}
						else if ( m->isMouseOver )
						{
							m->isMouseOver =  false;
							pParent->IsMouseOverOverRouteSet(false);
							pParent->RestoreCursorOnLeave();

							if (pParent->OnRouteLeave.DelegateSize() > 0)
							{
								pParent->OnRouteLeave.DelegateDo(m, 0);
							}

							pParent->Invalidate();
						}
					}
					iterRoute++;
				}

				//polygon
				vector<MapPolygon*>::iterator iterPolygon;
				iterPolygon = (*iterOverlay)->Polygon.Items.begin();
				while( iterPolygon != (*iterOverlay)->Polygon.Items.end() )
				{
					if ((*iterPolygon)->IsVisibleGet() && (*iterPolygon)->IsHitTestVisible)
					{
						MapPolygon* m = *iterPolygon;
						GPoint rp = GPoint( pParent->Core.mouseCurrent.X, pParent->Core.mouseCurrent.Y);

						if (m->IsInside(rp.X, rp.Y))
						{
							if (!m->isMouseOver)
							{
								pParent->SetCursorHandOnEnter();
								m->isMouseOver = true;
								pParent->IsMouseOverOverPolygonSet(true);

								if (pParent->OnPolygonEnter.DelegateSize() > 0)
								{
									pParent->OnPolygonEnter.DelegateDo(m, 0);
								}

								pParent->Invalidate();
							}
						}
						else if ( m->isMouseOver )
						{
							m->isMouseOver =  false;
							pParent->IsMouseOverOverPolygonSet(false);
							pParent->RestoreCursorOnLeave();

							if (pParent->OnPolygonLeave.DelegateSize() > 0)
							{
								pParent->OnPolygonLeave.DelegateDo(m, 0);
							}

							pParent->Invalidate();
						}
					}
					iterPolygon++;
				}
			}
			iterOverlay++;
		}
	}
#endif

	if (pParent->OnMouseWheel.DelegateSize() > 0)
	{
		MouseEventArg e;
		e.buttonType = MOUSE_MIDDL;
		e.X = 0;
		e.Y = 0;
		e.Step = event->wheel.y	;
		pParent->OnMouseWheel.DelegateDo(e, 0);
	}
	printf("out SDL_MOUSEWHEEL\n");
}

void MapGraph::OnMouseMove(MapControl* pp, SDL_Event * event)
{
	MapControl* pParent = pp;
	pParent->Core.mouseLastZoom.X = event->motion.x;
	pParent->Core.mouseLastZoom.Y = event->motion.y;
	if (!pParent->Core.IsDragging && ! pParent->Core.mouseDown.IsEmpty())
	{
		GPoint p = pParent->ApplyRotationInversion(event->motion.x, event->motion.y);
		if ( abs(p.X - pParent->Core.mouseDown.X) * 2 >= pParent->Graph.DragSize.width || abs(p.Y - pParent->Core.mouseDown.Y) * 2 >= pParent->Graph.DragSize.height)
		{
			pParent->Core.BeginDrag(pParent->Core.mouseDown);
		}
	}

	if (pParent->Core.IsDragging)
	{
		printf("local position is  %d --- %d and time is %d\n",event->motion.x, event->motion.y, event->common.timestamp);
		if (!pParent->IsDraggingGet())
		{
			pParent->isDragging = true;

			pParent->ChangeCursor(SizeAll);

			//if (pParent->cursorBefore)
			//	pParent->DeleteCursor(pParent->cursorBefore);
			//pParent->cursorBefore = pParent->SetCursor(pParent->CreatCursor(SizeAll));
		}
		pParent->Core.mouseCurrent = pParent->ApplyRotationInversion(event->motion.x, event->motion.y);
		//更新地图
		pParent->Core.Drag(pParent->Core.mouseCurrent);
		//更新图层
		pParent->ForceUpdateOverlays();

	}
	else
	{
		if (pParent->isSelected && !pParent->selectionStart.IsEmpty())
		{
			pParent->selectionEnd = pParent->FromLocalToLatLng(event->motion.x, event->motion.y);
			PointLatLng p1 = pParent->selectionStart;
			PointLatLng p2 = pParent->selectionEnd;
			double x1 = Min(p1.LngGet(), p2.LngGet());
			double y1 = Max(p1.LatGet(), p2.LatGet());
			double x2 = Max(p1.LngGet(), p2.LngGet());
			double y2 = Min(p1.LatGet(), p2.LatGet());

			pParent->selectedArea = RectLatLng(y1, x1, x2 - x1, y1 - y2);
		}
		else if (pParent->Core.mouseDown.IsEmpty())
		{
			vector<MapOverlay*>::reverse_iterator iterOverlay;
			iterOverlay = pParent->Overlays.Items.rbegin();
			while(iterOverlay != pParent->Overlays.Items.rend())
			{
				if( (*iterOverlay)->IsVisibileGet() )
				{
					//markers
					vector<MapMarker*>::iterator iterMarker;
					iterMarker = (*iterOverlay)->Markers.Items.begin();
					while( iterMarker != (*iterOverlay)->Markers.Items.end() )
					{
						if ((*iterMarker)->IsVisibleGet() && (*iterMarker)->IsHitTestVisible)
						{
							MapMarker* m = *iterMarker;
							GPoint rp = GPoint( event->motion.x, event->motion.y);

							if (m->AreaContains(rp.X, rp.Y))
							{
								if (!m->isMouseOver)
								{
									pParent->SetCursorHandOnEnter();
									m->isMouseOver = true;
									pParent->IsMouseOverMarkerSet(true);

									if (pParent->OnMarkerEnter.DelegateSize() > 0)
									{
										pParent->OnMarkerEnter.DelegateDo(m, 0);
									}

									pParent->Invalidate();
								}
							}
							else if ( m->isMouseOver )
							{
								m->isMouseOver =  false;
								pParent->IsMouseOverMarkerSet(false);
								pParent->RestoreCursorOnLeave();

								if (pParent->OnMarkerLeave.DelegateSize() > 0)
								{
									pParent->OnMarkerLeave.DelegateDo(m, 0);
								}

								pParent->Invalidate();
							}
						}
						iterMarker++;
					}

					//route
					vector<MapRoute*>::iterator iterRoute;
					iterRoute = (*iterOverlay)->Routes.Items.begin();
					while( iterRoute != (*iterOverlay)->Routes.Items.end() )
					{
						if ((*iterRoute)->IsVisibleGet() && (*iterRoute)->IsHitTestVisible)
						{
							MapRoute* m = *iterRoute;
							GPoint rp = GPoint( event->motion.x, event->motion.y);

							if (m->IsInside(rp.X, pParent->Graph.height - rp.Y))
							{
								if (!m->isMouseOver)
								{
									pParent->SetCursorHandOnEnter();
									m->isMouseOver = true;
									pParent->IsMouseOverOverRouteSet(true);

									if (pParent->OnRouteEnter.DelegateSize() > 0)
									{
										pParent->OnRouteEnter.DelegateDo(m, 0);
									}

									pParent->Invalidate();
								}
							}
							else if ( m->isMouseOver )
							{
								m->isMouseOver =  false;
								pParent->IsMouseOverOverRouteSet(false);
								pParent->RestoreCursorOnLeave();

								if (pParent->OnRouteLeave.DelegateSize() > 0)
								{
									pParent->OnRouteLeave.DelegateDo(m, 0);
								}

								pParent->Invalidate();
							}
						}
						iterRoute++;
					}

					//polygon
					vector<MapPolygon*>::iterator iterPolygon;
					iterPolygon = (*iterOverlay)->Polygon.Items.begin();
					while( iterPolygon != (*iterOverlay)->Polygon.Items.end() )
					{
						if ((*iterPolygon)->IsVisibleGet() && (*iterPolygon)->IsHitTestVisible)
						{
							MapPolygon* m = *iterPolygon;
							GPoint rp = GPoint( event->motion.x, event->motion.y);

							if (m->IsInside(rp.X, rp.Y))
							{
								if (!m->isMouseOver)
								{
									pParent->SetCursorHandOnEnter();
									m->isMouseOver = true;
									pParent->IsMouseOverOverPolygonSet(true);

									if (pParent->OnPolygonEnter.DelegateSize() > 0)
									{
										pParent->OnPolygonEnter.DelegateDo(m, 0);
									}

									pParent->Invalidate();
								}
							}
							else if ( m->isMouseOver )
							{
								m->isMouseOver =  false;
								pParent->IsMouseOverOverPolygonSet(false);
								pParent->RestoreCursorOnLeave();

								if (pParent->OnPolygonLeave.DelegateSize() > 0)
								{
									pParent->OnPolygonLeave.DelegateDo(m, 0);
								}

								pParent->Invalidate();
							}
						}
						iterPolygon++;
					}
				}
				iterOverlay++;
			}
		}
	}
	if (pParent->OnMouseMove.DelegateSize() > 0)
	{
		MouseEventArg e;
		e.buttonType = MOUSE_NULL;
		e.X = event->button.x;
		e.Y	= event->button.y;
		pParent->OnMouseMove.DelegateDo(e, 0);
	}
}

void MapGraph::OnMouseDown(MapControl* pp, SDL_Event * event)
{
	MapControl* pParent = pp;
	printf("SDL_MOUSEBUTTONDOWN\n");

	if (pParent->IsMouseOverMarkerGet())
		return;
	if (pParent->dragButton == MOUSE_LEFT)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			pParent->Core.mouseDown = pParent->ApplyRotationInversion(event->button.x, event->button.y);
		}
		else if (!pParent->isSelected)
		{
			pParent->isSelected = true;
			pParent->SelectedAreaSet(RectLatLng());
			pParent->selectionEnd = PointLatLng();
			pParent->selectionStart = pParent->FromLocalToLatLng(event->motion.x, event->motion.y);
		}
	} 
	else if (pParent->dragButton == MOUSE_RIGHT)
	{
		if (event->button.button == SDL_BUTTON_RIGHT)
		{
			pParent->Core.mouseDown = pParent->ApplyRotationInversion(event->button.x, event->button.y);
		}
		else if (!pParent->isSelected)
		{
			pParent->isSelected = true;
			pParent->SelectedAreaSet(RectLatLng());
			pParent->selectionEnd = PointLatLng();
			pParent->selectionStart = pParent->FromLocalToLatLng(event->motion.x, event->motion.y);
		}
	}
	if (pParent->OnMouseDown.DelegateSize() > 0)
	{
		MouseEventArg e;
		e.buttonType = MOUSE_RIGHT;
		e.X = event->button.x;
		e.Y	= event->button.y;
		pParent->OnMouseDown.DelegateDo(e, 0);
	}
}

void MapGraph::OnMouseUp(MapControl* pp, SDL_Event * event)
{
	MapControl* pParent = pp;
	printf("SDL_MOUSEBUTTONUP\n");
	if (pParent->isSelected)
	{
		pParent->isSelected = false;
	}

	int flag = 0;

	if (pParent->Core.IsDragging)
	{
		if (pParent->IsDraggingGet())
		{
			pParent->isDragging = false;

			pParent->RollBackCursor();

			//if (pParent->cursorBefore)
			//{
			//	pParent->cursorBefore = pParent->SetCursor(pParent->cursorBefore);
			//	pParent->DeleteCursor(pParent->cursorBefore);
			//	pParent->cursorBefore = NULL;
			//}
		}
		pParent->Core.EndDrag();
		flag = 1;
	}
	else
	{
		if (pParent->dragButton == MOUSE_LEFT)
		{
			if (event->button.button == SDL_BUTTON_LEFT)
			{
				pParent->Core.mouseDown = GPoint();
			}
			if ( (!pParent->selectionEnd.IsEmpty()) && (!pParent->selectionStart.IsEmpty()) && event->button.button == SDL_BUTTON_RIGHT)
			{
				bool zoomtofit = false;

				if (!pParent->selectedArea.IsEmpty())
				{
					pParent->Core.position = pParent->selectedArea.LocationMiddle();
					zoomtofit = pParent->SetZoomToFitRect(pParent->selectedArea);
				}

				if (pParent->OnSelectionChange.DelegateSize() > 0)
				{
					pParent->OnSelectionChange.DelegateDo(pParent->selectedArea, zoomtofit);
				}
				pParent->selectionEnd = PointLatLng();
				pParent->selectionStart = PointLatLng();
				pParent->selectedArea = RectLatLng();
				flag = 1;
			}
			else
			{
				pParent->Invalidate();
			}
		} 
		else if (pParent->dragButton == MOUSE_RIGHT)
		{
			if (event->button.button == SDL_BUTTON_RIGHT )
			{
				pParent->Core.mouseDown = GPoint();
			}
			if ( (!pParent->selectionEnd.IsEmpty()) && (!pParent->selectionStart.IsEmpty()) && event->button.button == SDL_BUTTON_LEFT )
			{
				bool zoomtofit = false;

				if (!pParent->selectedArea.IsEmpty())
				{
					pParent->Core.position = pParent->selectedArea.LocationMiddle();
					zoomtofit = pParent->SetZoomToFitRect(pParent->selectedArea);
				}
				pParent->selectionEnd = PointLatLng();
				pParent->selectionStart = PointLatLng();
				pParent->selectedArea = RectLatLng();
				flag = 1;
			}
			else
			{
				pParent->Invalidate();
			}
		}
	}

	MouseEventArg e;
	if (event->button.button == SDL_BUTTON_LEFT)
	{
		e.buttonType = MOUSE_LEFT;
		e.X = event->button.x;
		e.Y	= event->button.y;
	}
	else
	{
		e.buttonType = MOUSE_RIGHT;
		e.X = event->button.x;
		e.Y	= event->button.y;
	}
	if (flag == 0)
	{
		vector<MapOverlay*>::reverse_iterator iterOverlay;
		iterOverlay = pParent->Overlays.Items.rbegin();
		while(iterOverlay != pParent->Overlays.Items.rend())
		{
			if( (*iterOverlay)->IsVisibileGet() )
			{
				//markers
				vector<MapMarker*>::iterator iterMarker;
				iterMarker = (*iterOverlay)->Markers.Items.begin();
				while( iterMarker != (*iterOverlay)->Markers.Items.end() )
				{
					if ((*iterMarker)->IsVisibleGet() && (*iterMarker)->IsHitTestVisible)
					{
						MapMarker* m = *iterMarker;
						GPoint rp = GPoint( event->motion.x, event->motion.y);

						if (m->AreaContains(rp.X, rp.Y))
						{
							if (pParent->OnMarkerClick.DelegateSize() > 0)
							{
								pParent->OnMarkerClick.DelegateDo(m, e);
							}
							break;
						}
					}
					iterMarker++;
				}

				//route
				vector<MapRoute*>::iterator iterRoute;
				iterRoute = (*iterOverlay)->Routes.Items.begin();
				while( iterRoute != (*iterOverlay)->Routes.Items.end() )
				{
					if ((*iterRoute)->IsVisibleGet() && (*iterRoute)->IsHitTestVisible)
					{
						MapRoute* m = *iterRoute;
						GPoint rp = GPoint( event->motion.x, event->motion.y);

						if (m->IsInside(rp.X, pParent->Graph.height - rp.Y))
						{
							if (pParent->OnRouteClick.DelegateSize() > 0)
							{
								pParent->OnRouteClick.DelegateDo(m, e);
							}
							break;
						}
					}
					iterRoute++;
				}

				//polygon
				vector<MapPolygon*>::iterator iterPolygon;
				iterPolygon = (*iterOverlay)->Polygon.Items.begin();
				while( iterPolygon != (*iterOverlay)->Polygon.Items.end() )
				{
					if ((*iterPolygon)->IsVisibleGet() && (*iterPolygon)->IsHitTestVisible)
					{
						MapPolygon* m = *iterPolygon;
						GPoint rp = GPoint( event->motion.x, event->motion.y);

						if (m->IsInside(rp.X, rp.Y))
						{
							if (pParent->OnPolygonClick.DelegateSize() > 0)
							{
								pParent->OnPolygonClick.DelegateDo(m, e);
							}
							break;
						}
					}
					iterPolygon++;
				}
			}
			iterOverlay++;
		}
	}

	if (pParent->OnMouseUp.DelegateSize() > 0)
	{
		pParent->OnMouseUp.DelegateDo(e, 0);
	}
	INT64 curClickTime = GetTickCount();
	if ( (curClickTime - pParent->Graph.clickTime) > 300)
	{
		if (pParent->OnMouseClick.DelegateSize() > 0)
		{
			pParent->OnMouseClick.DelegateDo(e, 0);
		}
	}
	else
	{
		if (pParent->OnMouseDClick.DelegateSize() > 0)
		{
			pParent->OnMouseDClick.DelegateDo(e, 0);
		}
	}
	pParent->Graph.clickTime = curClickTime;
}

int MapGraph::AsysGraphEventPro(void *userdata, SDL_Event * event)
{
	MapControl* pParent = (MapControl*)userdata;
	if (event->window.windowID != pParent->Graph.windowID)
		return 0;
	switch (event->type) {
	case ReDrawMessage:
		{
			ReDrawSt value;
			value.par1 = event->user.data1;
			value.par2 = event->user.data2;
			value.code = event->user.code;
			pParent->DrawMap(value);
		}
		break;
	case SDL_MOUSEWHEEL:
		{
			pParent->Graph.OnMouseWheel( pParent, event);
		}
		break;
	case SDL_MOUSEMOTION:
		{
			pParent->Graph.OnMouseMove(pParent, event);
		}
		break;
	case SDL_QUIT:
		break;
	case SDL_MOUSEBUTTONDOWN:
		{
			pParent->Graph.OnMouseDown(pParent, event);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		{
			pParent->Graph.OnMouseUp(pParent, event);
		}
		break;
	default:

		break;
	}

 	if (event->type == SDL_WINDOWEVENT) {
 		switch (event->window.event) {
 		case SDL_WINDOWEVENT_SHOWN:
 			break;
 		case SDL_WINDOWEVENT_HIDDEN:
 			break;
 		case SDL_WINDOWEVENT_EXPOSED:
			//printf("SDL_WINDOWEVENT_EXPOSED\n");
			//pParent->Refresh(0);
 			break;
 		case SDL_WINDOWEVENT_MOVED:
 			break;
 		case SDL_WINDOWEVENT_RESIZED:
			pParent->Graph.MakeCurrent();
 			pParent->Graph.SetViewPort(event->window.data1, event->window.data2);
 			pParent->Core.OnMapSizeChanged(event->window.data1, event->window.data2);
			if (pParent->OnMapSizeChange.DelegateSize() > 0)
			{
				pParent->OnMapSizeChange.DelegateDo(event->window.data1, event->window.data2);
			}
 			pParent->ForceUpdateOverlays();
 			break;
 		case SDL_WINDOWEVENT_SIZE_CHANGED:
 			break;
 		case SDL_WINDOWEVENT_MINIMIZED:
			printf("SDL_WINDOWEVENT_MINIMIZED\n");
 			break;
 		case SDL_WINDOWEVENT_MAXIMIZED:
			printf("SDL_WINDOWEVENT_MAXIMIZED\n");
 			break;
 		case SDL_WINDOWEVENT_RESTORED:
			printf("SDL_WINDOWEVENT_RESTORED\n");
 			break;
 		case SDL_WINDOWEVENT_ENTER:
 			pParent->OnMouseEnter.DelegateDo( 0, 0);
 			break;
 		case SDL_WINDOWEVENT_LEAVE:
 			pParent->OnMouseLeave.DelegateDo( 0, 0);
 			break;
 		case SDL_WINDOWEVENT_FOCUS_GAINED:
 			pParent->OnMouseFocusGain.DelegateDo( 0, 0);
 			break;
 		case SDL_WINDOWEVENT_FOCUS_LOST:
 			pParent->OnMouseFocusLost.DelegateDo( 0, 0);
 			break;
 		case SDL_WINDOWEVENT_CLOSE:
 			break;
 		default:
			printf("UNDEFINE MESSAGE\n");
 			break;
 		}
 	}
	return 0;
}

vector<queue<SDL_Event>>* MapGraph::m_MsgQueue = NULL;

int MapGraph::SynProcessMes()
{
	int Drawflag = 0;
	if (ReDrawMode == 1)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			MapGraph::AsysGraphEventPro(pMapCtr, &event);
			Drawflag = 1;
			//if (event.type == ReDrawMessage)
			//{
			//	//HandleMessage(&event);
			//	MapGraph::AsysGraphEventPro(pMapCtr, &event);
			//	Drawflag = 1;
			//}
			//else
			//{
			//	SDL_Event * tmp = new SDL_Event();
			//	memcpy(tmp, &event, sizeof(SDL_Event));
			//	MyGraphEventProTask * task = new MyGraphEventProTask(0);
			//	task->userdata = pMapCtr;
			//	task->pEvent = tmp;
			//	pMapCtr->Core.threadpool->addTask( task, NORMAL);
			//}
		}
	}
	else if (ReDrawMode == 2)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if (event.type == ReDrawMessage)
			{
				MapGraph::SysGraphDrawEvent(pMapCtr, &event);
				Drawflag = 1;
			}
		}
	}
	else if (ReDrawMode == 3)
	{
#if 0
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if (event.type == ReDrawMessage)
			{
				if (event.window.windowID == windowID)
				{
					MapGraph::SysGraphDrawEvent(pMapCtr, &event);
					Drawflag = 1;
				}
			}
		}

#else
		vector<SDL_Event> tmpEventQueue;
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if (event.type == ReDrawMessage)
			{
				if (event.window.windowID == windowID)
				{
					MapGraph::SysGraphDrawEvent(pMapCtr, &event);
					Drawflag = 1;
				}
				else
				{
					tmpEventQueue.push_back(event);
				}
			}
		}

		//vector<SDL_Event>::iterator iter = tmpEventQueue.begin();
		//while(iter != tmpEventQueue.end())
		//{
		//	SDL_Event eventEvent = *iter;
		//	SDL_PushEvent( &eventEvent );
		//	iter++;
		//}

#endif

	}
	
	return Drawflag;
}

int MapGraph::HandleMessage(SDL_Event * event)
{
	//MapControl* pParent = this->pMapCtr;
	//switch (event->type) {
	//case ReDrawMessage:
	//	{
	//		LoadTask value;
	//		value.Pos.X = (int)event->user.data1;
	//		value.Pos.Y	= (int)event->user.data2;
	//		value.Zoom = event->user.code;
	//		pParent->DrawMap(value);
	//	}
	//	break;
	//case SDL_MOUSEWHEEL:
	//	{
	//		if (!pParent->Core.IsDragging)
	//		{
	//			pParent->Core.MouseWheelZooming = true;
	//			//pParent->PositionSet(pParent->FromLocalToLatLng(pParent->Core.mouseLastZoom.X, pParent->Core.mouseLastZoom.Y));
	//			pParent->Core.position = pParent->FromLocalToLatLng(pParent->Core.mouseLastZoom.X, pParent->Core.mouseLastZoom.Y);
	//			printf("the position is  %f --- %f\n",pParent->Core.position.lat, pParent->Core.position.lng);
	//			if ( (pParent->ZoomGet() + event->wheel.y) <= pParent->minzoom )
	//			{
	//				pParent->ZoomSet(pParent->minzoom);
	//			}
	//			else if ( (pParent->ZoomGet() + event->wheel.y) >= pParent->maxzoom )
	//			{
	//				pParent->ZoomSet(pParent->maxzoom);
	//			}
	//			else
	//			{
	//				pParent->ZoomSet(pParent->ZoomGet() + event->wheel.y);
	//			}
	//			SDL_WarpMouseInWindow(pParent->Graph.window,pParent->Graph.width/2, pParent->Graph.height/2);
	//			pParent->Core.MouseWheelZooming = false;
	//		}
	//		break;
	//		
	//	}
	//case SDL_MOUSEMOTION:
	//	{
	//		pParent->Core.mouseLastZoom.X = event->motion.x;
	//		pParent->Core.mouseLastZoom.Y = event->motion.y;
	//		
	//		if (!pParent->Core.IsDragging && ! pParent->Core.mouseDown.IsEmpty())
	//		{
	//			GPoint p = pParent->ApplyRotationInversion(event->motion.x, event->motion.y);
	//			if ( abs(p.X - pParent->Core.mouseDown.X) * 2 >= pParent->Graph.DragSize.width || abs(p.Y - pParent->Core.mouseDown.Y) * 2 >= pParent->Graph.DragSize.height)
	//			{
	//				pParent->Core.BeginDrag(pParent->Core.mouseDown);
	//			}
	//		}

	//		if (pParent->Core.IsDragging)
	//		{
	//			printf("local position is  %d --- %d\n",event->motion.x, event->motion.y);
	//			if (!pParent->IsDraggingGet())
	//			{
	//				pParent->isDragging = true;
	//			}
	//			pParent->Core.mouseCurrent = pParent->ApplyRotationInversion(event->motion.x, event->motion.y);
	//			//更新地图
	//			pParent->Core.Drag(pParent->Core.mouseCurrent);
	//			pParent->Core.RefreshAll(0);

	//			//更新图层
	//			pParent->ForceUpdateOverlays();

	//		}
	//		break;
	//	}
	//case SDL_QUIT:
	//	break;
	//case SDL_MOUSEBUTTONDOWN:
	//	{
	//		if (event->button.button == SDL_BUTTON_LEFT)
	//		{
	//			pParent->Core.mouseDown = pParent->ApplyRotationInversion(event->button.x, event->button.y);
	//		}
	//	}
	//	break;
	//case SDL_MOUSEBUTTONUP:
	//	{
	//		if (event->button.button == SDL_BUTTON_LEFT)
	//		{
	//			if (pParent->Core.IsDragging)
	//			{
	//				if (pParent->IsDraggingGet())
	//				{
	//					pParent->isDragging = false;
	//				}
	//				pParent->Core.EndDrag();
	//			}
	//		}
	//	}
	//	break;
	//}
	return 0;
}

int MapGraph::Init(MapControl * value)
{
	pMapCtr = value;
	InitScreen(pMapCtr->m_HParent);
	return 0;
}

void MapGraph::DrawImageScaled(ImageConver& img, int x, int y, int w, int h)
{
	SDL_Rect rect_piece={ x, y , w, h};
	if (ReDrawMode == 0)
	{
		if (0)
		{
			if ( y>= 0)
			{
				if (SDL_BlitScaled(img.pSurface, NULL, ScreenSurface, &rect_piece))
				{
					printf("SDL_BlitScaled error: %s\n", SDL_GetError());
					SDL_ClearError();
				}
			}
			else
			{
				ImageConver tmp( GRAY, 256, 256);
				SDL_BlitSurface(tmp.pSurface, NULL, ScreenSurface, &rect_piece);
			}
		} 
		else
		{
			if (SDL_BlitScaled(img.pSurface, NULL, ScreenSurface, &rect_piece))
			{
				printf("SDL_BlitScaled error: %s\n", SDL_GetError());
				SDL_ClearError();
			}
		}
	}
	else if(ReDrawMode == 1)
	{
		SDL_Texture * bitmapTex = NULL;
		bitmapTex = SDL_CreateTextureFromSurface(renderer, img.pSurface);
		SDL_RenderCopy(renderer, bitmapTex, NULL, &rect_piece);
		SDL_DestroyTexture(bitmapTex);
	}
	else if(ReDrawMode == 2)
	{
		SDL_Texture * bitmapTex = NULL;
		bitmapTex = SDL_CreateTextureFromSurface(renderer, img.pSurface);
		SDL_RenderCopy(renderer, bitmapTex, NULL, &rect_piece);
		SDL_DestroyTexture(bitmapTex);
	}
	else 
	{
		OpenGlDrawImage(img, x, y, w, h);
	}

	if (!manualUpdate)
	{
		UpdateScrean();
	}
}

void MapGraph::OpenGlDrawImage(ImageConver& img, int x, int y, int w, int h)
{
	SDL_Surface * tmpSurface = NULL;
	tmpSurface = img.pSurface;

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tmpSurface->w, tmpSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmpSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);



	VertexST quads[]={
		{ 0.0, 1.0, 0.0, 0.0,  0.0},
		{ 1.0, 1.0, 0.0, 0.0,  0.0},
		{ 1.0, 0.0, 0.0, 0.0,  0.0},
		{ 0.0, 0.0, 0.0, 0.0,  0.0},
	};

	//int X = 0;
	//int Y = 0;
	//GLint viewport[4]; 
	//GLdouble modelview[16]; 
	//GLdouble projection[16]; 
	//GLfloat winX, winY, winZ; 
	//GLdouble posX, posY, posZ; 

	//glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
	//glGetDoublev(GL_PROJECTION_MATRIX, projection); 
	//glGetIntegerv(GL_VIEWPORT, viewport); 

	//X = x;
	//Y = y + h;
	//winX = (float)X;
	//winY = (float)viewport[3] - (float)Y;
	//glReadPixels(X, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
	//gluUnProject(winX, winY, 5, modelview, projection, viewport, &posX, &posY, &posZ); 
	//quads[0].x = posX;
	//quads[0].y = posY;
	//quads[0].z = 5;

	////quads[0].x = X;
	////quads[0].y = Y;
	////quads[0].z = 0;

	//X = x + w;
	//Y = y + h;
	//winX = (float)X;
	//winY = (float)viewport[3] - (float)Y;
	//glReadPixels(X, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
	//gluUnProject(winX, winY, 5, modelview, projection, viewport, &posX, &posY, &posZ); 
	//quads[1].x = posX;
	//quads[1].y = posY;
	//quads[1].z = 5;

	////quads[1].x = X;
	////quads[1].y = Y;
	////quads[1].z = 0;

	//X = x + w;
	//Y = y;
	//winX = (float)X;
	//winY = (float)viewport[3] - (float)Y;
	//glReadPixels(X, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
	//gluUnProject(winX, winY, 5, modelview, projection, viewport, &posX, &posY, &posZ); 
	//quads[2].x = posX;
	//quads[2].y = posY;
	//quads[2].z = 5;

	////quads[2].x = X;
	////quads[2].y = Y;
	////quads[2].z = 0;

	//X = x;
	//Y = y;
	//winX = (float)X;
	//winY = (float)viewport[3] - (float)Y;
	//glReadPixels(X, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
	//gluUnProject(winX, winY, 5, modelview, projection, viewport, &posX, &posY, &posZ); 
	//quads[3].x = posX;
	//quads[3].y = posY;
	//quads[3].z = 5;

	////quads[3].x = X;
	////quads[3].y = Y;
	////quads[3].z = 0;

	quads[0].x = x;
	quads[0].y = height - (y + h);
	quads[0].z = 1;

	quads[1].x = x + w;
	quads[1].y = height - (y + h);
	quads[1].z = 1;

	quads[2].x = x + w;
	quads[2].y = height - y;
	quads[2].z = 1;

	quads[3].x = x;
	quads[3].y = height - y;
	quads[3].z = 1;

	glInterleavedArrays( GL_T2F_V3F, 0, quads);
	glDrawArrays( GL_QUADS, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MapGraph::DrawImage(ImageConver& img, int x, int y, int w, int h)
{
	if ( img.pSurface == NULL )
		return;
	UINT64 time1 = GetTickCount();
	SDL_Rect rect_piece={ x, y , w, h};
	if (ReDrawMode == 0)
	{
		if (SDL_BlitSurface(img.pSurface, NULL, ScreenSurface, &rect_piece))
		{
			printf("SDL_BlitScaled error: %s\n", SDL_GetError());
			SDL_ClearError();
		}
	}
	else if (ReDrawMode == 1)
	{
		SDL_Texture * bitmapTex = NULL;
		bitmapTex = SDL_CreateTextureFromSurface(renderer, img.pSurface);
		SDL_RenderCopy(renderer, bitmapTex, NULL, &rect_piece);
		SDL_DestroyTexture(bitmapTex);
	}
	else if (ReDrawMode == 2)
	{
		SDL_Texture * bitmapTex = NULL;
		bitmapTex = SDL_CreateTextureFromSurface(renderer, img.pSurface);
		SDL_RenderCopy(renderer, bitmapTex, NULL, &rect_piece);
		SDL_DestroyTexture(bitmapTex);
	}
	else
	{
		OpenGlDrawImage(img, x, y, w, h);
	}

	if (!manualUpdate)
	{
		UpdateScrean();
	}
	UINT64 time2 = GetTickCount();
//	printf("draw one pic use time is %d\n",time2 - time1);
}

void MapGraph::ManualUpdateStart()
{
	manualUpdate = true;
}
void MapGraph::ManualUpdateEnd()
{
	manualUpdate = false;
	UpdateScrean();
}

void MapGraph::SetLineWith(int value)
{
	glLineWidth(value);
}

void MapGraph::DrawLine(int x0, int y0, int x1, int y1, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Vertex line[]={
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
	};
	CopyToArray(&line[0], r, g, b, a, x0, height - (y0), 1);
	CopyToArray(&line[1], r, g, b, a, x1, height - (y1), 1);
	glInterleavedArrays( GL_C4UB_V3F, 0, line);
	glLineWidth(width);
	glDrawArrays( GL_LINES, 0, 2);

	if (!manualUpdate)
	{
		UpdateScrean();
	}
}

void MapGraph::DrawRect(int x0, int y0, int w, int h, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Vertex rect[]={
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
	};
	CopyToArray(&rect[0], r, g, b, a, x0, height - (y0), 1);
	CopyToArray(&rect[1], r, g, b, a, x0 + w, height - (y0), 1);
	CopyToArray(&rect[2], r, g, b, a, x0 + w, height - (y0 + h), 1);
	CopyToArray(&rect[3], r, g, b, a, x0, height - (y0 + h), 1);
	glInterleavedArrays( GL_C4UB_V3F, 0, rect);
	glLineWidth(width);
	glDrawArrays( GL_LINE_LOOP, 0, 4);

	if (!manualUpdate)
	{
		UpdateScrean();
	}
}

void MapGraph::DrawArc(int x, int y, int l, int w, double startangle, double sweepangle, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glPushMatrix();
	glTranslatef( x, height - y,1);
	glColor4ub(	r, g, b, a);
	gluPartialDisk(qobj, l - w / 2, l + w / 2, 20, 20, startangle, sweepangle);
	glColor4ub(255, 255, 255, 255);
	glPopMatrix();
	gluDeleteQuadric(qobj);

	if (!manualUpdate)
	{
		UpdateScrean();
	}
}

void MapGraph::CopyToArray(Vertex* DS, GLubyte R, GLubyte G, GLubyte B, GLubyte A, GLfloat X, GLfloat Y, GLfloat Z)
{
	DS->r = R;
	DS->g = G;
	DS->b = B;
	DS->a = A;
	DS->x = X;
	DS->y = Y;
	DS->z = Z;
}

void MapGraph::DrawPath(int x0, int y0, int x1, int y1, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (width > 100)
		return;

	Vertex line[]={
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
		{ 0, 0, 0, 0, 0.0, 0.0, 0.0},
	};
	int widthincrement = width / 2;
	double K = 0.0;
	int Xt = 0;
	int Yt = 0;
	double l = widthincrement;

	if (x1 == x0)
	{
		K = 1;
		Xt = widthincrement;
		Yt = 0;
	}
	else
	{
		K = (y1 - y0) / (x1 - x0);
		Xt = abs(K * sqrt( (l*l) / (K*K + 1) ));
		Yt = sqrt( (l*l) / (K*K + 1) );
	}

	if (widthincrement <= 1)
	{
		Xt += 1;
		Yt += 1;
	}

	if (K >= 0)
	{
		CopyToArray(&line[0], r, g, b, a, x0 + Xt, height - (y0 - Yt), 1);
		CopyToArray(&line[1], r, g, b, a, x0 - Xt, height - (y0 + Yt), 1);
		CopyToArray(&line[2], r, g, b, a, x1 - Xt, height - (y1 + Yt), 1);
		CopyToArray(&line[3], r, g, b, a, x1 + Xt, height - (y1 - Yt), 1);
	}
	else
	{
		CopyToArray(&line[0], r, g, b, a, x0 - Xt, height - (y0 - Yt), 1);
		CopyToArray(&line[1], r, g, b, a, x0 + Xt, height - (y0 + Yt), 1);
		CopyToArray(&line[2], r, g, b, a, x1 + Xt, height - (y1 + Yt), 1);
		CopyToArray(&line[3], r, g, b, a, x1 - Xt, height - (y1 - Yt), 1);
	}
	glInterleavedArrays( GL_C3F_V3F, 0, line);
	glDrawArrays( GL_QUADS, 0, 4);
	
	if (!manualUpdate)
	{
		UpdateScrean();
	}
}

void MapGraph::SetStyle(int value)
{
	TTF_SetFontStyle(m_font.font, value);
}

void MapGraph::SetViewPort(int widthValue, int heightValue)
{
	//ScreenSurface = SDL_GetWindowSurface(window);

	width = widthValue;
	height = heightValue;
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	gluOrtho2D(0, width, 0, height);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	glViewport(0, 0, width, height);

//	SDL_Surface *s;
//	Uint32 rmask, gmask, bmask, amask;
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//	rmask = 0xff000000;
//	gmask = 0x00ff0000;
//	bmask = 0x0000ff00;
//	amask = 0x000000ff;
//#else
//	rmask = 0x000000ff;
//	gmask = 0x0000ff00;
//	bmask = 0x00ff0000;
//	amask = 0xff000000;
//#endif
//	s = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
//	SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, 192, 192, 192, 255));
//	SDL_BlitSurface(s, NULL, ScreenSurface, NULL);
//	SDL_FreeSurface(s);

	//SDL_UpdateWindowSurface(window);
}

void MapGraph::MakeCurrent()
{
	if (SDL_GL_GetCurrentWindow() != window)
	{
		if (SDL_GL_MakeCurrent(window, openglcontext) != 0)
			printf("SDL_GL_MakeCurrent failed\n");
	}
}

void MapGraph::ResourceGener()
{
	m_MsgQueue = new vector<queue<SDL_Event>>();

	//设置版本
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		SDL_ClearError();
	}

	int flags=IMG_INIT_JPG|IMG_INIT_PNG;
	int initted=IMG_Init(flags);
	if((initted&flags) != flags) {
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}

	if ( TTF_Init() < 0 ) {
		printf("Couldn't initialize TTF: %s\n",SDL_GetError());
	}

	if ( SDL_RegisterEvents(10) == -1)
	{
		printf("couldn't create ReDrawMessage:%s\n", SDL_GetError());
		SDL_ClearError();
	}
}
void MapGraph::ResourceRelease()
{
	delete m_MsgQueue;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}