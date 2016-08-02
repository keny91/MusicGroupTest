#pragma once

#include <.\opencv2\opencv.hpp>
#include <.\opencv\highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include<opencv2/video.hpp>
#include "opencv2/videoio.hpp"
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/background_segm.hpp>


#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

namespace cv
{
	//using std::vector; //solves the issue with vectors
	using namespace std;
}


cv::Mat m_frame, cap_image, bg;
cv::Mat cap_image_hsv;
int m_number_of_fingers;
cv::vector< cv::vector< cv::Point> >hull;
cv::vector<cv::Point> fingers;
cv::vector< cv::Vec4i> defects;





void displayInfo();
bool detectRoundFigure(Mat Image, double mincontour, double roundLimit);
double FindRoundness(double a, double p);