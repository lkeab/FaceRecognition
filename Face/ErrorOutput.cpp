#include "stdafx.h"
#include"ErrorOutput.h"
using namespace std;

ErrorOutput::ErrorOutput(string path)
{
	this->errorFile.open(path, ios::out | ios::in | ios::app);
	if (!errorFile.is_open())
	{
#ifdef DEBUG
		cout << "打开error文件失败" << endl;
#endif
	}
}


void ErrorOutput::Print(string msg)
{
	m.lock();
	this->errorFile << msg << endl;
	m.unlock();
}