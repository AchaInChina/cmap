#pragma once
#include "windows.h"
#include "head.h"
#include "MyCond.h"
class CTask;
class CBaseThreadPool;
class CMyThread
{
public:
	CMyThread(CBaseThreadPool*threadPool);
	~CMyThread(void);
public:
	bool startThread();
	bool suspendThread();
	bool resumeThread();
	bool assignTask(CTask*pTask);
	bool startTask();
	void stopThread();
	void killThread();
	void exit();

	static void* threadProc( void* param );

	HANDLE m_hEvent;
	
	
private:
	pthread_t m_hThread;
	bool m_bIsExit;
	MyCond m_Cond;
	CTask*m_pTask;
	CBaseThreadPool*m_pThreadPool;	
};
