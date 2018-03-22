#include "LoadTask.h"

LoadTask::LoadTask(void)
{
	Zoom = 0;
}


LoadTask::~LoadTask(void)
{
}

LoadTask::LoadTask(GPoint pos, int zoom)
{
	Pos = pos;
	Zoom = zoom;
}

string LoadTask::ToString()
{
	char buff[1024] ={0};
	sprintf(buff, "%d", Zoom);
	return string(buff) + " - " + Pos.ToString();
}

bool LoadTask::Equals(LoadTask other)
{
	return (Zoom == other.Zoom && Pos == other.Pos);
}

bool LoadTask::operator ==(LoadTask right)
{
	return (this->Zoom == right.Zoom && this->Pos == right.Pos);
}

LoadTask::LoadTask( const LoadTask &value)
{
	this->Pos = value.Pos;
	this->Zoom = value.Zoom;
}