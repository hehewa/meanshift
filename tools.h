#pragma once
#include<opencv2\opencv.hpp>


namespace cv_tools {

	std::vector<float> calHist(cv::Mat img,int bin);
	cv::Mat calDistWeights(cv::Mat target, const cv::Rect &roi); // ����Ŀ��ͼ���Epannechnikov�ֲ���Ȩֵ����
	// ����Ҫ����Ŀ���ֱ��ͼ
	cv::Mat calTargetHist(cv::Mat target, const cv::Rect &roi, int bins = 16);
	// �����ѡĿ����ֱ��ͼ
	cv::Mat calCandidateHist(cv::Mat target, const cv::Rect &roi_rect, int bins = 16);

	cv::Mat calW(cv::Mat hist1, cv::Mat hist2);
	float kernel_Epannechnikov();
	float sumMat(cv::Mat& inputImg);
}