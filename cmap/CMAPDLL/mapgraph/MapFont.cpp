#include "MapFont.h"
#include "ImageConver.h"

MapFont::MapFont(void)
{
	iniValue();
}


MapFont::~MapFont(void)
{
}

void MapFont::iniValue()
{
	font = NULL;
	pstr =NULL;
}

SDL_Surface * MapFont::Printf(char * str)
{
	SDL_Surface * pTmp = NULL;
	return pTmp;
}

int MapFont::getLinesCount(char * str)
{
	int count = 0;
	int length = 0;
	int lastnode = 0;
	while(length < strlen(str))
	{
		if (str[length] == '\n')
		{
			count ++;
			lastnode= length;
		}
		length++;
	}
	if (length != (lastnode + 1))
	{
		count++;
	}
	return count;
}

char * MapFont::getLine(char* value)
{
	memset(buffmaxstr, 0, 512);
	pstr = value;
	int length = 0;
	while(length < strlen(pstr))
	{
		if (pstr[length] == '\n')
		{
			memset(buffstr, 0, 1024);
			memcpy(buffstr, pstr, length);
			pstr += length + 1;
			if (strlen(buffstr) > strlen(buffmaxstr))
			{
				strcpy(buffmaxstr, buffstr);
			}
			return buffstr;
		}
		length++;
	}
	if (length > 0)
	{
		memset(buffstr, 0, 1024);
		memcpy(buffstr, pstr, length);
		pstr += length;
		if (strlen(buffstr) > strlen(buffmaxstr))
		{
			strcpy(buffmaxstr, buffstr);
		}
		return buffstr;
	}
	return NULL;
}

char * MapFont::getMore()
{
	int length = 0;
	int err = strlen(pstr);
	while(length < strlen(pstr))
	{
		if (pstr[length] == '\n')
		{
			memset(buffstr, 0, 1024);
			memcpy(buffstr, pstr, length);
			pstr += length + 1;
			if (strlen(buffstr) > strlen(buffmaxstr))
			{
				strcpy(buffmaxstr, buffstr);
			}
			return buffstr;
		}
		length++;
	}
	if (length > 0)
	{
		memset(buffstr, 0, 1024);
		memcpy(buffstr, pstr, length);
		pstr += length;
		if (strlen(buffstr) > strlen(buffmaxstr))
		{
			strcpy(buffmaxstr, buffstr);
		}
		return buffstr;
	}
	return NULL;
}

char * MapFont::getMaxStr()
{
	return buffmaxstr;
}

ImageConver MapFont::getCompleteSurface(char * str, MapColor fc , MapColor bc)
{
#ifdef WIN32
#define RENDERFONT TTF_RenderUTF8_Blended
#else
#define RENDERFONT TTF_RenderUTF8_Blended
#endif
	SDL_Color fontcolor;
	fontcolor.r = fc.r;
	fontcolor.g = fc.g;
	fontcolor.b = fc.b;
	fontcolor.a = fc.a;
	vector<string> result = SPLIT::split(str, string("\n"));
	int maxindex = 0;
	for (int j = 0; j < result.size(); j ++)
	{
		if (result[j].size() > result[maxindex].size())
			maxindex = j;
	}
	SDL_Surface * tmp = NULL;
	char buff[1024] = {0};
	tmp = RENDERFONT(font, result[maxindex].c_str(), fontcolor);
	if (tmp)
	{
		ImageConver dssurfacetmp(bc, tmp->w + 10, tmp->h * result.size() + 10);
		SDL_Rect rect_piece={ 0, 0 , 0, 0};
		for (int i = 0; i < result.size(); i ++)
		{
			SDL_Surface * signalSurface = NULL;
			if (result[i].size())
			{
				memset(buff, 0, 1024);
				signalSurface = RENDERFONT(font, result[i].c_str(), fontcolor);
			}
			else
			{
				memset(buff, 0, 1024);
				signalSurface = RENDERFONT(font, " ", fontcolor);
			}
			rect_piece.x = 0 + 5;
			rect_piece.y = i*(tmp->h) + 5;
			rect_piece.w = signalSurface->w;
			rect_piece.h = signalSurface->h;
			SDL_BlitSurface(signalSurface, NULL,dssurfacetmp.pSurface, &rect_piece);
			SDL_FreeSurface(signalSurface);
		}
		SDL_FreeSurface(tmp);
		return dssurfacetmp;
	}
	
	ImageConver nullImage;
	return nullImage;
}