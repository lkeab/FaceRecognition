#include "stdafx.h"
#include "Train.h"
//#include "Output.h"
using namespace std;
using namespace cv;

cv::Ptr<cv::FaceRecognizer> model;

std::vector<cv::Mat> images; //两个容器images,labels来存放图像数据和对应的标签
std::vector<int> labels;
std::map<int, string> names;//记录人与数值ID的对应关系

const char* cascade_name =
"./Data/Model/haarcascade_frontalface_alt.xml";//别人已经训练好的人脸检测xml数据
const char* cascade_name1 = "./Data/Model/haarcascade_profileface.xml";
extern string OutPutModelFile = "./Data/Temp/testTrain.xml";
extern string OriginalNameFile = "./Data/Train/name.txt";
extern string OriginalIndexFile = "./Data/Train/index.txt";

string tempNameFile = "./Data/Temp/name.txt";
string tempIndexFile = "./Data/Temp/index.txt";

extern void Error(string msg);

template<class T>
string to_string(T t)
{
	ostringstream oss;//创建一个流
	oss << t;//把值传递如流中
	return oss.str();//获取转换后的字符转并将其写入result

}

void setModelPath(string modelPath)
{
	OutPutModelFile = modelPath;
}

void load(string namePath)
{
	if (!model)
		model = cv::createEigenFaceRecognizer();
	readFile(namePath, names);
	model->load(OutPutModelFile);
}
void load()
{
	if (!model)
		model = cv::createEigenFaceRecognizer();
	initWorkLoading();
}
void train()
{
	if (!model)
		model = cv::createEigenFaceRecognizer();
	initWorkTraining(tempNameFile, tempIndexFile);
}

//读取到name.txt中数值和人的对应关系，并存入键值对names中
void readFile(const string &filename, std::map<int, string> &a, char separator){
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

//读取文件中的图像数据和类别，存入images和labels这两个容器
void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator){
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
void initWorkLoading(){
	readFile(OriginalNameFile, names);
	model->load(OutPutModelFile);
}


void initWorkTraining(string nametxtPath, string attxtPath){
	//string fn_csv1 = string("./einfacedata/name.txt");
	string fn_csv1 = nametxtPath;
	readFile(fn_csv1, names);

	//读取你的CSV文件路径
	string fn_csv = attxtPath;
	try
	{
		//通过./einfacedata/at.txt这个文件读取里面的训练图像和类别标签
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception &e)
	{
#ifdef DEBUG
		std::cerr << "Error opening file " << fn_csv << ". Reason: " << e.msg << std::endl;
		Error("Error opening file " + fn_csv + ". Reason: " + e.msg);
#endif
		exit(1);
	}
#ifdef DEBUG
	std::cout << images.size() << ":" << labels.size() << std::endl;
	Error(to_string<int>((int)images.size()) + ":" + to_string<int>((int)labels.size()));
#endif
	//如果没有读到足够的图片，就退出
	if (images.size() <= 2)
	{
		string error_message = "This demo needs at least 2 images to work.";
		CV_Error(CV_StsError, error_message);
	}

	//模型训练并保存
	model->train(images, labels);
	model->save(OutPutModelFile);
	//model->save("MyEigenFaceModel1.xml");
    //model->load(OutPutModelFile);
}

void imageTransform(IplImage *img, string outPath){
	CvMemStorage* storage = 0;
	CvHaarClassifierCascade* cascade = 0;
	CvHaarClassifierCascade* cascade1 = 0;

	if (!cascade || !cascade1){
		cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //加载分类器 
		cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //加载分类器 
	}

	IplImage *gray, *small_img;
	int i;
	storage = cvCreateMemStorage(0);
	gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	small_img = cvCreateImage(cvSize(cvRound(img->width / 2), cvRound(img->height / 2)), 8, 1);
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
#ifdef DEBUG
			Error("未检测到人脸");
#endif
			CvSize dst_cvsize;
			dst_cvsize.width = (int)(80);
			dst_cvsize.height = (int)(80);
			IplImage *resizeRes;
			resizeRes = cvCreateImage(dst_cvsize, small_img->depth, small_img->nChannels);
			cvResize(small_img, resizeRes, CV_INTER_NN);
			imwrite(outPath, (Mat)resizeRes);
		}
		else
		{
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

				//转换后生成的80*80的目标图片
				imwrite(outPath, (Mat)resizeRes);
			}
		}
	}
	cvClearMemStorage(storage);
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);
}

