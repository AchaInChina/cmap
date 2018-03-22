#pragma once
#include "head.h"
class EXPORTS_CLASS CMyMutex
{
public:
	CMyMutex(void);
	~CMyMutex(void);
public:
	bool Lock();
	bool Unlock();
private:
	pthread_mutex_t m_mutex;
};

