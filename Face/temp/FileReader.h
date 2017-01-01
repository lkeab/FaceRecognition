#pragma once
#include <fstream>  
#include <string>  
#include <iostream> 
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <map>
using namespace std;
class FileReader
{
public:
	FileReader();
	FileReader(string namePath, string indexPath) :nameFilePath(namePath), indexFilePath(indexPath)
	{
	};
	~FileReader();
	void readPeopleList(const char *filename);
	void readVideoList(const char *filename);

	vector<string> getVideoList()
	{
		return videoList;
	};
	vector<int> getFrameList()
	{
		return frameNums;
	};

	string getNameFilePath() {
		return nameFilePath;
	};

	string getIndexFilePath() {
		return indexFilePath;
	}


private:
	string nameFilePath;
	string indexFilePath;

	map<string, int> name2id;

	vector<string> nameList;
	vector<string> imageList;
	vector<string> videoList;
	vector<int> frameNums;

	void generateNameFile();
	void generateImageFile();
	void generateIndexFile();
};



