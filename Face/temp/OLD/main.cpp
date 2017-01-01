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
//#include "ThreadPool.h"

using namespace std;

cv::Ptr<cv::FaceRecognizer> model;

int main(int argc, char **argv) {
	if (argc != 4) {
		printf("Face Recognition toolkit v 0.1a\n\n");
		printf("Parameters for testing:\n");
		printf("Face.exe pepolelist.txt Videolist.txt D:\Result\n");
	}
	else
	{

		model = cv::createEigenFaceRecognizer();

		FaceRecognization fa;
		fa.initWorkLoading();
		fa.detectMovie("dhsjd","G://C++//Face//mp4//index2.mp4");

	//	FileReader fileReader;
		/*
		读取peoplelist.txt D:\pepoleimage\image1.jpg 人物ID
		读取Videolist.txt D:\test\video1.mp4 360
		处理输出文件夹
		开始运行
		*/
	/*	fileReader.readPeopleList(argv[1]);
		fileReader.readVideoList(argv[2]);
		cout << argv[1] << "  " << argv[2] << endl;
		vector<string> videoList = fileReader.getVideoList();


#ifdef MUTITHREAD 
		ThreadPool pool(4);
		std::vector< std::future<int> > results;

		for (int i = 0; i < videoList.size(); ++i) {
			results.emplace_back(
				pool.enqueue([i, videoList, &fileReader] {
				FaceRecognization fa;
#ifdef DEBUG
				cout << videoList[i] << endl;
#endif
				DWORD start_time = GetTickCount();
				fa.startWork(videoList[i], fileReader.getNameFilePath(), fileReader.getIndexFilePath());
				fa.detectMovie();
				DWORD end_time = GetTickCount();
				std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
				return i;
			})
				);
		}
		for (auto && result : results)
			std::cout << result.get() << ' ';
		std::cout << std::endl;

#else		

		for (int i = 0; i < videoList.size(); i++)
		{
			FaceRecognization fa;
#ifdef DEBUG
			cout << videoList[i] << endl;
#endif
			DWORD start_time = GetTickCount();
			fa.startWork(videoList[i], fileReader.getNameFilePath(), fileReader.getIndexFilePath());
			fa.detectMovie();
			DWORD end_time = GetTickCount();
			std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
		}
#endif*/

	//	DWORD start_time = GetTickCount();
	//	DWORD start_timeT = GetTickCount();
	//	Mat img = imread("G://C++//Face//einfacedata//trainingdata1//FERET-209//3.png");
	//	FaceRecognization fa;
	//	IplImage ipl_img(img);
	//	fa.imageTransform(&ipl_img);
	///*	fa.initWork("G://C++//Face//mp4//index2.mp4");
	//	fa.detectMovie("V0001");*/
	//	DWORD end_timeT = GetTickCount();
	//	std::cout << "The run time is:" << (end_timeT - start_timeT) << "ms!" << std::endl;
		/*DWORD start_time = GetTickCount();
		vector<string>te;
		te.push_back("G://C++//Face//mp4//index1.mp4");
		te.push_back("G://C++//Face//mp4//index2.mp4");
		te.push_back("G://C++//Face//mp4//index3.mp4");
		te.push_back("G://C++//Face//mp4//index4.mp4");

		te.push_back("G://C++//Face//mp4//index.mp4");
		te.push_back("G://C++//Face//mp4//index5.mp4");
		te.push_back("G://C++//Face//mp4//index6.mp4");
		te.push_back("G://C++//Face//mp4//index7.mp4");

#pragma omp parallel sections
{
     #pragma omp section  
      {
		  for (int i = 0; i < te.size() / 2; i++){
			  DWORD start_time = GetTickCount();
			  FaceRecognization fa1;
			  fa1.initWork(te[i]);
			  fa1.detectMovie();
			  DWORD end_time = GetTickCount();
			  std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
		  }
      }
    #pragma omp section 
    {
		for (int i = te.size() / 2; i < te.size(); i++){
			DWORD start_time = GetTickCount();
			FaceRecognization fa1;
			fa1.initWork(te[i]);
			fa1.detectMovie();
			DWORD end_time = GetTickCount();
			std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
		}
    }
  }*/
		
			//#pragma omp parallel sections
			//	{
			//     #pragma omp section  
			//	{
			//		DWORD start_time = GetTickCount();
			//		FaceRecognization fa1;
			//		fa1.initWork("G://C++//Face//mp4//index1.mp4");
			//		fa1.detectMovie();
			//		DWORD end_time = GetTickCount();
			//		std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
			//	  }
			//  /*  #pragma omp section  
			//     {
			//		 DWORD start_time = GetTickCount();
			//		 FaceRecognization fa2;
			//		 fa2.initWork("G://C++//Face//mp4//index2.mp4");
			//		 fa2.detectMovie();
			//		 DWORD end_time = GetTickCount();
			//		 std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
			//	  }
			//  #pragma omp section  
			//	 {
			//		 DWORD start_time = GetTickCount();
			//		 FaceRecognization fa3;
			//		 fa3.initWork("G://C++//Face//mp4//index3.mp4");
			//		 fa3.detectMovie();
			//		 DWORD end_time = GetTickCount();
			//		 std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
			//	 }
			//  #pragma omp section  
			//	 {
			//		 DWORD start_time = GetTickCount();
			//		 FaceRecognization fa4;
			//		 fa4.initWork("G://C++//Face//mp4//index4.mp4");
			//		 fa4.detectMovie();
			//		 DWORD end_time = GetTickCount();
			//		 std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
			//	 }*/
			//	}

			/*	DWORD end_time = GetTickCount();
				std::cout << "The total run time is:" << (end_time - start_time) << "ms!" << std::endl;*/
//		FileReader fileReader;
//		/*
//		读取peoplelist.txt D:\pepoleimage\image1.jpg 人物ID
//		读取Videolist.txt D:\test\video1.mp4 360
//		处理输出文件夹
//		开始运行
//		*/
//		fileReader.readPeopleList(argv[1]);
//		fileReader.readVideoList(argv[2]);
//		cout << argv[1] << "  " << argv[2] << endl;
//
//		vector<string> videoList = fileReader.getVideoList();
//		for (int i = 0; i < videoList.size(); i++)
//		{
//			FaceRecognization fa;
//#ifdef DEBUG
//			cout << videoList[i] << endl;
//#endif
//			DWORD start_time = GetTickCount();
//			fa.startWork(videoList[i], fileReader.getNameFilePath(), fileReader.getIndexFilePath());
//			fa.detectMovie();
//			DWORD end_time = GetTickCount();
//			std::cout << "The run time is:" << (end_time - start_time) << "ms!" << std::endl;
//
//		}
//
//		printf("Output...\n");
//		printf("Over");
	}

	system("Pause");
	return 0;
}
