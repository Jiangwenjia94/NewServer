#include "LogEvent.h"


LogEvent::LogEvent()
{
	
}


LogEvent::~LogEvent()
{
}

void LogEvent::WriteLog(string logtext)
{
	ofstream out;
	out.open("out.txt", ios::app | ios::out);
	out << logtext << endl;
	out.close();
}