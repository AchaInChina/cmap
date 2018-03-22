#include "Task.h"
#include "windows.h"
CTask::CTask(int id)
{
	m_ID=id;
	InWorkThread = 0;
}
CTask::~CTask(void)
{
}

int CTask::getID()
{
	return m_ID;
}
