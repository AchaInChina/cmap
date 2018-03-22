#include "MapProvider.h"
#include "GPoint.h"
#include "ximage.h"
#include "curl.h"
#include "PureImage.h"
#include "PureProjection.h"

int MapProvider::TimeoutMs = 5 * 1000;

MapProvider::MapProvider(void)
{
	MaxZoom = 17;
	languageStr = "en";
	BypassCache = false;
	InvertedAxisY = false;
}


MapProvider::~MapProvider(void)
{
}

bool MapProvider::IsInitializedGet()
{
	return isInitialized;
}

void MapProvider::IsInitializedSet(bool value)
{
	isInitialized = value;
}

string MapProvider::LanguageStrGet()
{
	return languageStr;
}

void MapProvider::LanguageStrSet(string value)
{
	languageStr = value;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, string &content)  
{
	//int written = fwrite(ptr, size, nmemb, (FILE *)stream);
	long sizes = size * nmemb;
	string temp((char *)ptr, sizes);
	content += temp;
	return sizes;
}

static int index = 0;

string MapProvider::getUrl(string url)
{
	CURL* curl;
	CURLcode res;
	string png;
	struct curl_slist *headers = NULL;
	//headers = curl_slist_append(headers, "Accept: Agent-007");
	curl = curl_easy_init();
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		char urlbuff[1024] = {0};
		strcpy(urlbuff, url.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, urlbuff);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &png);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			//curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
			return png;
		}

		long retcode  = 0;
		res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode );
		if ( (res == CURLE_OK) && (retcode  = 200) )
		{
			double length = 0;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);
			while(1)
			{
				int allcount = png.size();
				if (png.size() >= length)
				{
					break;
				}
			}
		}
		curl_easy_cleanup(curl);
		return png;
	}
	curl_easy_cleanup(curl);
	return png;
}

//#define u_int8_t unsigned char 

void LockCleanUp:: CleanUpBuff(void * arg)
{
	delete arg;
}

PureImage MapProvider::GetTileImageUsingHttp(string url)
{
	PureImage ret;

	string tmp = getUrl(url);

	CxImage mempng((uint8_t *)tmp.c_str(), tmp.size(), CXIMAGE_FORMAT_PNG);
	if (mempng.GetType() == CXIMAGE_FORMAT_PNG)
	{
		unsigned char * buff = NULL;
		int nsize = 0;
		//pthread_cleanup_push(LockCleanUp:: CleanUpBuff, buff);
		mempng.Encode(buff, nsize, CXIMAGE_FORMAT_BMP);
		string str( (char *)buff, nsize);
		ret.Data = str;
		mempng.FreeMemory(buff);
		//pthread_cleanup_pop(0);
	}
	//ret.Data = tmp;
	return ret;
}

int MapProvider::GetServerNum(GPoint pos, int max)
{
	return (int)(pos.X + 2 * pos.Y) % max;
}

bool MapProvider::Equals(MapProvider* obj)
{
	if(  strcmp(Id().c_str(), obj->Id().c_str()) == 0 )
	{
		return true;
	}
	return false;
}

string MapProvider::ToString()
{
	return name;
}