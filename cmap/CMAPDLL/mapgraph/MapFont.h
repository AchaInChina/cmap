#pragma once
#include "head.h"
#include "SDL_ttf.h"
#include "SDL.h"

extern class ImageConver;

class EXPORTS_CLASS MapFont
{
public:
	MapFont(void);
	~MapFont(void);
	TTF_Font* font;
	
	char * pstr;
	char buffstr[1024];
	char buffmaxstr[512];

	void iniValue();
	SDL_Surface * Printf(char * str);
	int getLinesCount(char * str);
	char * getLine(char* str); //对一个字符串，第一次使用这个，后续的使用getMore()//
	char * getMore();//当使用了getline(char *) 之后，使用这个方法后的后续的值。//
	char * getMaxStr();
	ImageConver getCompleteSurface(char * str, MapColor fc , MapColor bc);
};

