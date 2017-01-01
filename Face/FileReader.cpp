#include "stdafx.h"
#include "FileReader.h"
#include <sstream>
#include <regex>
#include "Train.h"
#include <cv.h>
#include "highgui.h"

extern void Error(string msg);
std::vector<std::string> split(const string& input, const string& regex) {
	// passing -1 as the submatch index parameter performs splitting
	std::regex re(regex);
	std::sregex_token_iterator
		first{ input.begin(), input.end(), re, -1 },
		last;
	return{ first, last };
}

vector<string> splitWithStl(const string &str, const string &pattern)
{
	vector<string> resVec;

	if ("" == str)
	{
		return resVec;
	}
	//方便截取最后一段数据
	string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != string::npos)
	{
		string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

FileReader::FileReader()
{
	nameFilePath = "./Data/Temp/name.txt";
	indexFilePath = "./Data/Temp/index.txt";
	imageOutputPath = "./Data/Temp/images";
	maxIndex = 0;
	readOriginalNameFile();
	readOriginalIndexFile();
}

FileReader::~FileReader()
{
}

void FileReader::readPeopleList(const char *filename)
{
	ifstream in(filename);
	string line;
	int i = maxIndex + 1;
	if (in) {
		while (getline(in, line)) {
			vector<string> temp = split(line, string("\\s+"));
			string imagePath, peopleID;
			imagePath = temp[0];
			peopleID = temp[1];
#ifdef DEBUG
			std::cout << "ImagePath is "
				<< imagePath
				<< " and people ID is "
				<< peopleID
				<< endl;
			Error("ImagePath is "
				+ imagePath
				+ " and people ID is "
				+ peopleID);
#endif
			map<string, int>::iterator iter;
			iter = name2id.find(peopleID);
			if (iter == name2id.end()) {
				name2id[peopleID] = i;
				i++;
			}
			nameList.push_back(peopleID);
			imageList.push_back(imagePath);
		}
	}
	else {
#ifdef DEBUG
		std::cerr << "no peoplelist file" << endl;
		Error("no peoplelist file");
#endif
		exit(1);
	}
	if ((nameList.size() * imageList.size() != 0)) {
		generateIndexFile();
		generateNameFile();
	}
	else {
		exit(1);
	}

}
void FileReader::readVideoList(const char *filename)
{
	ifstream in(filename);
	string line;
	if (in) {
		while (getline(in, line)) {
			string videoPath, frameNum;
			istringstream is(line);
			is >> videoPath >> frameNum;
#ifdef DEBUG
			std::cout << "videoPath is "
				<< videoPath
				<< " and frameNum is "
				<< frameNum
				<< "\n";
			Error("videoPath is "
				+ videoPath
				+ " and frameNum is "
				+ frameNum
				+ "\n");
#endif
			videoList.push_back(videoPath);

			stringstream ss;
			ss << frameNum;
			int num;
			ss >> num;
			frameNums.push_back(num);
		}
	}
	else {
#ifdef DEBUG
		std::cout << "no videolist file" << endl;
		Error("no videolist file");
#endif
		exit(1);
	}
	if (videoList.size() != frameNums.size()) {
#ifdef DEBUG
		cerr << "Videolist is wrong;\n";
		Error("Videolist is wrong;\n");
#endif
		exit(1);
	}
}

void FileReader::generateNameFile()
{
	ofstream nameFile;
	nameFile.open(nameFilePath, ios::out | ios::app);
	if (!nameFile.is_open())
	{
#ifdef DEBUG
		cerr << "Cannot open file\n";
		Error("Cannot open file\n");
#endif
		exit(1);
	}
	if (name2id.size() == 0) {
#ifdef DEBUG
		cerr << "peoplelist is wrong\n";
		Error("peoplelist is wrong\n");
#endif
		exit(1);
	}
	map<string, int>::iterator it;
	for (it = name2id.begin(); it != name2id.end(); ++it) {
		stringstream ss;
		ss << it->second;
		nameFile << ss.str() << ";" << it->first << "\n";
	}
	nameFile.close();
}
void FileReader::generateIndexFile()
{
	ofstream nameFile;
	nameFile.open(indexFilePath, ios::out | ios::app);
	if (!nameFile.is_open())
	{
#ifdef DEBUG
		cerr << "Cannot open file\n";
		Error("Cannot open file\n");
#endif
		exit(1);
	}
	if (imageList.size() == 0) {
#ifdef DEBUG
		cerr << "imagelist is wrong\n";
		Error("imagelist is wrong\n");
#endif
		exit(1);
	}

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	for (int i = 0; i < imageList.size(); i++) {
		_splitpath(imageList[i].c_str(), drive, dir, fname, ext);
		
		cutImage(imageList[i], imageOutputPath + "/" + string(fname) + string(ext));

		imageList[i] = imageOutputPath + "/" + string(fname) + string(ext);
		
		stringstream ss;
		map<string, int>::iterator iter;
		iter = name2id.find(nameList[i]);
		if (iter == name2id.end()) {
			exit(1);
		}
		ss << iter->second;
		nameFile << imageList[i] << ";" << ss.str() << "\n";
	}
	nameFile.close();



}
void FileReader::cutImage(string imageFile,string outputName)
{
	IplImage* image = cvLoadImage(imageFile.c_str());
	imageTransform(image,outputName);
	cvReleaseImage(&image);
}

void FileReader::readOriginalNameFile()
{
	ofstream nameFile;
	nameFile.open(nameFilePath, ios::out | ios::trunc);
	if (!nameFile.is_open())
	{
#ifdef DEBUG
		cerr << "Cannot create file\n";
		Error("Cannot create file\n");
#endif
		exit(1);
	}
	ifstream in(OriginalNameFile);
	string line;
	if (in) {
		while (getline(in, line)) {
			nameFile << line;
			}
	}
	else {
#ifdef DEBUG
		std::cout << "no name file" << endl;
		Error("no name file");
#endif
		//exit(1);
	}
	in.close();
	nameFile.close();
}
void FileReader::readOriginalIndexFile()
{
	ofstream indexFile;
	indexFile.open(indexFilePath, ios::out | ios::trunc);
	if (!indexFile.is_open())
	{
#ifdef DEBUG
		cerr << "Cannot open file\n";
		Error("Cannot open file\n");
#endif
		exit(1);
	}
	ifstream in(OriginalIndexFile);
	string line;
	if (in) {
		while (getline(in, line)) {
			stringstream ss;
			ss << split(line, ";")[1];
			int i;
			ss >> i;
			if (i >= maxIndex)
				maxIndex = i;
			indexFile << line << endl;
		}
	}
	else {
#ifdef DEBUG
		std::cout << "no videolist file" << endl;
		Error("no videolist file");
#endif
		exit(1);
	}
	in.close();
	indexFile.close();

	
}