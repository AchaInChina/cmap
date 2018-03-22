#pragma once
#include "head.h"
#include "SDL.h"


class EXPORTS_CLASS ImageConver
{
public:
	ImageConver();
	ImageConver(SDL_RWops* source, SOURCEFROMAT fromat);
	ImageConver(char * path);
	ImageConver(string source, SOURCEFROMAT fromat);
	ImageConver(SDL_Surface* source);
	ImageConver(SOURCEFROMAT fromat, int w, int h);
	ImageConver(MapColor bc, int w, int h);
	~ImageConver(void);
	SDL_Surface * pSurface;
	int w;
	int h;
	ImageConver Cut(int x, int y, int w, int h);
	ImageConver(const ImageConver& image);
	ImageConver& operator = (const ImageConver& image);

	static void GenerSM();
	static void ReleaseSM();
};

