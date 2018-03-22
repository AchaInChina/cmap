#pragma once
#include<deque>
#include<map>
#include"MyMutex.h"
class CTask;
class CMyQueue
{
public:
	CMyQueue(void);
	~CMyQueue(void);
public:
	CTask*pop();
	bool push(CTask*t);
	bool pushFront(CTask*t);//²åµ½¶ÓÊ×¡£
	bool isEmpty();
	bool clear();
	CMyMutex m_DeleLock;
	std::map<CTask*, bool> m_TaskDeleFlag;
private:
	std::deque<CTask*>m_TaskQueue;
	CMyMutex m_mutex;
};

