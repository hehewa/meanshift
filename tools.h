#pragma once
#include<opencv2\opencv.hpp>


namespace cv_tools {

	std::vector<float> calHist(cv::Mat img,int bin);
	cv::Mat calDistWeights(cv::Mat target, const cv::Rect &roi); // 计算目标图像的Epannechnikov分布的权值矩阵
	// 计算要跟踪目标的直方图
	cv::Mat calTargetHist(cv::Mat target, const cv::Rect &roi, int bins = 16);
	// 计算候选目标框的直方图
	cv::Mat calCandidateHist(cv::Mat target, const cv::Rect &roi_rect, int bins = 16);

	cv::Mat calW(cv::Mat hist1, cv::Mat hist2);
	float kernel_Epannechnikov();
	float sumMat(cv::Mat& inputImg);
}