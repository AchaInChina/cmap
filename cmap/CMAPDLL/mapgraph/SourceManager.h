#pragma once
#include "head.h"
#include "SDL.h"

struct cmp_str
{
	bool operator()(char* a, char* b)
	{
		return strcmp(a, b) < 0;
	}
};


class EXPORTS_CLASS SourceManager
{
public:
	SourceManager(void);
	~SourceManager(void);

	map< char *, SDL_Surface*, cmp_str> pathMap;
	map< SDL_Surface*, int> countMap;

	SDL_Surface* GetSurface(char * path, SOURCEFROMAT fromat);
	SDL_Surface* GetSurface(SDL_RWops* source, SOURCEFROMAT fromat);
	SDL_Surface* GetSurface(string source, SOURCEFROMAT fromat);
	SDL_Surface* GetSurface(SOURCEFROMAT fromat, int w, int h);
	SDL_Surface* GetSurface(SDL_Surface* source);
	SDL_Surface* GetSurface(int r, int g, int b , int a, int w, int h);


	void INCQuote(SDL_Surface* value);
	void SUBQuote(SDL_Surface* value);

};

