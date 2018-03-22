#include "SourceManager.h"
#include "ximage.h"
#include "SDL_image.h"

SourceManager::SourceManager(void)
{
}

SourceManager::~SourceManager(void)
{
}

SDL_Surface* SourceManager::GetSurface(char * path, SOURCEFROMAT fromat)
{
	SDL_Surface* pSurface = NULL;
	SDL_Surface* tmpSurface = NULL;

	map< char *, SDL_Surface*, cmp_str>::iterator pathIter = pathMap.find(path);
	if (pathIter != pathMap.end())
	{
		pSurface = pathIter->second;
	}
	else
	{
		if (fromat == BMP)
		{
			tmpSurface = SDL_LoadBMP(path);
			pSurface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_ABGR8888, 0);
			if(pSurface == NULL) {
				fprintf(stderr, "SDL_LoadBMP failed: %s\n", SDL_GetError());
			}
		}


		if (fromat == PNG)
		{
			CxImage image;
#ifdef WIN32
			TCHAR pathName[100];
			MultiByteToWideChar(CP_ACP, 0, path, -1, pathName, 100);  
			if (image.Load(pathName, CXIMAGE_FORMAT_PNG))
#else	
			if (image.Load(path, CXIMAGE_FORMAT_PNG))
#endif
			{
				if (image.IsValid())
				{
					unsigned char * buff = NULL;
					int nsize = 0;
					image.Encode(buff, nsize, CXIMAGE_FORMAT_BMP);
					SDL_RWops* tmp = SDL_RWFromMem(buff, nsize);
					tmpSurface = SDL_LoadBMP_RW(tmp, 0);
					pSurface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_ABGR8888, 0);
					if(pSurface == NULL) 
					{
						fprintf(stderr, "SDL_LoadBMP failed: %s\n", SDL_GetError());
					}
					image.FreeMemory(buff);
				}
			}
		}

		if (pSurface)
		{
			countMap.insert(pair<SDL_Surface*, int>(pSurface, 0));
			pathMap.insert(pair<char *, SDL_Surface*>(path, pSurface));
		}
	}
	if (tmpSurface)
		SDL_FreeSurface(tmpSurface);
	
	return pSurface;
}

void SourceManager::INCQuote(SDL_Surface* value)
{
	map<SDL_Surface *, int>::iterator iter;
	iter = countMap.find(value);
	if (iter != countMap.end())
	{
		iter->second = iter->second + 1;
	}
}

void SourceManager::SUBQuote(SDL_Surface* value)
{
	map<SDL_Surface *, int>::iterator iter;
	iter = countMap.find(value);
	if (iter != countMap.end())
	{
		iter->second = iter->second - 1;
		if (iter->second == 0)
		{
			map< char *, SDL_Surface*, cmp_str>::iterator pathIter;
			pathIter = pathMap.begin();
			while(pathIter != pathMap.end())
			{
				if (pathIter->second == iter->first)
				{
					pathMap.erase(pathIter);
					break;
				}
				pathIter++;
			}

			SDL_FreeSurface(iter->first);
			countMap.erase(iter);
		}
	}
}

SDL_Surface* SourceManager::GetSurface(SDL_RWops* source, SOURCEFROMAT fromat)
{
	SDL_Surface* pSurface = NULL;
	SDL_Surface* tmpSurface = NULL;
	if (fromat == PNG)
	{
		tmpSurface = IMG_LoadPNG_RW(source);
		pSurface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_ABGR8888, 0);
		if(pSurface == NULL) {
			fprintf(stderr, "IMG_LoadPNG_RW failed: %s\n", SDL_GetError());
		}
	}

	if (pSurface)
	{
		countMap.insert(pair<SDL_Surface*, int>(pSurface, 0));
	}
	if (tmpSurface)
		SDL_FreeSurface(tmpSurface);
	return pSurface;
}

SDL_Surface* SourceManager::GetSurface(string source, SOURCEFROMAT fromat)
{
	SDL_Surface* pSurface = NULL;
	SDL_Surface* tmpSurface = NULL;
	if (fromat ==  PNG)
	{
		SDL_RWops* tmp = SDL_RWFromMem((void *)source.c_str(), source.size());
		tmpSurface = IMG_LoadPNG_RW(tmp);
		pSurface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_ABGR8888, 0);
		if(pSurface == NULL) {
			fprintf(stderr, "IMG_LoadPNG_RW failed: %s\n", SDL_GetError());
		}
	}

	if (fromat ==  BMP)
	{
		SDL_RWops* tmp = SDL_RWFromMem((void *)source.c_str(), source.size());
		tmpSurface = SDL_LoadBMP_RW(tmp, 1);
		pSurface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_ABGR8888, 0);
		if(pSurface == NULL) {
			fprintf(stderr, "IMG_LoadPNG_RW failed: %s\n", SDL_GetError());
		}
	}

	if (pSurface)
	{
		countMap.insert(pair<SDL_Surface*, int>(pSurface, 0));
	}
	if (tmpSurface)
		SDL_FreeSurface(tmpSurface);

	return pSurface;
}

SDL_Surface* SourceManager::GetSurface(SOURCEFROMAT fromat, int w, int h)
{
	SDL_Surface* pSurface = NULL;
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	if (fromat == GRAY)
	{
		pSurface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
		if(pSurface == NULL) {
			fprintf(stderr, "SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
		}
		SDL_FillRect(pSurface, NULL, SDL_MapRGBA(pSurface->format, 225, 225, 225, 255));
	}

	if (pSurface)
	{
		countMap.insert(pair<SDL_Surface*, int>(pSurface, 0));
	}

	return pSurface;
}

SDL_Surface* SourceManager::GetSurface(int r, int g, int b , int a, int w, int h)
{
	SDL_Surface* pSurface = NULL;
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	pSurface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
	if(pSurface == NULL) {
		fprintf(stderr, "SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
	}
	SDL_FillRect(pSurface, NULL, SDL_MapRGBA(pSurface->format, r, g, b, a));

	if (pSurface)
	{
		countMap.insert(pair<SDL_Surface*, int>(pSurface, 0));
	}

	return pSurface;
}

SDL_Surface* SourceManager::GetSurface(SDL_Surface* source)
{
	SDL_Surface* pSurface = NULL;
	pSurface = source;
	return pSurface;
}