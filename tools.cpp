#include "stdafx.h"
#include"tools.h"

namespace cv_tools {
	std::vector<float> calHist(cv::Mat img, int bin) {//����ֱ��ͼ����
		std::vector<float> hist;

		return hist;
	}

	cv::Mat calDistWeights(cv::Mat target,const cv::Rect &roi) { // ����Ŀ��ͼ���Epannechnikov�ֲ���Ȩֵ����
		int width = target.cols;
		int height = target.rows;
		// ���������Ϊһ�������ľ��Σ������ĵ������
		int f_x = target.cols / 2;
		int f_y = target.rows / 2;

		// ���㵱ǰ����λ�����������е����ĵ�x��y������
		int tic_x = roi.x + roi.width / 2;
		int tic_y = roi.y + roi.height / 2;
		// �½�һ���������ڴ洢Ȩֵ��CV_8UC1��ʾ�þ���Ϊ��ͨ������,���е�ֵȫ������Ϊ0
		cv::Mat m_wei = cv::Mat::zeros(cv::Size(width, height), CV_32FC1);
		// imshow("before", m_wei);
		int h = pow(f_x, 2) + pow(f_y, 2); //�˺�������
		for (int i = 0; i < width;i++) {
			for (int j = 0; j < height; j++) {
				float dist = pow(i - f_x, 2) + pow(j - f_y, 2);
				m_wei.at<float>(j, i) = 1 - dist / h;
			}
		}
		imshow("Epanechnikov�ֲ�����", m_wei);
		
		float C = 1 / sumMat(m_wei);

		return m_wei;
	}
	

	cv::Mat calTargetHist(cv::Mat target, const cv::Rect &roi_rect, int bins) {
		int width = target.cols;
		int height = target.rows;

		cv::Mat hist = cv::Mat::zeros(cv::Size(pow(bins, 3),1), CV_32FC1);

		// ����Ȩ�طֲ�
		cv::Mat weightsDist = calDistWeights(target, roi_rect);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int v_r = floor(target.at<cv::Vec3b>(j, i)[0]/16);
				int v_g = floor(target.at<cv::Vec3b>(j, i)[1]/16);
				int v_b = floor(target.at<cv::Vec3b>(j, i)[2]/16);
				int index = v_r * 256 + v_g * 16 + v_b;
				hist.at<float>(0, index) = hist.at<float>(0, index) + weightsDist.at<float>(j, i);
			}
		}
		float C = 1 / sumMat(weightsDist);
		hist = hist * C;

		return hist;
	}


	cv::Mat calCandidateHist(cv::Mat frame, const cv::Rect &roi_rect, int bins) {
		cv::Mat roi = frame(roi_rect);
		int width = roi.cols;
		int height = roi.rows;

		cv::Mat hist = cv::Mat::zeros(cv::Size(pow(bins, 3), 1), CV_32FC1);

		// ����Ȩ�طֲ�
		cv::Mat weightsDist = calDistWeights(roi, roi_rect);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int v_r = floor(roi.at<cv::Vec3b>(j, i)[0] / 16);
				int v_g = floor(roi.at<cv::Vec3b>(j, i)[1] / 16);
				int v_b = floor(roi.at<cv::Vec3b>(j, i)[2] / 16);
				
				int index = v_r * 256 + v_g * 16 + v_b;
				hist.at<float>(0, index) = hist.at<float>(0, index) + weightsDist.at<float>(j, i);
			}
		}
		float C = 1 / sumMat(weightsDist);
		hist = hist * C;

		return hist;
	}

	cv::Mat calW(cv::Mat hist1, cv::Mat hist2) {
		cv::Mat w = cv::Mat::zeros(cv::Size(4096,1),CV_32FC1);

		for (int i = 0; i < 4096; i++) {
			if (hist2.at<float>(i) != 0) {
				w.at<float>(i) = sqrt(hist1.at<float>(i) / hist2.at<float>(i));
			}
		}
		return w;
	}

	float kernel_Epannechnikov() {

		return 0;
	}


	// ����һ��Mat�е�����Ԫ�����
	float sumMat(cv::Mat& inputImg)
	{
		int sum = 0.0;
		int rowNumber = inputImg.rows;
		int colNumber = inputImg.cols * inputImg.channels();
		for (int i = 0; i < rowNumber; i++)
		{
			uchar* data = inputImg.ptr<uchar>(i);
			for (int j = 0; j < colNumber; j++)
			{
				sum = data[j] + sum;
			}
		}
		//mean = sum / rowNumber * inputImg.cols;
		return sum;
	}
}