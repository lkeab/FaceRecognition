#include "stdafx.h"
#include "FileWriter.h"
#include <fstream>
//#include <iostream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <mutex>

std::mutex m;

extern void Error(string msg);
FileWriter::~FileWriter()
{
}

//double转string
string douConvertToString(double d) {
	ostringstream os;
	if (os << d)
		return os.str();
	return "invalid conversion";
}

//int 转string
string intConvertToString(int d){
	char buf[10];
	sprintf(buf, "%d", d);
	string b = buf;
	return b;
}

void FileWriter::Write(string videoName, string peopleID, double similarity, int num, vector<OutputStruct>&vec){
	m.lock();
	ofstream nameFile;
	num = (int)vec.size();
	nameFile.open(filePath + "/" + fileName + ".txt", std::ios::app | ios::out);
	if (!nameFile.is_open()){
#ifdef DEBUG
		std::cout << "文件打开失败" << std::endl;
		Error("文件打开失败");
#endif
	}
	
	nameFile << videoName << "," << peopleID << "," << setprecision(2) << std::fixed << similarity << "," << num;
	for (int i = 0; i < num; i++){
		nameFile << ",(" << vec[i].start << "," << vec[i].end << "," << setprecision(2) << std::fixed << vec[i].similarity << ")";
	}

	nameFile << "；" << endl;
	nameFile << flush;
	nameFile.close();
	m.unlock();
}

