#include "MyCond.h"


MyCond::MyCond(void)
{
	pthread_mutex_init(&m_EventMtx, NULL);
	pthread_cond_init(&m_Cond, NULL);
	isvalid =false;
}


MyCond::~MyCond(void)
{
}

void MyCond::Wait()
{
	pthread_mutex_lock(&m_EventMtx);
	while(!isvalid)
	{
		pthread_cond_wait(&m_Cond, &m_EventMtx);
	}
	isvalid =false;
	pthread_mutex_unlock(&m_EventMtx);
}

void MyCond::Set()
{
	pthread_mutex_lock(&m_EventMtx);
	isvalid =true;
	pthread_cond_signal(&m_Cond);
	pthread_mutex_unlock(&m_EventMtx);
}
