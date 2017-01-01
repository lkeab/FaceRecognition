#pragma
#include<stdio.h>
#include<iostream>

using namespace std;
//typedef char byte;

class Logger
{
private:
	char* readed;
	FILE *log;
	int size;
public:
	Logger(char *path, int videoNum);	//传递文件总数参数
	~Logger(){
		delete[] readed;
		fclose(log);
	}
	void WriteLog(int pos);		//传递已读完文件编号,计数从0开始
	void InitLog(char *path, int videoNum);
	void ReadLog();		//将日志中的数据读入记录数组
	bool IsReaded(int pos);		//获取该编号视频是否已读完,是返回true,否返回false
	void Error(char* err);	//输出错误信息
};