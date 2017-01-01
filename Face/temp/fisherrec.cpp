#include "stdafx.h"/*


#include "cv.h"
#include "highgui.h"

#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

static  Mat norm_0_255(cv::InputArray _src)
{
	Mat src = _src.getMat();
	Mat dst;

	switch(src.channels())
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

static void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator = ';')
{
	std::ifstream file(filename.c_str(), ifstream::in);
	if(!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel;
	while(getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);  //遇到分号就结束
		getline(liness, classlabel);     //继续从分号后面开始，遇到换行结束
		if(!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

int main(int argc, char *argv[])
{
	string output_folder;
	output_folder = string("F:/VS2010/faceRecgnition/einfacedata");

	//读取你的CSV文件路径
	string fn_csv = string("F:/VS2010/faceRecgnition/einfacedata/at.txt");

	//两个容器来存放图像数据和对应的标签
	vector<Mat> images;
	vector<int> labels;

	try
	{
		read_csv(fn_csv, images, labels);	
	}
	catch(cv::Exception &e)
	{
		cerr<<"Error opening file "<<fn_csv<<". Reason: "<<e.msg<<endl;
		exit(1);
	}

	//如果没有读到足够的图片，就退出
	if(images.size() <= 1)
	{
		string error_message = "This demo needs at least 2 images to work.";
		CV_Error(CV_StsError, error_message);
	}

	//得到第一张照片的高度，在下面对图像变形到他们原始大小时需要
	int height = images[0].rows;

	//移除最后一张图片，用于做测试
	Mat testSample = images[images.size() - 1];
	cv::imshow("testSample", testSample);
	int testLabel = labels[labels.size() - 1];

	images.pop_back();
	labels.pop_back();




	cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();

	//training
	model->train(images, labels);

	
	//下面对测试图像进行预测，predictedLabel 是预测标签结果
	int predictedLabel = model->predict(testSample);

	string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	cout<<result_message<<endl;


	//获取特征脸模型的特征值的例子，使用了getMat方法
	Mat eigenvalues = model->getMat("eigenvalues");
	//获取特征向量
	Mat W = model->getMat("eigenvectors");

	//得到训练图像的均值向量
	Mat mean = model->getMat("mean");

	imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));
	cv::imwrite(format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));

	//实现并保存特征脸
	for(int i=0; i <min(10, W.cols); i++)
	{
		string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
		cout<<msg<<endl;

		Mat ev = W.col(i).clone();

		//把他变成原始大小，为了把数据显示归一化到0-255.
		Mat grayscale = norm_0_255(ev.reshape(1,height));

		//使用伪彩色来显示结果
		Mat cgrayscale;
		cv::applyColorMap(grayscale, cgrayscale, COLORMAP_JET);

		imshow(format("eigenface_%d", i), cgrayscale);
		imwrite(format("%s/eigenface_%d.png", output_folder.c_str(), i), cgrayscale);
	}
	
	//在预测过程中，显示并保存重建后的图片
	for(int num_components = 10; num_components < 390; num_components += 15)
	{
		//从模型中的特征向量截取一部分
		Mat evs = Mat(W, Range::all(), Range(0, num_components));
		//投影
		Mat projection = cv::subspaceProject(evs, mean, images[0].reshape(1,1));
		//重构
		Mat reconstruction = cv::subspaceReconstruct(evs, mean, projection);

		reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));

		imshow(format("eigenface_reconstruction_%d", num_components),reconstruction);
		imwrite(format("%s/eigenface_reconstruction_%d.png", output_folder.c_str(),num_components), reconstruction);

	}
	
	cv::waitKey(0);
	return 0;
}
*/