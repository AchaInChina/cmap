#pragma once
#include "head.h"
//#include "MyMutex.h"
#include "RectLatLng.h"
#include "GPoint.h"

extern class PureImage;
extern class PureProjection;

class EXPORTS_CLASS MapProvider
{
public:
	MapProvider(void);
	~MapProvider(void);

	string name;
	string id;
	//static vector<MapProvider> MapProviders;
	bool isInitialized ;
	RectLatLng Area;
	int MinZoom;
	int MaxZoom ;
	static int TimeoutMs;
	string RefererUrl;
	string Copyright;
	bool InvertedAxisY;
	string languageStr;
	bool BypassCache ;

	virtual string Name() = 0;
	virtual string Id() = 0;
	virtual PureProjection* Projection() = 0;
	virtual vector<MapProvider*>& Overlays() = 0;
	virtual PureImage GetTileImage(GPoint pos, int zoom) = 0;
	bool IsInitializedGet();
	void IsInitializedSet(bool value);
	virtual void OnInitialized(){};
	string LanguageStrGet();
	void LanguageStrSet(string value);
	PureImage GetTileImageUsingHttp(string url);
	static int GetServerNum(GPoint pos, int max);
	bool Equals(MapProvider* obj);
	string ToString();
	string getUrl(string url);
	//CMyMutex saveLock;
};



