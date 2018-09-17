#pragma once
#include <fstream>
#include <string>
using namespace std;

class LogEvent
{
public:
	LogEvent();
	~LogEvent();

	void WriteLog(string logtext);
};

