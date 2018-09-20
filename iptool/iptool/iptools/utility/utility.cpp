#include "utility.h"
#include <fstream>
#include <sstream>

#define MAXRGB 255
#define MINRGB 0

using namespace cv;

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cv::cvtColor(src, tgt, CV_BGR2GRAY);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	blur(src,tgt,Size(WindowSize,WindowSize));
}

/*-----------------------------------------------------------------------**/
void utility::cv_histEqual(Mat &src, Mat &tgt, int x, int y, int Sx, int Sy, int rois) {
	Mat ycrcb, b_hist, g_hist, r_hist, hist;
	vector<Mat> channels;
	int histSize = 256;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	float range[] = { 0, 256 };
	const float* histRange = { range };
	Mat histImage_before(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	Mat histImage_after(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	if (rois > 0) {
		// Check if ROI doesn't go out of bounds of image
		if (x + Sx > src.cols) {
			std::cout << "ROI x out of bounds" << endl;
			return;
		}
		if (y + Sy > src.rows) {
			std::cout << "ROI y out of bounds" << endl;
			return;
		}
	}

	Rect r(x, y, Sx, Sy);
	Mat roi = src(r);

	if (src.channels() >= 3) {
		/*----------BEFORE HIST EQUAL----------*/
		if (rois > 0) {
			cv::cvtColor(roi, roi, CV_BGR2YCrCb);
			split(roi, channels);		
		}
		else {
			cv::cvtColor(src, ycrcb, CV_BGR2YCrCb);
			split(ycrcb, channels);
		}

		if (rois <= 1) {
			cv::calcHist(&channels[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);
			cv::normalize(hist, hist, 0, histImage_before.rows, NORM_MINMAX, -1, Mat());

			for (int i = 1; i < histSize; i++) {
				line(histImage_before, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
					Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
			}

			// Histogram of ROI before equalization
			cv::imwrite("../output/Equalization_Hist_Before.jpg", histImage_before);
		}

		cv::equalizeHist(channels[0], channels[0]);

		/*----------AFTER HIST EQUAL----------*/
		cv::calcHist(&channels[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);
		cv::normalize(hist, hist, 0, histImage_after.rows, NORM_MINMAX, -1, Mat());
		for (int i = 1; i < histSize; i++) {
			line(histImage_after, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		}

		// Histogram of ROI after equalization
		cv::imwrite("../output/Equalization_Hist_After.jpg", histImage_after);

		if (rois > 0) {
			cv::merge(channels, roi);
			cv::cvtColor(roi, roi, CV_YCrCb2BGR);
			tgt = src;
		}
		else {
			cv::merge(channels, ycrcb);
			cv::cvtColor(ycrcb, tgt, CV_YCrCb2BGR);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_sobel(cv::Mat &src, cv::Mat &tgt, int x, int y, int Sx, int Sy, int rois) {
	Mat grad;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	if (rois > 0) {
		// Check if ROI doesn't go out of bounds of image
		if (x + Sx > src.cols) {
			std::cout << "ROI x out of bounds" << endl;
			return;
		}
		if (y + Sy > src.rows) {
			std::cout << "ROI y out of bounds" << endl;
			return;
		}
	}

	Rect r(x, y, Sx, Sy);
	Mat roi = src(r);

	if (rois > 0) {
		GaussianBlur(roi, roi, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cv::cvtColor(roi, roi, COLOR_BGR2GRAY);

		// Sobel in dx and dy
		cv::Sobel(roi, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		cv::Sobel(roi, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);

		cv::convertScaleAbs(grad_x, abs_grad_x);
		cv::convertScaleAbs(grad_y, abs_grad_y);
		cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, roi);

		tgt = src;
	}
	else {
		GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cv::cvtColor(src, src, COLOR_BGR2GRAY);

		// Sobel in dx and dy
		cv::Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		cv::Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);

		cv::convertScaleAbs(grad_x, abs_grad_x);
		cv::convertScaleAbs(grad_y, abs_grad_y);
		cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

		tgt = grad;
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_canny(cv::Mat &src, cv::Mat &tgt, int x, int y, int Sx, int Sy, int rois) {
	Mat detected_edges;
	int ratio = 3;
	int kernel_size = 3;

	if (rois > 0) {
		// Check if ROI doesn't go out of bounds of image
		if (x + Sx > src.cols) {
			std::cout << "ROI x out of bounds" << endl;
			return;
		}
		if (y + Sy > src.rows) {
			std::cout << "ROI y out of bounds" << endl;
			return;
		}
	}

	Rect r(x, y, Sx, Sy);
	Mat roi = src(r);

	if (rois > 0) {
		cvtColor(roi, roi, CV_BGR2GRAY);
		blur(roi, roi, Size(3, 3));
		Canny(roi, roi, 60, 60 * ratio, kernel_size);
		tgt = src;
	}
	else {
		cvtColor(src, src, CV_BGR2GRAY);
		blur(src, detected_edges, Size(3, 3));
		Canny(detected_edges, detected_edges, 20, 50, kernel_size);
		tgt = detected_edges;
	}
}