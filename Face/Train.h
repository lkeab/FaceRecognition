#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cv.h>
#include <highgui.h>
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <opencv2\gpu\gpu.hpp>
#include <opencv2\core\internal.hpp>

using namespace cv;

extern string OutPutModelFile;
extern string OriginalNameFile;
extern string OriginalIndexFile;

void setModelPath(string modelPath);
void load(string namePath);
void load();
void train();

//读取到name.txt中数值和人的对应关系，并存入键值对names中
void readFile(const string &filename, std::map<int, string> &a, char separator = ';');

//读取文件中的图像数据和类别，存入images和labels这两个容器
void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator = ';');

void initWorkLoading();
void initWorkTraining(string nametxtPath, string attxtPath);

void imageTransform(IplImage *img, string outPath);