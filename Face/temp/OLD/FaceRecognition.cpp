#include "stdafx.h"
#include "FaceRecognition.h"
#include "FileWriter.h"
#include <algorithm>
#include <Windows.h>

#ifdef _EiC
#define WIN32
#endif

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

void FaceRecognization::imageTransform(IplImage *img){

	if (!cascade || !cascade1){
		cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //加载分类器 
		cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //加载分类器 
	}
	
	IplImage *gray, *small_img;
	int i, j;
	storage = cvCreateMemStorage(0);
	gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	small_img = cvCreateImage(cvSize(cvRound(img->width / 2),cvRound(img->height / 2)), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY); // 彩色RGB图像转为灰度图像 
	cvResize(gray, small_img, CV_INTER_LINEAR);
	cvEqualizeHist(small_img, small_img); // 直方图均衡化 

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

		if (!faces->total){
			cout << "未检测到人脸" << endl;
			return;
		}

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

			cvShowImage("test", resizeRes);

			//转换后生成的80*80的目标图片
			imwrite("./einfacedata/trainingdata/ouput6.jpg",(Mat)resizeRes);			
		}
		
	}
	cvClearMemStorage(storage);
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);
}

bool FaceRecognization::recog_and_draw(IplImage* img, int num)
{
	IplImage *gray, *small_img;
	int i, j;
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
			int predictedLabel = -1;

			//在这里对人脸进行判别
			double predicted_confidence = 10.0;
			model->predict((Mat)resizeRes, predictedLabel, predicted_confidence);

			//std::cout << "predict:" << predictedLabel << "\nconfidence:" << predicted_confidence << std::endl;
			if ((predictedLabel == 201) || (predictedLabel == 207)){
				std::cout << "predict1:" << names[predictedLabel] << "\nconfidence:" << predicted_confidence << std::endl;
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

Mat FaceRecognization::norm_0_255(cv::InputArray _src)
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

//读取文件中的图像数据和类别，存入images和labels这两个容器
void FaceRecognization::read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel, name;
	while (getline(file, line))
	{
		std::stringstream liness(line);
		getline(liness, path, separator);  //遇到分号就结束
		getline(liness, classlabel);     //继续从分号后面开始，遇到换行结束
		//		getline(liness, name);
		if (!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

//读取到name.txt中数值和人的对应关系，并存入键值对names中
void FaceRecognization::readFile(const string &filename, std::map<int, string> &a, char separator){
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel;
	while (getline(file, line))
	{
		std::stringstream liness(line);
		getline(liness, path, separator);  //遇到分号就结束
		getline(liness, classlabel);     //继续从分号后面开始，遇到换行结束

		if (!path.empty() && !classlabel.empty())
		{
			std::stringstream ss;
			ss << path;
			int i;
			ss >> i;
			a.insert(std::pair<int, string>(i, classlabel));
		}
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
}

//参数：MP4名，MP4地址
void FaceRecognization::detectMovie(string movieName,string address){

	char* c;
	const int len = address.length();
	c = new char[len + 1];
	strcpy(c, address.c_str());

	storage = cvCreateMemStorage(0); // 创建内存存储器   
	capture = cvCaptureFromFile(c);

	if (capture)
	{
		numFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		totalFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

		vector<bool>record1(totalFrames, 0);
		vector<double>confidenceRecord1(totalFrames, 0);

		record = record1;
		confidenceRecord = confidenceRecord1;

		std::cout << "帧率：" << numFrames << std::endl;
		std::cout << "总帧数：" << totalFrames << std::endl;

		int sec = totalFrames / numFrames;

		for (int i = 0; i < sec/2; i++){
			readMovieByNum(i*numFrames*2);
		}

		repeat = true;

		if (!con.size()){
			std::cout << "该视频不存在目标任务" << std::endl;
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
				std::cout << "第" << i<< "帧存在人脸" << std::endl;
				totalSum += confidenceRecord[i];
				m++;
			}
		}

		if (record[record.size() - 1]){
			s.push_back(record.size() - 1);
		}

		for (int i = 0; i < s.size(); i=i+2){
			OutputStruct a;
			a.start = s[i];
			a.end = s[i+1];
			a.similarity = generatePartSim(a.start, a.end);
			v.push_back(a);
		}

		if (m < numFrames){
			std::cout << "该视频不存在目标任务" << std::endl;
			return;
		}
		else
		{
			std::cout << "目标任务：" << detectedName << std::endl;
			std::cout << "总共有" << m << "帧存在人脸" << std::endl;
		}

		double totalSim = totalSum / (m*maxSim);

		FileWriter fw("G://Cuda", "武汉大学_人脸识别_第1组");
	/*	vector<OutputStruct>v;
		OutputStruct a;
		a.start = 5;
		a.end = 6;
		a.similarity = 0.7;
		v.push_back(a);
		OutputStruct a1;
		a1.start = 5;
		a1.end = 6;
		a1.similarity = 0.7;*/
		/*v.push_back(a1);*/
		fw.Write(movieName, detectedName, totalSim, v.size(), v);
		//fw.Write(movieName, "柯磊",totalSim,v.size(), v);

		cvReleaseImage(&frame_copy);
		cvReleaseCapture(&capture);
	}
}

double FaceRecognization::generatePartSim(int begin ,int end){
	
	double s = 0;
	for (int i = begin; i <= end; i++){
		s += confidenceRecord[i] / maxSim;
	}
	double result = s / (end - begin + 1);
	return result;
}

void FaceRecognization::setMaxSim(){
	for (int i = 0; i < confidenceRecord.size(); i++){
		if (confidenceRecord[i]>maxSim){
			maxSim = confidenceRecord[i];
		}
	}
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
	if (readMovieByNum(num - numFrames )){
		for (int i = num - numFrames ; i < num; i++){
			record[i] = true;
			if (i != num - numFrames){
				confidenceRecord[i] = (confidenceRecord[num - numFrames]+confidenceRecord[num])/2.0;
			}
		}
	}
	else if (readMovieByNum(num - numFrames / 2)){
		for (int i = num - numFrames / 2; i < num; i++){
			record[i] = true;
			if (i != num - numFrames/2){
				confidenceRecord[i] = (confidenceRecord[num - numFrames/2] + confidenceRecord[num]) / 2.0;
			}
		}
		int m = num - numFrames / 2;

		while (readMovieByNum(m)){
			record[m] = true;
			m--;
			confidenceRecord[m] = confidenceRecord[num-numFrames/2];
			if (m <= num - numFrames ){
				break;
			}
		}
	}
	else{
		int n = num - numFrames / 4;
		while (readMovieByNum(n)){
			record[n] = true;
			n--;
			confidenceRecord[n] = confidenceRecord[num - numFrames/4];
			if (n <= num - numFrames / 2){
				break;
			}
		}
	}

	if (readMovieByNum(num + numFrames )){
		for (int i = num + 1; i < num + numFrames ; i++){
			record[i] = true;
            confidenceRecord[i] = (confidenceRecord[num+numFrames] + confidenceRecord[num]) / 2.0;
		}
	}
	else if (readMovieByNum(num + numFrames / 2)){
		for (int i = num+1; i < num + numFrames / 2; i++){
			record[i] = true;
            confidenceRecord[i] = (confidenceRecord[num+numFrames / 2] + confidenceRecord[num]) / 2.0;
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
		int n = num+numFrames/4;
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

void FaceRecognization::startWork(string nameFile, string indexFile)
{

	//char* c;
	//const int len = movieFile.length();
	//c = new char[len + 1];
	//strcpy(c, movieFile.c_str());
	//address = c;
	//readFile(nameFile, names);

	cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //加载分类器 
	cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //加载分类器 
	//nested_cascade = (CvHaarClassifierCascade*)cvLoad(nested_cascade_name, 0, 0, 0);

	if (!cascade || !cascade1){
		fprintf(stderr, "ERROR: Could not load classifier cascade\n");
		getchar();
		return;
	}

	//model = cv::createLBPHFaceRecognizer();
	//model->set("threshold", 2000.0);
	string output_folder;
	output_folder = string("./einfacedata");

	//读取你的CSV文件路径
	string fn_csv = indexFile;
	try {
		//通过./einfacedata/at.txt这个文件读取里面的训练图像和类别标签
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception &e) {
		std::cerr << "Error opening file " << fn_csv << ". Reason: " << e.msg << std::endl;
		exit(1);
	}
	std::cout << images.size() << ":" << labels.size() << std::endl;

	if (images.size() <= 2)
	{
		string error_message = "This demo needs at least 2 images to work.";
		CV_Error(CV_StsError, error_message);
	}

	//	model->train(images, labels);

}

void FaceRecognization::initWorkLoading(){

	string fn_csv1 = string("../einfacedata/name.txt");
	readFile(fn_csv1, names);

	if (!cascade || !cascade1){
		cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //加载分类器 
		cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //加载分类器 
	}

	if (!cascade || !cascade1)
	{
		fprintf(stderr, "ERROR: Could not load classifier cascade\n");
		getchar();
		return;
	}

	model->load("MyEigenFaceModel1.xml");
}

void FaceRecognization::initWorkTraining(){

	string fn_csv1 = string("../einfacedata/name.txt");
	readFile(fn_csv1, names);

	if (!cascade || !cascade1){
		cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //加载分类器 
		cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //加载分类器 
	}
	//nested_cascade = (CvHaarClassifierCascade*)cvLoad(nested_cascade_name, 0, 0, 0);

	if (!cascade || !cascade1)
	{
		fprintf(stderr, "ERROR: Could not load classifier cascade\n");
		getchar();
		return;
	}


	//读取你的CSV文件路径
	string fn_csv = string("at.txt");
	try
	{
		//通过./einfacedata/at.txt这个文件读取里面的训练图像和类别标签
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception &e)
	{
		std::cerr << "Error opening file " << fn_csv << ". Reason: " << e.msg << std::endl;
		exit(1);
	}

	//std::cout << images.size() << ":" << labels.size() << std::endl;
	//如果没有读到足够的图片，就退出
	if (images.size() <= 2)
	{
		string error_message = "This demo needs at least 2 images to work.";
		CV_Error(CV_StsError, error_message);
	}

	//模型训练并保存
	model->train(images, labels);
	model->save("MyEigenFaceModel1.xml");
}