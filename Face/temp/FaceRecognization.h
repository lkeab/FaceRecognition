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

class FaceRecognization{
	CvMemStorage* storage = 0;
	CvHaarClassifierCascade* cascade = 0;
	CvHaarClassifierCascade* cascade1 = 0;

	CvHaarClassifierCascade* nested_cascade = 0;
	int use_nested_cascade = 0;

	const char* cascade_name =
		"./data/haarcascade_frontalface_alt.xml";//别人已经训练好的人脸检测xml数据
	const char* nested_cascade_name =
		"./data/haarcascade_eye_tree_eyeglasses.xml";
	const char* cascade_name1 = "./data/haarcascade_profileface.xml";

	CvCapture* capture = 0;
	IplImage *frame, *frame_copy = 0;
	IplImage *image = 0;
	const char* scale_opt = "--scale="; // 分类器选项指示符号 
	int scale_opt_len = (int)strlen(scale_opt);
	const char* cascade_opt = "--cascade=";
	int cascade_opt_len = (int)strlen(cascade_opt);
	const char* nested_cascade_opt = "--nested-cascade";
	int nested_cascade_opt_len = (int)strlen(nested_cascade_opt);
	double scale = 4;
	int num_components = 9;
	double facethreshold = 9.0;

	int numFrames=0 ;
	int totalFrames =0;
	//opencv的FaceRecogizer目前有三个类实现了他，特征脸和fisherface方法最少训练图像为两张，而LBP可以单张图像训练
	//cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
	//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
	cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();//LBP的这个方法在单个人脸验证方面效果最好

	vector<Mat> images;//两个容器images,labels来存放图像数据和对应的标签
	vector<int> labels;
	vector<Container> con;
	bool repeat = false;
	std::map<int, string> names;
   
	string detectedName="";
	vector<bool>record;

	

public:
	const char* windowName="result";
	char *address = 0;

	void startWork(string movieFile, string nameFile, string indexFile);

	void readMovie();
	bool readMovieByNum( int num);
	void readMovieBySegment(int start, int end);
	void detectMovie();
	void initWork(char *address1);
	bool recog_and_draw(IplImage* img,int num);
	void refineSearch(int num);
	void cvText(IplImage* img, const char* text, int x, int y);
	Mat norm_0_255(cv::InputArray _src);

	//读取文件中的图像数据和类别，存入images和labels这两个容器
	void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator = ';');
	void readFile(const string &filename, std::map<int, string> &a, char separator = ';');
	bool read_img(vector<Mat> &images, vector<int> &labels);
	
	// 只是检测人脸，并将人脸圈出 
	void detect_and_draw(IplImage* img);
};