#include "MyImageGetTask.h"
#include "MapCore.h"

MyImageGetTask::MyImageGetTask(int id):CTask(id)
{
	workType = GETIMAGE;
}

MyImageGetTask::~MyImageGetTask(void)
{
}

void MyImageGetTask::taskProc()
{
	MapCore::ProcessLoadTask(pValue);
}
