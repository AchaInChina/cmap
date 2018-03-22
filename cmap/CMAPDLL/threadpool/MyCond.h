#pragma once
#include "head.h"
class MyCond
{
public:
	MyCond(void);
	~MyCond(void);

	void Wait();
	void Set();

	pthread_cond_t m_Cond;
private:
	pthread_mutex_t m_EventMtx;
	bool isvalid;
};

