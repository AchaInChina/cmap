#include "MyThread.h"
#include"task.h"
#include "MyThreadPool.h"
#include "Log.h"
#include<cassert>
#define WAIT_TIME 20
CMyThread::CMyThread(CBaseThreadPool*threadPool)
{
	m_pTask=NULL;
	m_pThreadPool=threadPool;
	//m_hEvent=CreateEvent(NULL,false,false,NULL);
	m_bIsExit=false;
}

//bool CMyThread::m_bIsActive=false;
CMyThread::~CMyThread(void)
{
	//CloseHandle(m_hEvent);

}

bool CMyThread::startThread()
{
	if (pthread_create(&m_hThread,NULL,  threadProc,this) != 0)
	{
		printf("pthread_create failed\n");
		return false;
	}
	return true;
}

bool CMyThread::suspendThread()
{
	//ResetEvent(m_hEvent);
	return true;
}
//有任务到来，通知线程继续执行。//
bool CMyThread::resumeThread()
{
	//SetEvent(m_hEvent);
	m_Cond.Set();
	return true;
}

//DWORD WINAPI CMyThread::threadProc( LPVOID pParam )
void* CMyThread::threadProc( void* pParam )
{
	Log log;

	CMyThread *pThread=(CMyThread*)pParam;
	while(!pThread->m_bIsExit)
	{
		pThread->m_Cond.Wait();
		if (pThread->m_bIsExit)
		{
			return 0;
		}
		
		if(pThread->m_pTask)
		{
			pThread->m_pTask->taskProc();//执行任务。//

			log.LOG("geting m_DeleLock");
			pthread_cleanup_push( LockCleanUp::CleanUpLock, &(((CMyThreadPool*)(pThread->m_pThreadPool))->m_TaskQueue.m_DeleLock));
			((CMyThreadPool*)(pThread->m_pThreadPool))->m_TaskQueue.m_DeleLock.Lock();
			log.LOG("get m_DeleLock");

			log.LOG("geting m_pTask lock");
			pthread_cleanup_push( LockCleanUp::CleanUpLock, &(pThread->m_pTask->lock));
			pThread->m_pTask->lock.Lock();
			log.LOG("get m_pTask lock");

			pThread->m_pTask->InWorkThread--;

			pThread->m_pTask->lock.Unlock();
			pthread_cleanup_pop(0);

			if (pThread->m_pTask->InWorkThread == 0)  //如果执行次数和需要删除的次数相同，则说明任务已完成。//
			{
				//删除时，调用析构函数，释放锁//
				map<CTask*,bool>::iterator iter = ((CMyThreadPool*)(pThread->m_pThreadPool))->m_TaskQueue.m_TaskDeleFlag.find(pThread->m_pTask);
				iter->second = false;
				((CMyThreadPool*)(pThread->m_pThreadPool))->m_TaskQueue.m_TaskDeleFlag.erase(iter);
				delete pThread->m_pTask;
			}

			((CMyThreadPool*)(pThread->m_pThreadPool))->m_TaskQueue.m_DeleLock.Unlock();
			pthread_cleanup_pop(0);

			pThread->m_pTask=NULL;
			pThread->m_pThreadPool->SwitchActiveThread(pThread);				
		}
	}
	return 0;
}
//将任务关联到线程类。
bool CMyThread::assignTask( CTask*pTask )
{
	assert(pTask);
	if(!pTask)
		return false;
	m_pTask=pTask;
	
	return true;
}
//开始执行任务。//
bool CMyThread::startTask()
{
	resumeThread();
	return true;
}

void CMyThread::exit()
{
	pthread_join(m_hThread, NULL);
}

void CMyThread::stopThread()
{
	m_bIsExit = true;
	resumeThread();
	return ;
}

void LockCleanUp::CleanUpLock(void * arg)
{
	CMyMutex* tmp = (CMyMutex*)arg;
	tmp->Unlock();
}

void CMyThread::killThread()
{
	pthread_cancel(m_hThread);
	pthread_join(m_hThread, NULL);
}