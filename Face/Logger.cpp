#include "stdafx.h"
#include"Logger.h"

using namespace std;

//void Logger::Error(char* err)
//{
//#ifdef DEBUG
//	cout << err << endl;
//#endif
//}
extern void Error(string msg);
extern string to_string(int t);
extern string to_string(double t);
extern string to_string(float t);

Logger::Logger(char * path, unsigned int videoNum)
{
	if (videoNum <= 0)
	{
		Error("Logger constructor param videoNum is less than or equal to 0");
	}
	InitLog(path, videoNum);
}

void Logger::InitLog(char *path, unsigned int videoNum)
{
	this->readed = new char[videoNum];
	this->size = videoNum;
	bool isLogExist = true;
	this->log.open(path, std::ios::out | std::ios::in);
	if (!this->log.is_open())
	{
		this->log.open(path, std::ios::out | std::ios::app);
		this->log.close();
		this->log.open(path, std::ios::out | std::ios::in);
		if (!this->log.is_open())
		{
			Error("´ò¿ªlogÊ§°Ü");
		}
		isLogExist = false;
	}
	if (!isLogExist)
	{
		this->log.seekp((off_t)0, ios::beg);
		char s = '0';
		for (unsigned int i = 0; i < videoNum; i++)
		{
			this->log << s;
		}
		return;
	}
	else
	{
		ReadLog();
		return;
	}
}

void Logger::ReadLog()
{
	this->m.lock();
	this->log.seekg((off_t)0, ios::beg);
	for (int i = 0; i < this->size; i++)
	{
		this->readed[i] = this->log.get();
	}
	this->m.unlock();
	return;
}

void Logger::WriteLog(int pos)
{
	char s = '1';
	this->m.lock();
	this->log.seekp((off_t)pos, ios::beg);
	this->log << s;
	readed[pos] = '1';
	this->m.unlock();
}

bool Logger::IsReaded(int pos)
{
	this->m.lock();
	bool b = readed[pos] == '1';
	this->m.unlock();
	return b;
}