#ifndef _HEAD_H_
#define _HEAD_H_

#pragma once
#include <float.h>
#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <typeinfo>
#include <sstream>
#include <iostream>
using namespace std;

#ifdef WIN32

#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>

#include <crtdbg.h>

#endif


#include "delegate.h"

#include "glew.h"
#include "wglew.h"
#include "pthread.h"

//#include "vld.h"



#ifdef _EXPORTS
#define EXPORTS_CLASS __declspec(dllexport)
#else
#define EXPORTS_CLASS __declspec(dllimport)
#endif

#define Max(a,b) ((a) > (b) ? (a) : (b))
#define Min(a,b) ((a) > (b) ? (b) : (a))

int round(float f);

enum SOURCEFROMAT
{
	GRAY,
	SDL_RWOPS,
	BMP,
	PNG
};

enum TASKTYPE
{
	GETIMAGE,
	PROEVENT
};


//par code 为 0 这是地图底图的重绘，控制在111MS内只绘制一次。
//par code 为 1 这是地图图层的重绘，不控制绘制次数。
//par code 为 2 这是地图图层的重绘，最后一次绘制，必须显示。
struct ReDrawSt
{
	int code ;
	void* par1;
	void* par2;
};

typedef struct GetPsInPolygon{
	/*多边形计算*/
	int    polySides;  
	double  polyX[10000];  
	double  polyY[10000];  
	double  x;
	double  y;  

	bool pointInPolygon() 
	{
		int i;
		int j = polySides-1;
		bool  oddNodes = false  ;

		for (i=0;i<polySides; i++) 
		{
			if( ( polyY[i]<y && polyY[j]>=y ) ||  ( polyY[j]<y && polyY[i]>=y ) ) 
			{
				if(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x) 
				{
					oddNodes=!oddNodes;
				}
			}
			j=i;
		}
		return oddNodes; 
	}
}GETPSINPOLYGON;


//GL顶点
struct Vertex{
	GLubyte r, g, b, a;
	GLfloat x, y, z;
};

//GL纹理顶点
struct VertexST{
	GLfloat s,t;
	GLfloat x, y, z;
};


//计算量直线相交

struct Point
{
	Point(){};
	Point(double x,double y):x(x),y(y){};
	double x,y;
};

struct Seg
{
	Point p1,p2;
};

class LineCrosss
{

//#define N 105

	//const double eps = 1e-6;

	//const double Pi = acos(-1.0);



	int sgn(double x)
	{
		double eps = 1e-6;
		return x<-eps ? -1 : (x>eps);
	}

	double Cross(const Point& p1,const Point& p2,const Point& p3,const Point& p4)
	{
		return (p2.x-p1.x)*(p4.y-p3.y) - (p2.y-p1.y)*(p4.x-p3.x);
	}

	double Area(const Point& p1,const Point& p2,const Point& p3)
	{
		return Cross(p1,p2,p1,p3);
	}

	double fArea(const Point& p1,const Point& p2,const Point& p3)
	{
		return fabs(Area(p1,p2,p3));
	}

	bool Meet(const Point& p1,const Point& p2,const Point& p3,const Point& p4)
	{
		return max(min(p1.x,p2.x),min(p3.x,p4.x)) <= min(max(p1.x,p2.x),max(p3.x,p4.x))
			&& max(min(p1.y,p2.y),min(p3.y,p4.y)) <= min(max(p1.y,p2.y),max(p3.y,p4.y))
			&& sgn(Cross(p3,p2,p3,p4) * Cross(p3,p4,p3,p1)) >= 0
			&& sgn(Cross(p1,p4,p1,p2) * Cross(p1,p2,p1,p3)) >= 0;
	}

	Point Inter(const Point& p1,const Point& p2,const Point& p3,const Point& p4)
	{
		double k = fArea(p1,p2,p3) / fArea(p1,p2,p4);
		return Point((p3.x + k*p4.x)/(1+k),(p3.y + k*p4.y)/(1+k));
	}

};

class SPLIT
{
public:
	//字符串分割函数
	static std::vector<std::string> split(std::string str,std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str+=pattern;//扩展字符串以方便操作
		int size=str.size();

		for(int i=0; i<size; i++)
		{
			pos=str.find(pattern,i);
			if(pos<size)
			{
				std::string s=str.substr(i,pos-i);
				result.push_back(s);
				i=pos+pattern.size()-1;
			}
		}
		return result;
	}

};


class MapColor
{
public:
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
	MapColor()
	{

	}

	MapColor(int rvalue, int gvalue, int bvalue, int avalue = 255)
	{
		r = rvalue;
		g = gvalue;
		b = bvalue;
		a = avalue;
	}
};

enum MOUSE_BUTTON
{
	MOUSE_LEFT,
	MOUSE_MIDDL,
	MOUSE_RIGHT,
	MOUSE_NULL,
};

class MouseEventArg
{
public:
	MOUSE_BUTTON buttonType;
	int X;
	int Y;
	int Step;
};


class LockCleanUp
{
public:
	static void CleanUpLock(void * arg);
	static void CleanUpBuff(void * arg);
};


#endif
