// Face.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <Windows.h>
#include <omp.h>
#include "FileReader.h"
#include "FaceRecognition.h"
#include "ThreadPool.h"
#include "Logger.h"
#include "Train.h"
#include "Output.h"
using namespace std;

#define MUTITHREAD 1
#define THREADNUM 8

int main(int argc, char **argv) {

	if (argc != 4) {
		printf("Face Recognition toolkit v 0.1a\n\n");
		printf("Parameters for testing:\n");
		printf("Face.exe pepolelist.txt Videolist.txt D:\\Result\n");
	}
	else
	{
		char *logPath = "./log.txt";
		string outPutPath = string(argv[3]);
		string outPutFileName = "武汉大学_人脸识别_第1组";
		FileReader fileReader;
		fileReader.readPeopleList(argv[1]);
		fileReader.readVideoList(argv[2]);
		train();
		vector<string> videoList = fileReader.getVideoList();
		Logger logger(logPath, (unsigned int)videoList.size());

#ifdef MUTITHREAD 

		ThreadPool pool(THREADNUM);
		std::vector< std::future<int> > results;

		for (int i = 0; i < videoList.size(); ++i) {
			if (logger.IsReaded(i))
				continue;
			results.emplace_back(
				pool.enqueue([i, videoList,outPutPath,outPutFileName, &logger] {

				FaceRecognization fa;
				fa.setoutputDir(outPutPath);
				fa.setoutputFile(outPutFileName);
#ifdef DEBUG
				cout << videoList[i] << endl;
#endif
				DWORD start_time = GetTickCount();
				fa.detectMovie(videoList[i]);
				logger.WriteLog(i);
				DWORD end_time = GetTickCount();
#ifdef DEBUG	
				std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
#endif
				return i;
			})
				);
		}
#else		

		for (int i = 0; i < videoList.size(); i++) {
			FaceRecognization fa;
#ifdef DEBUG
			cout << videoList[i] << endl;
#endif
			DWORD start_time = GetTickCount();
			fa.startWork(fileReader.getNameFilePath(), fileReader.getIndexFilePath());
			fa.detectMovie(videoList[i]);
			DWORD end_time = GetTickCount();
#ifdef DEBUG
			std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
#endif
		}
#endif

}

	system("Pause");
	return 0;
}
