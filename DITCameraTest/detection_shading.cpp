#include "opencv2/opencv.hpp"
#include "detection_shading.h"
#include <fstream>
#include "error_exception.h"

using namespace detection_app;

void DetectionShading::Initialization(TerminalData *data)
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
			else if (line == "[Shading]")
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

	m_shade_result.m_item.push_back("_TL");
	m_shade_result.m_item.push_back("_TR");
	m_shade_result.m_item.push_back("_BL");
	m_shade_result.m_item.push_back("_BR");
	m_shade_result.m_item.push_back("_Diff");
	m_shade_result.m_item.push_back("_Center");

	m_shade_result.m_spec_name = spec_path.substr(spec_path.find_last_of("\\/") + 1);
	m_shade_result.m_obj_name = "Shading";

	m_shade_result.m_shade_result_set.pass_level_up = config[0];
	m_shade_result.m_shade_result_set.pass_level_low= config[1];
	m_shade_result.m_shade_result_set.pass_diff= config[2];
	m_shade_result.m_shade_result_set.pass_center_up= config[3];
	m_shade_result.m_shade_result_set.pass_center_low = config[4];

	m_shade_result.m_others =
		"Spec: PassLevel=" + to_string(static_cast<int>(config[1])) + "~" + to_string(static_cast<int>(config[0])) + ";" +
		"Diff=0~" + to_string(static_cast<int>(config[2])) + ";" +
		"Center=" + to_string(static_cast<int>(config[4])) + "~" + to_string(static_cast<int>(config[3])) + ";";
}

LogData DetectionShading::RunDetection()
{
	cv::Mat image = ReadImage(m_data->m_input_image_path);
	cv::Mat image_resize, image_YCrCr, y_channel, center_roi;
	resize(image, image, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
	vector<cv::Mat> channels;
	int image_width = image.cols;
	int image_height = image.rows;
	int center_roi_width = image_width / 2;
	int center_roi_height = image_height / 2;
	int roi_width = image_width / 10;
	int roi_height = image_height / 10;
	cv::Mat y1, y2, y3, y4, yc;
	cv::Rect y1_xyhw(0, 0, roi_width, roi_height), y2_xyhw(image_width - roi_width, 0, roi_width, roi_height),
		y3_xyhw(0, image_height - roi_height, roi_width, roi_height), y4_xyhw(image_width - roi_width, image_height - roi_height, roi_width, roi_height);
	double y1_value, y2_value, y3_value, y4_value, yc_value;
	int thickness = 1;

	cvtColor(image, image_YCrCr, cv::COLOR_BGR2YCrCb);
	split(image_YCrCr, channels);
	y_channel = channels.at(0);

	y1 = y_channel(y1_xyhw);
	y2 = y_channel(y2_xyhw);
	y3 = y_channel(y3_xyhw);
	y4 = y_channel(y4_xyhw);

	y1_value = mean(y1)[0];
	y2_value = mean(y2)[0];
	y3_value = mean(y3)[0];
	y4_value = mean(y4)[0];

	center_roi = y_channel(cv::Rect(center_roi_width / 2, center_roi_height / 2, center_roi_width, center_roi_height));

	double largest_yc = DBL_MIN;
	double largest_x, largest_y;

	for (int j = 0; j < center_roi_height - roi_height; j++)
	{
		for (int i = 0; i < center_roi_width - roi_width; i++)
		{
			yc_value = cv::mean(center_roi(cv::Rect(i, j, roi_width, roi_height)))[0];
			if (yc_value > largest_yc)
			{
				largest_yc = yc_value;
				largest_x = center_roi_width / 2 + i;
				largest_y = center_roi_height / 2 + j;
			}
		}
	}

	rectangle(image, y1_xyhw, cv::Scalar(0, 0, 255), 2, 8, 0);
	rectangle(image, y2_xyhw, cv::Scalar(0, 0, 255), 2, 8, 0);
	rectangle(image, y3_xyhw, cv::Scalar(0, 0, 255), 2, 8, 0);
	rectangle(image, y4_xyhw, cv::Scalar(0, 0, 255), 2, 8, 0);
	rectangle(image, cv::Rect(static_cast<int>(largest_x), static_cast<int>(largest_y), roi_width, roi_height), cv::Scalar(0, 0, 255), 2, 8, 0);

	int bias = 25;
	putText(image, "Y1:" + to_string(static_cast<int>(y1_value)), cv::Point(0, bias), cv::FONT_HERSHEY_DUPLEX, .8, cv::Scalar(0, 0, 255), thickness);
	putText(image, "Y2:" + to_string(static_cast<int>(y2_value)), cv::Point(image_width - roi_width, bias), cv::FONT_HERSHEY_DUPLEX, .8, cv::Scalar(0, 0, 255), thickness);
	putText(image, "Y3:" + to_string(static_cast<int>(y3_value)), cv::Point(0, image_height - roi_height + bias), cv::FONT_HERSHEY_DUPLEX, .8, cv::Scalar(0, 0, 255), thickness);
	putText(image, "Y4:" + to_string(static_cast<int>(y4_value)), cv::Point(image_width - roi_width, image_height - roi_height + bias), cv::FONT_HERSHEY_DUPLEX, .8, cv::Scalar(0, 0, 255), thickness);
	putText(image, "Yc:" + to_string(static_cast<int>(yc_value)), cv::Point(static_cast<int>(largest_x), static_cast<int>(largest_y) + bias), cv::FONT_HERSHEY_DUPLEX, .8, cv::Scalar(0, 0, 255), thickness);

	if (m_data->m_display_result_image)
		ShowImage(image);

	m_shade_result.m_shade_result_set.top_left_value = y1_value;
	m_shade_result.m_shade_result_set.top_right_value = y2_value;
	m_shade_result.m_shade_result_set.bottom_left_value = y3_value;
	m_shade_result.m_shade_result_set.bottom_right_value = y4_value;
	m_shade_result.m_shade_result_set.center_value = yc_value;

	string result = Judge();
	m_shade_result.m_final_result = result;
	m_shade_result.m_result_image_name = GetResultImageName(result);

	SaveImage(image, m_shade_result.m_result_image_name);

	return m_shade_result;
}

string DetectionShading::Judge()
{
	double tl = m_shade_result.m_shade_result_set.top_left_value;
	double tr = m_shade_result.m_shade_result_set.top_right_value;
	double bl = m_shade_result.m_shade_result_set.bottom_left_value;
	double br = m_shade_result.m_shade_result_set.bottom_right_value;
	double ct = m_shade_result.m_shade_result_set.center_value;
	
	double pass_level_low = m_shade_result.m_shade_result_set.pass_level_low;
	double pass_level_up = m_shade_result.m_shade_result_set.pass_level_up;
	double pass_diff = m_shade_result.m_shade_result_set.pass_diff;
	double pass_center_low = m_shade_result.m_shade_result_set.pass_center_low;
	double pass_center_up = m_shade_result.m_shade_result_set.pass_center_up;

	string final_result = "PASS";

	// 1. top-left
	if (pass_level_low < tl && tl < pass_level_up)
		m_shade_result.m_shade_result_set.tl_result = "PASS";
	else
	{
		m_shade_result.m_shade_result_set.tl_result = "FAIL";
		final_result = "FAIL";
	}

	// 2. top-right
	if (pass_level_low < tr && tr < pass_level_up)
		m_shade_result.m_shade_result_set.tr_result = "PASS";
	else
	{
		m_shade_result.m_shade_result_set.tr_result = "FAIL";
		final_result = "FAIL";
	}

	// 3. bottom-left
	if (pass_level_low < bl && bl < pass_level_up)
		m_shade_result.m_shade_result_set.bl_result = "PASS";
	else
	{
		m_shade_result.m_shade_result_set.bl_result = "FAIL";
		final_result = "FAIL";
	}
		

	// 4. bottom-right
	if (pass_level_low < br && br < pass_level_up)
		m_shade_result.m_shade_result_set.br_result = "PASS";
	else
	{
		m_shade_result.m_shade_result_set.br_result = "FAIL";
		final_result = "FAIL";

	}
		
	// 5. center
	if (pass_center_low < ct && ct < pass_center_up)
		m_shade_result.m_shade_result_set.center_result = "PASS";
	else
	{
		m_shade_result.m_shade_result_set.center_result = "FAIL";
		final_result = "FAIL";
	}


	double diff = FindDiff(tl, tr, bl, br);
	m_shade_result.m_shade_result_set.diff_value = diff;
	// 6. diff
	if (diff < pass_diff)
		m_shade_result.m_shade_result_set.diff_result = "PASS";
	else
	{
		m_shade_result.m_shade_result_set.diff_result = "FAIL";
		final_result = "FAIL";
	}

	return final_result;
}


inline double DetectionShading::FindDiff(double a, double b, double c, double d)
{
	double max_value = max(max(max(a, b), c), d);
	double min_value = min(min(min(a, b), c), d);

	return max_value - min_value;
}