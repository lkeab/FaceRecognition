#include "stdafx.h"
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include "FileReader.h"
#include "FaceRecognization.h"

using namespace std;
int main(int argc, char **argv) {
	if (argc != 4) {
		printf("Face Recognition toolkit v 0.1a\n\n");
		printf("Parameters for testing:\n");
		printf("Face.exe pepolelist.txt Videolist.txt D:\Result\n");
	}
	else
	{
		FileReader fileReader;
		/*
		读取peoplelist.txt D:\pepoleimage\image1.jpg 人物ID
		读取Videolist.txt D:\test\video1.mp4 360
		处理输出文件夹
		开始运行
		*/
		fileReader.readPeopleList(argv[1]);
		fileReader.readVideoList(argv[2]);

		vector<string> videoList =  fileReader.getVideoList();
		for (int i = 0; i < videoList.size(); i++)
		{
			FaceRecognization fa;
#ifdef DEBUG
			cout << videoList[i] << endl;
#endif
			fa.startWork(videoList[i], fileReader.getNameFilePath(), fileReader.getIndexFilePath());
			fa.detectMovie();
		}
		
		printf("Output...\n");
		printf("Over");
	}

	system("Pause");
	return 0;
}
