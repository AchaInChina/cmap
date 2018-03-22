#pragma once
#include "head.h"
#include "MyMutex.h"
class CTask
{
public:
	CTask(int id);
	~CTask(void);
public:
	virtual void taskProc()=0;
	int getID();
	int InWorkThread;
	TASKTYPE workType;
	CMyMutex lock;

	
private:
	int m_ID;
};

