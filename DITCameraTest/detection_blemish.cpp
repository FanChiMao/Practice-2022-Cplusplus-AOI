#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include <windows.h>
#include "detection_blemish.h"
#include "error_exception.h"

using namespace detection_app;

void DetectionBlemish::Initialization(TerminalData *data)
{
	ErrorException error_code;
	error_code.CheckNullPointer(data);
	m_data = data;

	string spec_path = m_data->m_spec_file_path;

	error_code.CheckFileExsist(spec_path);
	ifstream cFile(spec_path);
	vector<double> config;
	if (cFile.is_open())
	{
		string line;
		int flag = 0;
		while (getline(cFile, line)) 
		{
			line = line.c_str();
			line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

			if (line.empty() && flag == 1)
				break;
			else if (line.empty())
				flag = 0;
			else if (line == "[Blemish]")
				flag = 1;

			else if (flag == 1)
			{
				stringstream buffer;
				double value;
				auto delimiterPos = line.find("=");
				auto name = line.substr(0, delimiterPos);
				auto value_char = line.substr(delimiterPos + 1);
				buffer << value_char;
				buffer >> value;
				config.push_back(value);
			}
		}
	}
	cFile.close();

	m_blemish_result.m_item.push_back("_LargestRegion");
	m_blemish_result.m_spec_name = spec_path.substr(spec_path.find_last_of("\\/") + 1);
	m_blemish_result.m_obj_name = "Blemish";
	m_blemish_result.m_blemish_result_set.pass_region = config[0];
	m_blemish_result.m_others = "Spec: PassRegion=0~" + to_string(static_cast<int>(config[0])) + ";";

}


LogData DetectionBlemish::RunDetection()
{
	ErrorException error_code;
	error_code.CheckNullPointer(m_data);

	cv::Mat image = ReadImage(m_data->m_input_image_path);
	cv::Mat img_grayscale, img_equalization, img_binarymask, img_gaussianblur, img_canny;

	cvtColor(image, img_grayscale, cv::COLOR_BGR2GRAY);
	equalizeHist(img_grayscale, img_equalization);
	GaussianBlur(img_equalization, img_gaussianblur, cv::Size(31, 31), 0);
	threshold(img_gaussianblur, img_binarymask, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	Canny(img_binarymask, img_canny, 20, 160);

	// find contours and get the largest contour

	vector<vector<cv::Point> > contours;
	vector<vector<cv::Point> > largest_contours;
	cv::Rect box;
	double largest_area = 0;
	double area;
	findContours(img_canny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		area = fabs(contourArea(contours[i]));
		if (area >= largest_area)
		{
			largest_area = area;
			largest_contours.clear();
			largest_contours.push_back(contours[i]);
			box = boundingRect(contours[i]);
		}
	}

	if (m_data->m_display_result_image)
	{
		ShowImage(img_equalization);
		ShowImage(img_gaussianblur);
		ShowImage(img_binarymask);
		ShowImage(img_canny);
		ShowImage(image);
	}

	m_blemish_result.m_blemish_result_set.largest_region_value = largest_area;

	string result = Judge();
	m_blemish_result.m_final_result = result;

	if (result == "FAIL")
	{
		// drawContours(image, largest_contours, -1, cv::Scalar(0, 0, 0), 2);
		rectangle(image, box, cv::Scalar(0, 0, 255), 4);
	}

	m_blemish_result.m_result_image_name = GetResultImageName(result);

	SaveImage(image, m_blemish_result.m_result_image_name);

	return m_blemish_result;
}


string DetectionBlemish::Judge()
{
	string final_result = "PASS";
	if (m_blemish_result.m_blemish_result_set.largest_region_value >= m_blemish_result.m_blemish_result_set.pass_region)
	{
		m_blemish_result.m_blemish_result_set.region_result = "FAIL";
		final_result = "FAIL";
	}

	return final_result;
}