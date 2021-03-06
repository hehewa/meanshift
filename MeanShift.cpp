// MeanShift.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<opencv2\opencv.hpp>
#include"tools.h"

using namespace std;
using namespace cv;

//*********************
//用于目标选择的变量
//
//
//*********************
cv::Mat roi;
cv::Rect roi_rect;
//用于标志是否已经选择了ROI
bool is_selected = false;
// 用于标志是否已经开始了跟踪
bool start_tracking = false;
//用于存储ROI的起始点以及终止点坐标
cv::Point startPoint;
cv::Point endPoint;
// 鼠标事件的回调函数
void mouseHandler(int event, int x, int y, int flags, void* param);


int main()
{
	// 调用摄像头
	VideoCapture cap(0);
	Mat frame;
	std::string winame = "tracking";
	Mat target_model;
	Mat candiate_model;


	cv::namedWindow(winame);
	setMouseCallback(winame, mouseHandler, 0);

	while (1) {
		cap >> frame;
		if (!frame.empty()) {
			cv::imshow(winame, frame);
			if (is_selected && !start_tracking) { // 用于选取目标框	
				roi = frame(roi_rect);
				// 设置此标志为，开始跟踪
				start_tracking = true;
				cv::imshow("roi", roi);
				rectangle(frame, roi_rect, Scalar(0, 0, 255), 2);
				// 计算目标图像的Epannechnikov分布的权值矩阵
				//cv_tools::calDistWeights(roi, roi_rect);
				// 对目标进行直方图建模
				target_model = cv_tools::calTargetHist(roi, roi_rect,16);
			}
			else if (start_tracking) {
				// 对候选目标进行直方图建模
				candiate_model = cv_tools::calCandidateHist(frame, roi_rect, 16);
				// 计算w矩阵
				cv::Mat w = cv_tools::calW(target_model, candiate_model);
				// 得到候选目标框所对应的图像
				cv::Mat target = frame(roi_rect);
				// 计算sum_w和sum_wx
				int width = target.cols;
				int height = target.rows;
				// 计算的是作为一个独立的矩形，其中心点的坐标
				int f_x = width / 2;
				int f_y = height / 2;
				float f[] = {f_x, f_y};
				//存储对应像素在直方图中的位置
				cv::Mat indexs = cv::Mat::zeros(target.size(), CV_16UC1);
				for (int i = 0; i < width; i++) {
					for (int j = 0; j < height; j++) {
						int v_r = floor(roi.at<cv::Vec3b>(j, i)[0] / 16);
						int v_g = floor(roi.at<cv::Vec3b>(j, i)[1] / 16);
						int v_b = floor(roi.at<cv::Vec3b>(j, i)[2] / 16);
						indexs.at<ushort>(j, i) = v_r * 256 + v_g * 16 + v_b;
					}
				}
				float sum_w = 0;
				float xw[] = {0, 0};
				cv::Point m_vector = cv::Point(0, 0);
				for (int i = 0; i < width; i++) {
					for (int j = 0; j < height; j++) {
						sum_w = sum_w + w.at<float>(0, indexs.at<ushort>(j, i));
						//xw = xw + w.at<float>(0, indexs.at<int>(j, i)) *[i - y(1) - 0.5, j - y(2) - 0.5];
						xw[0] = xw[0] + w.at<float>(0, indexs.at<ushort>(j, i)) *(i - f_x - 0.5);
						xw[1] = xw[1] + w.at<float>(0, indexs.at<ushort>(j, i)) *(j - f_y - 0.5);
					}
				}
				f[0] = xw[0] / sum_w;
				f[1] = xw[1] / sum_w;
				// 中心点位置更新
				roi_rect.x = roi_rect.x + f[0];
				roi_rect.y = roi_rect.y + f[1];
				rectangle(frame, roi_rect, Scalar(0, 0, 255), 2);

				cv::imshow(winame, frame);
			}
			//std::cout << roi_rect << endl;

			cv::waitKey(1);
		}


	}
	return 0;
}

void mouseHandler(int event, int x, int y, int flags, void* param){
	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		//代码
		if (!is_selected) {
			startPoint = cv::Point(x, y);
		}
		break;
	case CV_EVENT_LBUTTONUP:
		//代码
		if (!is_selected) {
			if (roi_rect.area() != 0) {

			}
			is_selected = true;
			endPoint = cv::Point(x, y);
			roi_rect = cv::Rect(startPoint, endPoint);
		}
		break;
	case CV_EVENT_LBUTTONDBLCLK:
		//代码
		break;
	case CV_EVENT_RBUTTONDOWN:
		//代码
		break;
	case CV_EVENT_RBUTTONUP:
		//代码
		break;
	case CV_EVENT_RBUTTONDBLCLK:
		//代码
		break;
	case CV_EVENT_MBUTTONDOWN:
		//代码
		break;
	case CV_EVENT_MBUTTONUP:
		//代码
		break;
	case CV_EVENT_MBUTTONDBLCLK:
		//代码
		break;
	case CV_EVENT_MOUSEMOVE:
		//代码
		break;
	}
}