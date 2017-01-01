#include "stdafx.h"
#include "FileOutPut.h"

void FileOutPut::createFile(string fileName)
{
	ofstream outputFile;
	fileNameWithPath = outputPath + "/" + fileName;
	outputFile.open(fileNameWithPath, ios::out | ios::trunc);
	if (!outputFile.is_open())
	{
		cerr << "Fatal:Cannot create file\n";
		exit(1);
	}
	outputFile.close();
	fileCreated = true;
}
void FileOutPut::writeFile(string output)
{
	if (!fileCreated) {
		cerr << "Fatal: cannot write file.";
		exit(1);
	}
	ofstream nameFile;
	nameFile.open(fileNameWithPath, ios::out | ios::app);
	if (!nameFile.is_open())
	{
		cerr << "Cannot open file\n";
		exit(1);
	}
	if (output.size() == 0) {
		cerr << "nothing to written\n";
		nameFile.close();
		return;
	}
	nameFile << output << "\n";
	nameFile.close();
}
FileOutPut::~FileOutPut()
{
}
