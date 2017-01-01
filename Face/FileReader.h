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

std::vector<std::string> split(const string& input, const string& regex);
class FileReader
{
public:
	FileReader();
	FileReader(string namePath, string indexPath) :nameFilePath(namePath), indexFilePath(indexPath)
	{
		maxIndex = 0;
		imageOutputPath = "./Data/Temp/images";
		readOriginalNameFile();
		readOriginalIndexFile();
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
	int maxIndex;

	void readOriginalNameFile();
	void readOriginalIndexFile();

	string nameFilePath;
	string indexFilePath;
	string imageOutputPath;

	map<string, int> name2id;

	vector<string> nameList;
	vector<string> imageList;
	vector<string> videoList;
	vector<int> frameNums;

	void generateNameFile();
	void generateImageFile();
	void cutImage(string imageFile, string outputName);
	void generateIndexFile();
};
/*
步骤如下：
1. 读取原始的name和at两个文件
	将这两个文件的内容放入namelist,imagelist以及name2id中
	readOriginalNameFile
	readOriginalIndexFile

2.读取新文件内容，追加到上述容器中
	readPeopleList

3.写入新文件，结束整个任务
	generateNameFile
	generateIndexFile
	generateImageFile


*/



