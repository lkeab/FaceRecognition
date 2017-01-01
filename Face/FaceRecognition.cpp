#include "stdafx.h"
#include "FaceRecognition.h"
#include "FileWriter.h"
#include <algorithm>
#include <Windows.h>

#ifdef _EiC
#define WIN32
#endif

extern const char* cascade_name;
extern const char* cascade_name1;

extern void Error(string msg);
//extern string to_string(int t);
//extern string to_string(double t);
//extern string to_string(float t);

template<class T>
string to_string(T& t)
{
	ostringstream oss;//创建一个流
	oss << t;//把值传递如流中
	return oss.str();//获取转换后的字符转并将其写入result
}

static CvScalar colors[] =
{
	{ { 0, 0, 255 } },
	{ { 0, 128, 255 } },
	{ { 0, 255, 255 } },
	{ { 0, 255, 0 } },
	{ { 255, 128, 0 } },
	{ { 255, 255, 0 } },
	{ { 255, 0, 0 } },
	{ { 255, 0, 255 } }
};

Mat norm_0_255(cv::InputArray _src)
{
	Mat src = _src.getMat();
	Mat dst;

	switch (src.channels())
	{
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

bool FaceRecognization::recog_and_draw(IplImage* img, int num)
{
	IplImage *gray, *small_img;
	int i;
	gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	small_img = cvCreateImage(cvSize(cvRound(img->width / scale),
		cvRound(img->height / scale)), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY); // 彩色RGB图像转为灰度图像 
	cvResize(gray, small_img, CV_INTER_LINEAR);
	cvEqualizeHist(small_img, small_img); // 直方图均衡化 

	int cont = 0;
	if (cascade&&cascade1)
	{
		CvSeq* faces = cvHaarDetectObjects(small_img, cascade, storage,
			1.1, 3,
			CV_HAAR_DO_CANNY_PRUNING
			);

		if (!(faces->total)){
			faces = cvHaarDetectObjects(small_img, cascade1, storage,
				1.1, 3,
				CV_HAAR_DO_CANNY_PRUNING
				);
		}
		cont = faces->total;
		for (i = 0; i < (faces ? faces->total : 0); i++)
		{
			CvRect* r = (CvRect*)cvGetSeqElem(faces, i); // 将faces数据从CvSeq转为CvRect 
			CvMat small_img_roi;

			cvGetSubRect(small_img, &small_img_roi, *r);

			IplImage* imgd = cvCreateImage(cvGetSize(&small_img_roi), 8, 1);
			cvGetImage(&small_img_roi, imgd);

			IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width = (int)(80);
			dst_cvsize.height = (int)(80);

			resizeRes = cvCreateImage(dst_cvsize, imgd->depth, imgd->nChannels);
			//检测到的区域可能不是80x80大小，所以需要插值处理到统一大小，图像的大小可以自己指定的
			cvResize(imgd, resizeRes, CV_INTER_NN);
			cvReleaseImage(&imgd);
			int predictedLabel = -1;

			//在这里对人脸进行判别
			double predicted_confidence = 10.0;
			model->predict((Mat)resizeRes, predictedLabel, predicted_confidence);
			cvReleaseImage(&resizeRes);
			//std::cout << "predict:" << predictedLabel << "\nconfidence:" << predicted_confidence << std::endl;
			if (!(names.find(predictedLabel) == names.end())){
#ifdef DEBUG	
				cout << "predict1:" << names[predictedLabel] << "\nconfidence:" << predicted_confidence << endl;
				Error("predict1:" + names[predictedLabel] + "\nconfidence:" + to_string<double>(predicted_confidence));
#endif
			}
			else{
				return false;
			}

			if (!repeat){
				Container a;
				a.name = names[predictedLabel];
				a.num = num;
				con.push_back(a);
				confidenceRecord[num] = predicted_confidence;
				record[num] = true;
			}
		}
		
	}
	//cvShowImage(windowName, img);
	cvClearMemStorage(storage);
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);

	if (cont == 0){
		return false;
	}
	else{
		return true;
	}
}

bool FaceRecognization::readMovieByNum(int num){
	//capture = cvCaptureFromFile(address);  //读取视频文件

	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, num);
	//cvNamedWindow("res", 1);
	int count = num;
	IplImage* pFrame = 0;
	while (cvGrabFrame(capture) && count <= num)
	{
		pFrame = cvRetrieveFrame(capture);// 获取当前帧
		count++;
	}

	if (!pFrame){
		return false;
	}

	if (pFrame->width > 1500){
		scale = 8;
	}
	else if (pFrame->width > 1000){
		scale = 6;
	}

	//此处需要读视频图像的分辨率
	if (!frame_copy)
		frame_copy = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, pFrame->nChannels);

	if (pFrame->origin == IPL_ORIGIN_TL)
		cvCopy(pFrame, frame_copy, 0);
	else
		cvFlip(pFrame, frame_copy, 0);

	bool captu = recog_and_draw(frame_copy, num);

	cvReleaseImage(&frame_copy);

	return captu;
}

//参数：MP4名，MP4地址
void FaceRecognization::detectMovie(string movieName, string address){

	char* c;
	const int len = (int)address.length();
	c = new char[len + 1];
	strcpy(c, address.c_str());

	storage = cvCreateMemStorage(0); // 创建内存存储器   
	capture = cvCaptureFromFile(c);
	delete c;

	if (capture)
	{
		numFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		totalFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

		vector<bool>record1(totalFrames, 0);
		vector<double>confidenceRecord1(totalFrames, 0);

		record = record1;
		confidenceRecord = confidenceRecord1;

#ifdef DEBUG	
		std::cout << "帧率：" << numFrames << std::endl;
		std::cout << "总帧数：" << totalFrames << std::endl;
		Error("帧率：" + to_string<int>(numFrames));
		Error("总帧数：" + to_string<int>(totalFrames));
#endif
		int sec = totalFrames / numFrames;

		for (int i = 0; i < sec / 2; i++){
			readMovieByNum(i*numFrames * 2);
		}

		repeat = true;

		if (!con. size()){
#ifdef DEBUG
			std::cout << "该视频不存在目标任务" << std::endl;
			Error("该视频不存在目标任务");
#endif
			return;
		}

		decideName();

		for (int i = 0; i < con.size(); i++){
			refineSearch(con[i].num);
		}

		setMaxSim();

		int m = 0;
		double totalSum = 0;
		vector<OutputStruct>v;
		vector<int>s;
		if (record[0]){
			s.push_back(0);
		}

		for (int i = 0; i < record.size(); i++){
			if (i != 0){
				if (record[i] && !record[i - 1]){
					s.push_back(i);
				}
			}
			if (i != record.size() - 1){
				if (record[i] && !record[i + 1]){
					s.push_back(i);
				}
			}

			if (record[i]){
#ifdef DEBUG
				std::cout << "第" << i << "帧存在人脸" << std::endl;
				Error("第" + to_string<int>(i) + "帧存在人脸");
#endif
				totalSum += confidenceRecord[i];
				m++;
			}
		}

		if (m < numFrames * 3) {
#ifdef DEBUG
			std::cout << "该视频不存在目标任务" << std::endl;
			Error("该视频不存在目标任务");
#endif
			return;
		}
		else
		{
#ifdef DEBUG
			std::cout << "目标任务：" << detectedName << std::endl;
			std::cout << "总共有" << m << "帧存在人脸" << std::endl;
			Error("目标任务：" + detectedName);
			Error("总共有" + to_string<int>(m) + "帧存在人脸");
#endif
		}

		if (record[record.size() - 1]){
			s.push_back(record.size() - 1);
		}

		for (int i = 0; i < s.size(); i = i + 2){
			OutputStruct a;
			a.start = s[i];
			a.end = s[i + 1];
			a.similarity = generatePartSim(a.start, a.end);
			v.push_back(a);
		}

		double totalSim = 1 - totalSum / (m*maxSim);

		FileWriter fw(outputDir, outputFile);
		fw.Write(movieName, detectedName, totalSim, v.size(), v);

		//cvReleaseImage(&frame_copy);
		cvReleaseCapture(&capture);
	}

}

void FaceRecognization::detectMovie(string add){

	if (!cascade || !cascade1){
		cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //加载分类器 
		cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //加载分类器 
	}

	if (!cascade || !cascade1)
	{
#ifdef DEBUG
		fprintf(stderr, "ERROR: Could not load classifier cascade\n");
		Error("ERROR: Could not load classifier cascade\n");
#endif
		getchar();
		return;
	}

	char* path_buffer;
	const int len = add.length();
	path_buffer = new char[len + 1];
	strcpy(path_buffer, add.c_str());
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(path_buffer, drive, dir, fname, ext);
	strcat(fname, ext);
	detectMovie(fname, add);
	delete path_buffer;
}

double FaceRecognization::generatePartSim(int begin, int end){

	double s = 0;
	for (int i = begin; i <= end; i++){
		s +=1- confidenceRecord[i] / maxSim;
	}
	double result = s / (end - begin + 1);
	return result;
}

void FaceRecognization::setMaxSim(){
	/*for (int i = 0; i < confidenceRecord.size(); i++){
		if (confidenceRecord[i]>maxSim){
			maxSim = confidenceRecord[i];
		}
	}*/
	maxSim = 7500;
}

void FaceRecognization::decideName(){
	std::map <string, int> appCon;
	appCon.insert(std::pair<string, int>(con[0].name, 1));
	for (int i = 1; i < con.size(); i++){
		std::map<string, int >::iterator l_it;
		l_it = appCon.find(con[i].name);
		if (l_it == appCon.end()){
			appCon.insert(std::pair<string, int>(con[0].name, 1));
		}
		else{
			appCon[con[i].name]++;
		}
	}

	int max = 0;
	std::map<string, int >::iterator my_Itr;
	string name = "";

	for (my_Itr = appCon.begin(); my_Itr != appCon.end(); ++my_Itr) {
		if (my_Itr->second > max){
			max = my_Itr->second;
			detectedName = my_Itr->first;
		}
	}
}

void FaceRecognization::refineSearch(int num){

	if (num < numFrames)
	{
		if (readMovieByNum(num + numFrames)){
			for (int i = num + 1; i < num + numFrames; i++){
				record[i] = true;
				confidenceRecord[i] = (confidenceRecord[num + numFrames] + confidenceRecord[num]) / 2.0;
			}
		}
		else if (readMovieByNum(num + numFrames / 2)){
			for (int i = num + 1; i < num + numFrames / 2; i++){
				record[i] = true;
				confidenceRecord[i] = (confidenceRecord[num + numFrames / 2] + confidenceRecord[num]) / 2.0;
			}
			int m = num + numFrames / 2;

			while (readMovieByNum(m)){
				record[m] = true;
				m++;
				confidenceRecord[m] = confidenceRecord[num + numFrames / 2];
				if (m >= num + numFrames){
					break;
				}
			}
		}
		else{
			int n = num + numFrames / 4;
			while (readMovieByNum(n)){
				record[n] = true;
				n++;
				confidenceRecord[n] = confidenceRecord[num + numFrames / 4];
				if (n >= num + numFrames / 2){
					break;
				}
			}
		}

		for (int i = num; i < num + numFrames / 4; i++){
			record[i] = true;
			confidenceRecord[i] = confidenceRecord[num];
		}
		return;
	}



	if (readMovieByNum(num - numFrames)){
		for (int i = num - numFrames; i < num; i++){
			record[i] = true;
			if (i != num - numFrames){
				confidenceRecord[i] = (confidenceRecord[num - numFrames] + confidenceRecord[num]) / 2.0;
			}
		}
	}
	else if (readMovieByNum(num - numFrames / 2)){
		for (int i = num - numFrames / 2; i < num; i++){
			record[i] = true;
			if (i != num - numFrames / 2){
				confidenceRecord[i] = (confidenceRecord[num - numFrames / 2] + confidenceRecord[num]) / 2.0;
			}
		}
		int m = num - numFrames / 2;

		while (readMovieByNum(m)){
			record[m] = true;
			m--;
			confidenceRecord[m] = confidenceRecord[num - numFrames / 2];
			if (m <= num - numFrames){
				break;
			}
		}
	}
	else{
		int n = num - numFrames / 4;
		while (readMovieByNum(n)){
			record[n] = true;
			n--;
			confidenceRecord[n] = confidenceRecord[num - numFrames / 4];
			if (n <= num - numFrames / 2){
				break;
			}
		}
	}

	if (readMovieByNum(num + numFrames)){
		for (int i = num + 1; i < num + numFrames; i++){
			record[i] = true;
			confidenceRecord[i] = (confidenceRecord[num + numFrames] + confidenceRecord[num]) / 2.0;
		}
	}
	else if (readMovieByNum(num + numFrames / 2)){
		for (int i = num + 1; i < num + numFrames / 2; i++){
			record[i] = true;
			confidenceRecord[i] = (confidenceRecord[num + numFrames / 2] + confidenceRecord[num]) / 2.0;
		}
		int m = num + numFrames / 2;

		while (readMovieByNum(m)){
			record[m] = true;
			m++;
			confidenceRecord[m] = confidenceRecord[num + numFrames / 2];
			if (m >= num + numFrames){
				break;
			}
		}
	}
	else{
		int n = num + numFrames / 4;
		while (readMovieByNum(n)){
			record[n] = true;
			n++;
			confidenceRecord[n] = confidenceRecord[num + numFrames / 4];
			if (n >= num + numFrames / 2){
				break;
			}
		}
	}

	for (int i = num - numFrames / 4; i < num + numFrames / 4; i++){
		record[i] = true;
		confidenceRecord[i] = confidenceRecord[num];
	}
}


