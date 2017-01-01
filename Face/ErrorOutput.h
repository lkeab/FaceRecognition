#pragma
#include<mutex>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<mutex>

using namespace std;

class ErrorOutput
{
private:
	fstream errorFile;
	mutex m;
public:
	ErrorOutput(string path);
	void Print(string msg);
	~ErrorOutput()
	{
		if (errorFile.is_open())
			errorFile.close();
	}
};

