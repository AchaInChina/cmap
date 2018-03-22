#include "MyGraphEventProTask.h"
#include "MapGraph.h"

MyGraphEventProTask::MyGraphEventProTask(int id):CTask(id)
{
	workType = PROEVENT;
}


MyGraphEventProTask::~MyGraphEventProTask(void)
{
}

void MyGraphEventProTask::taskProc()
{
	MapGraph::AsysGraphEventPro(userdata, pEvent);
	delete pEvent;
}
