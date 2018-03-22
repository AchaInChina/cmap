#pragma once
#include "head.h"
#include "Task.h"
#include "SDL.h"


class MyGraphEventProTask:public CTask
{
public:
	MyGraphEventProTask(int id);
	~MyGraphEventProTask(void);
	void *userdata;
	SDL_Event * pEvent;
	virtual void taskProc();
};

