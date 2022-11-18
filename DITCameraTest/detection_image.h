#pragma once
#ifndef  DETECTION_IMAGE_H 
#define DETECTION_IMAGE_H 

#include "log_data.h"
#include "terminal_data.h"
#include "opencv2/opencv.hpp"
#include <io.h>
#include <string.h>
#include <direct.h>

using namespace detection_app;

namespace detection_app
{
	class DetectionImage
	{
	protected:
		TerminalData *m_data = NULL;

	public:
		virtual ~DetectionImage() {};
		virtual void Initialization(TerminalData *data) = 0;
		virtual string Judge() = 0;
		virtual LogData RunDetection() = 0;

		cv::Mat ReadImage(string image_path);
		string GetResultImageName(string result);
		void SaveImage(cv::Mat image, string result_image_path);
		void ShowImage(cv::Mat image);

	};
}

#endif // DETECTION_IMAGE_H