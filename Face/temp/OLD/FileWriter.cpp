#include "stdafx.h"
#include "FileWriter.h"
#include <fstream>
//#include <iostream>
#include <sstream>
#include <string>

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

void FileWriter::Write(string videoName, string peopleID, double similarity,int num,vector<OutputStruct>&vec){
	//stringstream ss;
//	locale::global(loc);
	ofstream nameFile;
	num = vec.size();
	nameFile.open(filePath + "//" + fileName + ".txt", std::ios::app|ios::out);
	if (!nameFile.is_open()){
		std::cout << "文件打开失败" << std::endl;
	}
	
	//string append1 = "";
	//string append2 = "";
	//locale::global(loc);
   // nameFile.imbue(locale("", locale::all ^ locale::numeric));
	//string append1 = videoName + "," + _T(peopleID) + "," + douConvertToString(similarity) + "," + intConvertToString(num);
	
	//locale &loc = locale::global(locale(locale(), "", LC_CTYPE));
	//string content = videoName + "," + peopleID + "," + similarity + "," + num;
	//setlocale(LC_ALL, "Chinese-simplified");
	//setlocale(LC_ALL, "C");
	nameFile << videoName << "," << peopleID<< "," << similarity << "," << num;
	for (int i = 0; i < num; i++){
	    //append1 += ",(" + intConvertToString(vec[i].start) + "," + intConvertToString(vec[i].end) + "," + intConvertToString(vec[i].similarity)+ ")";
		nameFile << ",(" << vec[i].start << "," << vec[i].end << "," << vec[i].similarity << ")";
	}
	////nameFile.write(append);

	////nameFile << "fshdjkfhsdkfjksdfjsdlkfjsdlfjsdkfjsldfjosfjwiefjkcnsdkdskfsdfjweofjsdlfjdslkfjaoefjeifjewfasfjsdkfldsjfldsfjkdfjkdsfsklfshdjkfhsdkfjksdfjsdlkfjsdlfjsdkfjsldfjosfjwiefjkcnsdkdskfsdfjweofjsdlfjdslkfjaoefjeifjewfasfjsdkfldsjfldsfjkdfjkdsfskl";
	////nameFile << append1<<append2;
	////nameFile.write(append1.data(), strlen(append1.data()));
	//nameFile << append1;
	/*std::locale::global(std::locale(""));*/
	nameFile << endl;
	nameFile << "中文乱码" << endl;

	nameFile << endl;
	//locale::global(loc);
	nameFile << flush;
 	nameFile.close(); 
}

