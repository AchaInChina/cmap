#include "ImageConver.h"
#include "SourceManager.h"
#include "SDL_image.h"

SourceManager* gSourceManager = NULL;

void ImageConver::GenerSM()
{
	gSourceManager = new SourceManager();
}

void ImageConver::ReleaseSM()
{
	delete gSourceManager;
}

ImageConver::ImageConver()
{
	w = 0;
	h = 0;
	pSurface = NULL;
}

ImageConver::ImageConver(SDL_RWops* source, SOURCEFROMAT fromat)
{
	w = 0;
	h = 0;
	pSurface = gSourceManager->GetSurface(source, fromat);
	if (pSurface != NULL)
	{
		w = pSurface->w;
		h = pSurface->h;
	}
	gSourceManager->INCQuote(pSurface);
}

ImageConver::ImageConver(char * path)
{
	w = 0;
	h = 0;
	vector<string> result =  SPLIT::split(string(path), string("."));
	if (result[1] == "png")
	{
		pSurface = gSourceManager->GetSurface(path, PNG);
	}
	if (result[1] == "bmp")
	{
		pSurface = gSourceManager->GetSurface(path, BMP);
	}

	if (pSurface != NULL)
	{
		w = pSurface->w;
		h = pSurface->h;
	}
	gSourceManager->INCQuote(pSurface);
}

ImageConver::ImageConver(string source, SOURCEFROMAT fromat)
{
	w = 0;
	h = 0;
	pSurface = gSourceManager->GetSurface(source, fromat);
	if (pSurface != NULL)
	{
		w = pSurface->w;
		h = pSurface->h;
	}
	gSourceManager->INCQuote(pSurface);
}

ImageConver::ImageConver(SDL_Surface* source)
{
	w = 0;
	h = 0;
	pSurface = gSourceManager->GetSurface(source);
	if (pSurface != NULL)
	{
		w = pSurface->w;
		h = pSurface->h;
	}
	gSourceManager->INCQuote(pSurface);
}

ImageConver::ImageConver(SOURCEFROMAT fromat, int w, int h)
{
	this->w = 0;
	this->h = 0;
	pSurface = gSourceManager->GetSurface(fromat, w, h);
	if (pSurface != NULL)
	{
		this->w = w;
		this->h = h;
	}
	gSourceManager->INCQuote(pSurface);
}

ImageConver::ImageConver(MapColor bc, int w, int h)
{
	this->w = 0;
	this->h = 0;
	pSurface = gSourceManager->GetSurface( bc.r, bc.g, bc.b, bc.a, w, h);
	if (pSurface != NULL)
	{
		this->w = w;
		this->h = h;
	}
	gSourceManager->INCQuote(pSurface);
}

ImageConver::~ImageConver(void)
{
	gSourceManager->SUBQuote(pSurface);
}

ImageConver ImageConver::Cut(int x, int y, int w, int h)
{
	ImageConver tmp = ImageConver(GRAY, w, h);
	SDL_Surface*  surfacecover = tmp.pSurface;
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	if (pSurface != NULL)
	{
		//SDL_BlitScaled( pSurface, &rect, surfacecover, NULL );
		if (SDL_BlitScaled( pSurface, &rect, surfacecover, NULL ))
		{
			printf("Cut SDL_BlitScaled error: %s\n", SDL_GetError());
			SDL_ClearError();
		}
	}
	tmp.w = 0;
	tmp.h = 0;
	if (surfacecover != NULL)
	{
		tmp.w = w;
		tmp.h = h;
	}
	return tmp;
}

ImageConver::ImageConver(const ImageConver& image)
{
	pSurface = image.pSurface;
	w = 0;
	h = 0;
	if (pSurface != NULL)
	{
		w = image.w;
		h = image.h;
	}
	gSourceManager->INCQuote(pSurface);
}

ImageConver& ImageConver::operator = (const ImageConver& image)
{
	if (this == &image)
	{
		return *this;
	}
	if (this->pSurface)
	{
		gSourceManager->SUBQuote(pSurface);
	}

	this->pSurface = image.pSurface;
	w = 0;
	h = 0;
	if (pSurface != NULL)
	{
		w = image.w;
		h = image.h;
	}
	gSourceManager->INCQuote(pSurface);
	return *this;
}