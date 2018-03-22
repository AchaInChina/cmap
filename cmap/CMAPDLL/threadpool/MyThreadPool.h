#pragma once
#include<list>
#include "MyMutex.h"
#include "MyStack.h"
#include "MyList.h"
#include"MyQueue.h"
class CMyThread;
class CTask;
enum PRIORITY
{
	NORMAL,
	HIGH
};
class CBaseThreadPool
{
public:
	virtual bool SwitchActiveThread(CMyThread*)=0;
};
class CMyThreadPool:public CBaseThreadPool
{
public:
	CMyThreadPool(int num);
	~CMyThreadPool(void);
public:
	virtual CMyThread* PopIdleThread();
	virtual bool SwitchActiveThread(CMyThread*);
	virtual CTask*GetNewTask();
	virtual void Taskdispatcher( CTask *task, CMyThread* pThread);
public:
	//priorityΪ���ȼ��������ȼ������񽫱����뵽���ס�
	bool addTask(CTask*t,PRIORITY priority);
	bool start();//��ʼ���ȡ�
	bool destroyThreadPool();
private:
	int m_nThreadNum;
	bool m_bIsExit;
public:
	int m_WorksOnThread;
	
	CMyStack m_IdleThreadStack;
	CMyList m_ActiveThreadList;
	CMyQueue m_TaskQueue;
};

