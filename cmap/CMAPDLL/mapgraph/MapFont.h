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
	char * getLine(char* str); //��һ���ַ�������һ��ʹ�������������ʹ��getMore()//
	char * getMore();//��ʹ����getline(char *) ֮��ʹ�����������ĺ�����ֵ��//
	char * getMaxStr();
	ImageConver getCompleteSurface(char * str, MapColor fc , MapColor bc);
};

