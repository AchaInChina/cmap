#include "MyMutex.h"


CMyMutex::CMyMutex(void)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&m_mutex, &attr);
}


CMyMutex::~CMyMutex(void)
{
	pthread_mutex_destroy(&m_mutex);
}

bool CMyMutex::Lock()
{
	pthread_mutex_lock(&m_mutex);

	return true;
}

bool CMyMutex::Unlock()
{
	pthread_mutex_unlock(&m_mutex);
	return true;
}
