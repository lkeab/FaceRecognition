#pragma
#include "stdafx.h"
#include<mutex>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<mutex>
using namespace std;

class Logger
{
private:
	char* readed;
	fstream log;
	int size;
	std::mutex m;
public:
	Logger(){ ; }
	Logger(char *path, unsigned int videoNum);
	~Logger(){
		if (readed)
			delete[] readed;
		if (log.is_open())
			log.close();
	}
	void WriteLog(int pos);		
	void InitLog(char *path, unsigned int videoNum);
	void ReadLog();		
	bool IsReaded(int pos);		
	//void Error(char* err);	
};

