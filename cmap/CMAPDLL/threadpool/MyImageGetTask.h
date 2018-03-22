#pragma once
#include "head.h"
#include "Task.h"

class MyImageGetTask: public CTask
{
public:
	MyImageGetTask(int id);
	~MyImageGetTask(void);

	void * pValue;
	
	virtual void taskProc();
};

