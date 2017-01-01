

#include "stdafx.h"
#include "FileReader.h"
#include <sstream>


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
	nameFilePath = "name.txt";
	indexFilePath = "index.txt";
}

FileReader::~FileReader()
{
}
void FileReader::readPeopleList(const char *filename)
{
	ifstream in(filename);
	string line;
	int i = 0;
	if (in) {
		while (getline(in, line)) {
			vector<string> temp = splitWithStl(line, string(";"));
			string imagePath, peopleID;
			imagePath = temp[0];
			peopleID = temp[1];
#ifdef DEBUG
			std::cout << "ImagePath is "
				<< imagePath
				<< " and people ID is "
				<< peopleID
				<< endl;
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
		std::cerr << "no peoplelist file" << endl;
		exit(1);
	}
	if ((nameList.size() == imageList.size()) && (nameList.size() * imageList.size() != 0)) {
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
		std::cout << "no videolist file" << endl;
		exit(1);
	}
	if (videoList.size() != frameNums.size()) {
		cerr << "Videolist is wrong;\n";
		exit(1);
	}
}

void FileReader::generateNameFile()
{
	ofstream nameFile;
	nameFile.open(nameFilePath, ios::out | ios::trunc);
	if (!nameFile.is_open())
	{
		cerr << "Cannot open file\n";
		exit(1);
	}
	if (name2id.size() == 0) {
		cerr << "peoplelist is wrong\n";
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
	nameFile.open(indexFilePath, ios::out | ios::trunc);
	if (!nameFile.is_open())
	{
		cerr << "Cannot open file\n";
		exit(1);
	}
	if (imageList.size() == 0) {
		cerr << "imagelist is wrong\n";
		exit(1);
	}
	for (int i = 0; i < imageList.size(); i++) {
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