#pragma
#include "stdafx.h"
#include "Logger.h"

void Logger::Error(char* err)
{
	cout << err << endl;;
}

Logger::Logger(char * path, int videoNum)
{
	if (videoNum <= 0)
	{
		Error("Logger constructor param videoNum is less than or equal to 0");
	}
	InitLog(path, videoNum);
}

void Logger::InitLog(char *path, int videoNum)
{
	this->readed = new char[videoNum];
	this->size = videoNum;
	this->log = fopen(path, "r+");
	if (NULL == this->log)
	{
		Error("can not open log");
		return;
	}
	int ch = fgetc(this->log);
	if (ch == EOF)
	{
		if (fseek(log, 0, SEEK_SET))
		{
			Error("Initial seek failed");
			return;
		}
		char s = '0';
		for (int i = 0; i < videoNum; i++)
		{
			if (1 != fwrite(&s, 1, 1, log))
			{
				Error("init failed");
				return;
			}
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
	if (fseek(this->log, 0, SEEK_SET))
	{
		Error("read log seek failed");
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		fread(&this->readed[i], sizeof(char), 1, log);
		return;
		//cout << "read:" << readed[i] << endl;
	}
}

void Logger::WriteLog(int pos)
{
	char s = '1';
	if (fseek(log, pos, SEEK_SET))
	{
		Error("write seek failed");
		return;
	}
	cout << "write position :" << ftell(log) << endl;
	if (fwrite(&s, 1, 1, log) != 1)
	{
		Error("wrote failed");
		return;
	}
	readed[pos] = '1';
}

bool Logger::IsReaded(int pos)
{
	return readed[pos] == '1';
}