#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <opencv2\gpu\gpu.hpp>
#include <opencv2\core\internal.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <io.h>  
#include <map>
#include <direct.h> 

using namespace cv;
using namespace cv::gpu;

struct Container
{
	int num;
	string name;
};

extern cv::Ptr<cv::FaceRecognizer> model;
//extern CvHaarClassifierCascade* cascade ;
//extern CvHaarClassifierCascade* cascade1;

extern vector<Mat> images; //两个容器images,labels来存放图像数据和对应的标签
extern vector<int> labels;
extern std::map<int, string> names;//记录人与数值ID的对应关系

class FaceRecognization{

	CvMemStorage* storage = 0;
	CvCapture* capture = 0;

	double scale = 4;

	int numFrames = 0;
	int totalFrames = 0;

	vector<Container> con;//记录每人的出现次数以便选最大
	IplImage * frame_copy = 0;
	bool repeat = false;

	string detectedName = "";
	vector<bool>record;
	vector<double>confidenceRecord;

	CvHaarClassifierCascade* cascade;
	CvHaarClassifierCascade* cascade1;
	double maxSim = 1.0;
	string outputDir = ".";
	string outputFile = "武汉大学_人脸识别_第1组";


public:

	void setoutputDir(string dir) {
		outputDir = dir;
	}
	void setoutputFile(string file)
	{
		outputFile = file;
	}

	bool readMovieByNum(int num);

	void detectMovie(string movieName, string add);

	void detectMovie(string add);

	double generatePartSim(int begin, int end);

	void setMaxSim();

	bool recog_and_draw(IplImage* img, int num);

	void refineSearch(int num);

	void decideName();

	~FaceRecognization()
	{
		cvReleaseHaarClassifierCascade(&(this->cascade));
		cvReleaseHaarClassifierCascade(&(this->cascade1));
		cvReleaseMemStorage(&storage);
	}
};