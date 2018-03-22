#include "MyQueue.h"
#include"task.h"

CMyQueue::CMyQueue(void)
{
}


CMyQueue::~CMyQueue(void)
{
}

CTask* CMyQueue::pop()
{
	CTask*p=NULL;
	m_mutex.Lock();
	if(!m_TaskQueue.empty())
	{
		p=m_TaskQueue.front();
		m_TaskQueue.pop_front();
	}
	m_mutex.Unlock();
	return p;
}

bool CMyQueue::push(CTask*t)
{
	if(!t)
	{
		return false;
	}
	m_mutex.Lock();
	m_DeleLock.Lock();
	m_TaskQueue.push_back(t);

	map<CTask*,bool>::iterator iter = m_TaskDeleFlag.find(t);
	if ( iter ==  m_TaskDeleFlag.end())
		m_TaskDeleFlag.insert(pair<CTask*, bool>(t, true));

	m_DeleLock.Unlock();
	m_mutex.Unlock();
	return true;
}

bool CMyQueue::isEmpty()
{
	bool ret=false;
	m_mutex.Lock();
	ret=m_TaskQueue.empty();
	m_mutex.Unlock();
	return ret;
}

bool CMyQueue::pushFront( CTask*t )
{
	if(!t)
	{
		return false;
	}
	m_mutex.Lock();
	m_DeleLock.Lock();
	m_TaskQueue.push_front(t);

	map<CTask*,bool>::iterator iter = m_TaskDeleFlag.find(t);
	if ( iter ==  m_TaskDeleFlag.end())
		m_TaskDeleFlag.insert(pair<CTask*, bool>(t, true));

	m_DeleLock.Unlock();
	m_mutex.Unlock();
	return true;
}

bool CMyQueue::clear()
{
	m_DeleLock.Lock();
	m_mutex.Lock();
 	//std::deque<CTask*>::iterator iter=m_TaskQueue.begin();
 	//for(;iter!=m_TaskQueue.end();iter++)
 	//{
 	//	delete (*iter);
 	//}
	m_TaskQueue.clear();
	m_TaskDeleFlag.clear();
	m_mutex.Unlock();
	m_DeleLock.Unlock();
	return true;
}
