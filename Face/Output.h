#pragma
#include"ErrorOutput.h"
#include<sstream>

ErrorOutput eo("../error.txt");

void Error(string msg)
{
	eo.Print(msg);
}

//string to_string(int t)
//
//{
//	ostringstream oss;//创建一个流
//	oss << t;//把值传递如流中
//	return oss.str();//获取转换后的字符转并将其写入result
//}
//
//string to_string(double t)
//
//{
//	ostringstream oss;//创建一个流
//	oss << t;//把值传递如流中
//	return oss.str();//获取转换后的字符转并将其写入result
//}
//
//
//string to_string(float t)
//{
//	ostringstream oss;//创建一个流
//	oss << t;//把值传递如流中
//	return oss.str();//获取转换后的字符转并将其写入result
//}
//
