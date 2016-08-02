#include "DataStructures.h"


using namespace cv;

void displayInfo()
{
	
	cout << "--------------------------------------------------------------------------" << endl
		<< "This program Segments the hand of a user and indicates how many fingers " << endl
		<< "or if  the user is performed a predefined gesture." << endl
		<< endl
		<< "Usage:" << endl
		<< "Keep a bright plain background as the recording environment." << endl
		<< "After lauching the application put your hand in the scene and toy around. " << endl
		<< "--------------------------------------------------------------------------" << endl
		<< endl;
}


bool detectRoundFigure(Mat Image, double mincontour, double roundLimit) {

	double contourSize, perimeter, roundness;
	double mincontourSize = mincontour;
	Mat grayImg, CannyImage;
	cvtColor(Image, grayImg, CV_BGR2GRAY);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat contourImage = Mat::zeros(grayImg.size(), CV_8UC3);
	Canny(grayImg, CannyImage, 100, 200, 3);
	//imshow("CannyEdges", CannyImage);

	findContours(CannyImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));
	vector<Point>contours_approx;

	for (unsigned int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_approx, arcLength(Mat(contours[i]), true) * 0.01, true);
		//if (hierarchy[i][3] >= 0) {   //has parent, inner (hole) contour of a closed edge (looks good)
			contourSize = contourArea(contours[i]);
			perimeter = arcLength(contours[i], true);
			roundness = FindRoundness(perimeter, contourSize);
			if (contourSize > mincontourSize && roundness>roundLimit) {
				return true;
			}


		//}
	}
	return false;
}


double FindRoundness(double area, double perimeter) {
	double Round = ((perimeter*perimeter) / area);
	return Round;
}