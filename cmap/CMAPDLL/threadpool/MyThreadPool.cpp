#include "MyThreadPool.h"
#include "MyThread.h"
#include "Task.h"
#include<cassert>
#include<iostream>
#include "MyQueue.h"
CMyThreadPool::CMyThreadPool(int num)
{
	m_bIsExit=false;
	m_WorksOnThread = 0;
	for(int i=0;i<num;i++)
	{
		CMyThread*p=new CMyThread(this);
		m_IdleThreadStack.push(p);
		p->startThread();
	}
}
CMyThreadPool::~CMyThreadPool(void)
{
	destroyThreadPool();
}
CMyThread* CMyThreadPool::PopIdleThread()
{
	CMyThread *pThread=m_IdleThreadStack.pop();
	//pThread->m_bIsActive=true;
	return pThread;
}
//将线程从活动队列取出，放入空闲线程栈中。在取之前判断此时任务队列是否有任务。如任务队列为空时才挂起。否则从任务队列取任务继续执行。
bool CMyThreadPool::SwitchActiveThread( CMyThread*t)
{
	CTask *pTask=NULL;
	pTask=m_TaskQueue.pop();
	if (pTask != NULL )
	{
		Taskdispatcher(pTask, t);
	}
	else
	{
		m_ActiveThreadList.removeThread(t);
		m_IdleThreadStack.push(t);
	}

	return true;
}
bool CMyThreadPool::addTask( CTask*t,PRIORITY priority )
{
	assert(t);
	if(!t||m_bIsExit)
		return false;	
	CTask *task=NULL;
	//std::cout<<" 【"<<t->getID()<<"】添加！"<<std::endl;//
	if(priority==PRIORITY::NORMAL)
	{
		m_TaskQueue.push(t);//进入任务队列。//
	}
	else if(PRIORITY::HIGH)
	{
		m_TaskQueue.pushFront(t);//高优先级任务。//
	}

	task=m_TaskQueue.pop();//取出列头任务。//
	if(task)
	{
		Taskdispatcher( task,NULL);
	}
}
bool CMyThreadPool::start()
{
	return 0;
}
CTask* CMyThreadPool::GetNewTask()
{
	if(m_TaskQueue.isEmpty())
	{
		return NULL;
	}
	CTask *task=m_TaskQueue.pop();//取出列头任务。
	if(task==NULL)
	{
		std::cout<<"任务取出出错。"<<std::endl;
		return 0;
	}
	return task;
}
bool CMyThreadPool::destroyThreadPool()
{
	m_bIsExit=true;
	m_IdleThreadStack.clear();
	m_ActiveThreadList.clear();
	m_TaskQueue.clear();
	return true;
}

void CMyThreadPool::Taskdispatcher(CTask *task, CMyThread* pThread)
{
	if (pThread)
	{
		m_TaskQueue.m_DeleLock.Lock();
		map<CTask*,bool>::iterator iter = m_TaskQueue.m_TaskDeleFlag.find(task);
		if ( (iter !=  m_TaskQueue.m_TaskDeleFlag.end()) && iter->second)
		{
			int count = 0;
			bool hasDo = false;
			if (task->workType == GETIMAGE)
			{
				count = m_WorksOnThread;
			}
			else
			{
				count = 1;
			}
			task->lock.Lock();
			task->InWorkThread++;
			task->lock.Unlock();
			//std::cout<<"【"<<pThread->m_threadID<<"】 执行   【"<<task->getID()<<"】"<<std::endl;
			pThread->assignTask(task);
			pThread->startTask();
			hasDo = true;
			count--;

			while(count > 0)
			{
				CMyThread*pThread=PopIdleThread();
				if (pThread != NULL)
				{
					task->lock.Lock();
					task->InWorkThread++;
					task->lock.Unlock();
					//std::cout<<"【"<<pThread->m_threadID<<"】 执行   【"<<task->getID()<<"】"<<std::endl;
					m_ActiveThreadList.addThread(pThread);
					pThread->assignTask(task);
					pThread->startTask();
				}
				else
				{
					break;
				}
				count--;
			}
			if ( !hasDo )
			{
				m_TaskQueue.pushFront(task);
			}
		}
		m_TaskQueue.m_DeleLock.Unlock();
	}
	else
	{
		if (task)
		{
			m_TaskQueue.m_DeleLock.Lock();
			map<CTask*,bool>::iterator iter = m_TaskQueue.m_TaskDeleFlag.find(task);
			if ( (iter !=  m_TaskQueue.m_TaskDeleFlag.end()) && iter->second)
			{
				int count = 0;
				bool hasDo = false;
				if (task->workType == GETIMAGE)
				{
					count = m_WorksOnThread;
				}
				else
				{
					count = 1;
				}
				while(count > 0)
				{
					CMyThread*pThread=PopIdleThread();
					if (pThread != NULL)
					{
						task->lock.Lock();
						task->InWorkThread++;
						task->lock.Unlock();
						//std::cout<<"【"<<pThread->m_threadID<<"】 执行   【"<<task->getID()<<"】"<<std::endl;
						m_ActiveThreadList.addThread(pThread);
						pThread->assignTask(task);
						pThread->startTask();
						hasDo = true;
					}
					else
					{
						break;
					}
					count--;
				}
				if ( !hasDo )
				{
					m_TaskQueue.pushFront(task);
				}
			}
			m_TaskQueue.m_DeleLock.Unlock();
		}
	}
}
