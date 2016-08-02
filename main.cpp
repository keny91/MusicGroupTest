
#include "DataStructures.h"



using namespace cv;



int main() {

	/*
	Mat image;         
	VideoCapture cap;         
	cap.open(0);
	namedWindow("window", 1);          
	while (1) {
		cap >> image;          
		imshow("window", image);         
		waitKey(33);          //delay 33ms
	}

	*/


	///////////////////////////////////////////
	//print help information
	displayInfo();

	int iLowH = 0;
	int iHighH = 255;

	int iLowS = 119;  
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;
	int TH = 100;


	VideoCapture cap;
	Mat ch1,ch2,ch3;
	Mat theFilteredImage;
	Mat edges;
	Mat dst(bg.rows, bg.cols, CV_8UC1, Scalar::all(0));
	vector<cv::Point> max_contour, max_convex;
	//BackgroundSubtractorMOG2 MOG;

	// Break if could not open reading stream
	if (!cap.open(0)) {

		cerr << "Can´t open Default Video stream" << endl;
		cerr << "Exiting..." << endl;
		return EXIT_FAILURE;
	}

	cap >> bg;
	cap_image_hsv = Mat::zeros(cap_image.size(), CV_8UC3);
	//fgMaskMOG2 = Mat::zeros(cap_image.size(), CV_8UC3);


	/*
	Parameters:	
history – Length of the history.
varThreshold – Threshold on the squared Mahalanobis distance between the pixel and the model to decide whether a pixel is well described by the background model. This parameter does not affect the background update.
detectShadows – If true, the algorithm will detect shadows and mark them. It decreases the speed a bit, so if you do not need this feature, set the parameter to false.
*/
	// BG sustractors
	cv::Mat fgMaskMOG2;
	cv::BackgroundSubtractorMOG2* pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2(20, 16, true); //MOG2 approach

	//pMOG2 = new BackgroundSubtractorMOG2();

	//Do until interrupt
	//while ((char)keyboard != 'q' && (char)keyboard != 27)
	while (true) {
		//cap >> cap_image;ç

		//Break Case we are unable to read next frame
		if (!cap.read(cap_image)) {
			cerr << "Unable to read next frame." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);		
		}

		/*
		vector<Mat> channels(3);
		split(cap_image, channels);
		ch1 = channels[0];
		ch2 = channels[1];
		ch3 = channels[2];
		*/

		//update background mode
		if(!cap_image.empty()){
			//pMOG2->apply(cap_image, fgMaskMOG2);
			threshold(cap_image, edges, TH, 255, THRESH_BINARY);
			// add white border around the image
			//copyMakeBorder(cap_image, cap_image, 10, 10, 10, 10, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255)); 

			//flip horizontal
			flip(cap_image, cap_image, 1);
			//change BRG to YCC to improve skin detection
			cvtColor(cap_image, cap_image_hsv, CV_BGR2YCrCb);
			cvtColor(cap_image, cap_image_hsv, COLOR_BGR2HSV);

			inRange(cap_image_hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), theFilteredImage);

			//morphological opening (removes small objects from the foreground)
			erode(theFilteredImage, theFilteredImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(theFilteredImage, theFilteredImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//morphological closing (removes small holes from the foreground)
			dilate(theFilteredImage, theFilteredImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(theFilteredImage, theFilteredImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			// Keep the largest object
			vector<vector<Point>> contours; // Vector for storing contour
			vector<Vec4i> hierarchy;
			int largest_area = 0;
			int largest_contour_index = 0;
			Rect bounding_rect;
			//dst(theFilteredImage.rows, theFilteredImage.cols, CV_8UC1, Scalar::all(0));
			findContours(theFilteredImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

			for (int i = 0; i< contours.size(); i++) // iterate through each contour. 
			{
				double a = contourArea(contours[i], false);  //  Find the area of contour
				if (a>largest_area) {
					largest_area = a;
					largest_contour_index = i;                //Store the index of largest contour
															  //bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
				}

			}

			Scalar color(255, 255, 255);
			drawContours(dst, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy); // Draw the largest contour using previously stored index.

																								//rectangle(src, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);
			vector<Point> WhitePixels;
			findNonZero(dst, WhitePixels);
			//imshow("src", src);
			//cout << "Cloud all white pixels: " << WhitePixels.size() << endl;
			if (WhitePixels.size() < 500) {
				cout << "Hand recognized. Size =" << WhitePixels.size() << endl;
			}


			cv::convexHull(cv::Mat(contours[largest_contour_index]), hull[largest_contour_index], false, false);

			//find moments of contours
			std::vector<cv::Moments> mu(contours.size());
			for (int i = 0; i < contours.size(); i++)
			{
				mu[i] = moments(contours[i], false);
			}

			//find centre of moment
			std::vector<cv::Point2f> mc(contours.size());
			for (int i = 0; i < contours.size(); i++)
			{
				mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
			}

			//draw the contour, the hull and the centre
			cv::drawContours(cap_image, contours, largest_contour_index, cv::Scalar(255, 0, 50), 2, 8, hierarchy, 0, cv::Point());
			cv::drawContours(cap_image, hull, largest_contour_index, cv::Scalar(255, 0, 50), 2, 8, hierarchy, 0, cv::Point());
			circle(cap_image, mc[largest_contour_index], 4, cv::Scalar(30, 55, 190), -1, 8, 0);


			max_contour = contours[largest_contour_index];
			max_convex = hull[largest_contour_index];
			std::vector<cv::Point> fingers;
			cv::Point centreMass = mc[largest_contour_index];
			int k = 0;

			//Detect individual fingerss
			for (int i = 0; i<max_contour.size(); i++) {
				for (int j = 0; j<max_convex.size(); j++) {
					if (cv::norm(cv::Mat(max_contour[i]), cv::Mat(max_convex[j])) == 0) {

						fingers.push_back(max_contour[i]);
						k++;

					}
				}
			}


			double distance = 0;
			double centre_distance = 0;
			m_number_of_fingers = 0;

			//refine fingertip selection
			for (int i = 1; i<fingers.size(); i++) {

				distance = cv::norm(cv::Mat(fingers[i - 1]), cv::Mat(fingers[i]));
				centre_distance = cv::norm(cv::Mat(fingers[i]), cv::Mat(centreMass));
				if (distance>20 && fingers[i].y <  mc[largest_contour_index].y &&centre_distance >100) {
					m_number_of_fingers++;
					cv::circle(cap_image, fingers[i], 4, cv::Scalar(255, 0, 255), -1, 8, 0);
				}

			}
			if (m_number_of_fingers == 0) {
				string labelGesture = "Gesture: Fist";
				putText(cap_image, labelGesture, cv::Point(10, 10), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
			}

			if (m_number_of_fingers == 2) {
				string labelGesture = "Gesture: Peace";
				putText(cap_image, labelGesture, cv::Point(10, 10), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));						
			}

			else if (m_number_of_fingers == 5) {
				string labelGesture = "Gesture: Extended";
				putText(cap_image, labelGesture, cv::Point(10, 10), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
			}

			else if (detectRoundFigure(cap_image, 400, 0.6)) {
				string labelGesture = "Gesture: Oki Doki";
				putText(cap_image, labelGesture, cv::Point(10, 10), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
			}
			else {
				stringstream s;
				s << int(m_number_of_fingers);
				string FingerNumberString = s.str();
				putText(cap_image, FingerNumberString, cv::Point(10, 10), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

			}
			

		}








		//get the frame number and write it on the current frame
		stringstream ss;
		rectangle(cap_image, cv::Point(10, 2), cv::Point(100, 20), Scalar(255, 255, 255), -1);
		ss << cap.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		putText(cap_image, frameNumberString.c_str(), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		//show the current frame and the fg masks

		//imshow("FG Mask MOG 2", fgMaskMOG2);



		//Since we can assume we are working with a perfectly solid background

		char * temp = "largest Contour";
		imshow(temp, dst);
		imshow("Camera Capture", cap_image);
		waitKey(33);          //delay 33ms

	 }



		//ALTERNATIVE - non adaptative background
		//cap_image = bg - cap_image  ;

		






	 return 0;


	}



