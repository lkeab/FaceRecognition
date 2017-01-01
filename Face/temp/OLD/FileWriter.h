#pragma once

#include <fstream>  
#include <string>  
#include <iostream> 
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>

using namespace std;

struct OutputStruct
{
	int start;
	int end;
	double similarity;
};

class FileWriter
{
public:
	FileWriter(string filepath, string filename) :filePath(filepath), fileName(filename){

	};

	void Write(string videoName, string peopleID, double similarity, int num,vector<OutputStruct>&vec);

	~FileWriter();
	
private:
	string filePath;
	string fileName;	
};
