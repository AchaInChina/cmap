#pragma once
//#include "windows.h"
#include "head.h"
#include "SDL.h"
#include "GSize.h"
#include "glew.h"
#include "MapFont.h"

#define ReDrawMessage SDL_USEREVENT
#define MySDLMessage ReDrawMessage+1

extern class MapControl;

class EXPORTS_CLASS MapGraph
{
public:
	MapGraph(void);
	~MapGraph(void);
	SDL_Window *window;
	SDL_Surface* ScreenSurface;
	SDL_Surface *surface;
	SDL_Renderer *renderer;
	
	MapFont m_font;
	SDL_Event event;
	int m_HParent;
	MapControl * pMapCtr;
	int width,height;
	GSize DragSize;
	int ReDrawMode;//0:process event and draw asyn,1:process event and draw syn,2:rpocess event asyn,draw syn

	SDL_GLContext openglcontext;
	GLuint tex;
	Uint32 windowID;

	bool manualUpdate;
	static int ReferenceCount;
	static INT64 clickTime;

	static vector<queue<SDL_Event>>* m_MsgQueue;

	void ManualUpdateStart();
	void ManualUpdateEnd();

	int Init(MapControl * value);
	int InitScreen(int value);
	int ClearScreen();
	int UpdateScrean();
	int Quit();
	int SetEventWatch(SDL_EventFilter filter, void* userdata);
	static int AsysGraphEventPro(void *userdata, SDL_Event * event);
	static int AsysGraphEventProNoDraw(void *userdata, SDL_Event * event);
	static int SysGraphDrawEvent(void *userdata, SDL_Event * event);
	int HandleMessage(SDL_Event * event);
	int SynProcessMes();

	void SetLineWith(int value);
	void DrawImage(ImageConver& img, int x, int y, int w, int h);
	void DrawImageScaled(ImageConver& img, int x, int y, int w, int h);
	void DrawLine(int x0, int y0, int x1, int y1, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
	void DrawRect(int x0, int y0, int w, int h, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
	void DrawPath(int x0, int y0, int x1, int y1, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
	void DrawArc(int x, int y, int l, int w, double startangle, double sweepangle, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);

	void CopyToArray(Vertex* DS, GLubyte R, GLubyte G, GLubyte B, GLubyte A, GLfloat X, GLfloat Y, GLfloat Z);

	void OpenGlDrawImage(ImageConver& img, int x, int y, int w, int h);

	void SetStyle(int value);

	void SetViewPort(int widthValue, int heightValue);

	void MakeCurrent();

	void OnMouseWheel(MapControl* pp, SDL_Event * event);
	void OnMouseMove(MapControl* pp, SDL_Event * event);
	void OnMouseDown(MapControl* pp, SDL_Event * event);
	void OnMouseUp(MapControl* pp, SDL_Event * event);

	static void ResourceGener();
	static void ResourceRelease();
};

