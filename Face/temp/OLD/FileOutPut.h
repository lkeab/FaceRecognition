#pragma once
#include <string>
#include <fstream>  
#include <iostream>
using namespace std;
class FileOutPut
{
private:
	string outputPath;
	string fileNameWithPath;
	bool fileCreated;
public:
	FileOutPut(string outPath) :outputPath(outPath)
	{
		fileCreated = false;
	};
	void createFile(string fileName);
	void writeFile(string output);
	~FileOutPut();
};

