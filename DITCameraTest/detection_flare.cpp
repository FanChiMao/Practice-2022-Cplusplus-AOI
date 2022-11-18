#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include "detection_flare.h"
#include "error_exception.h"

using namespace detection_app;

void DetectionFlare::Initialization(TerminalData *data)
{
	ErrorException error_code;
	error_code.CheckNullPointer(data);
	m_data = data;

	std::string spec_path = m_data->m_spec_file_path;

	error_code.CheckFileExsist(spec_path);
	ifstream cFile(spec_path);
	vector<double> config;
	if (cFile.is_open())
	{
		std::string line;
		int flag = 0;
		while (getline(cFile, line))
		{
			line = line.c_str();
			line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

			if (line.empty() && flag == 1)
				break;
			else if (line.empty())
				flag = 0;
			else if (line == "[Flare]")
				flag = 1;

			else if (flag == 1)
			{
				std::stringstream buffer;
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

	m_flare_result.m_item.push_back("_Variance");
	m_flare_result.m_spec_name = spec_path.substr(spec_path.find_last_of("\\/") + 1);
	m_flare_result.m_obj_name = "Flare";
	m_flare_result.m_flare_result_set.pass_variance = config[0];
	m_flare_result.m_others = "Spec: PassVariance>" + to_string(static_cast<int>(config[0])) + ";";
}


LogData DetectionFlare::RunDetection()
{
	ErrorException error_code;
	error_code.CheckNullPointer(m_data);

	cv::Mat image = cv::imread(m_data->m_input_image_path);
	cv::Mat laplacianImage;
	cv::Mat img_grayscalae;

	cv::cvtColor(image, img_grayscalae, cv::COLOR_BGR2GRAY);
	cv::Laplacian(img_grayscalae, laplacianImage, CV_64F);

	// Compute the mean and standard deviation of the laplacian
	cv::Scalar mean, stddev;
	meanStdDev(laplacianImage, mean, stddev, cv::Mat());

	// Compute the variance
	double variance = stddev.val[0] * stddev.val[0];

	m_flare_result.m_flare_result_set.laplacian_variance_value = variance;

	std::string result = Judge();
	m_flare_result.m_final_result = result;

	putText(image, "Variance of Laplacian: ", cv::Point(20, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 255), 2, 15);
	putText(image, to_string(variance), cv::Point(20, 50), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 255), 2, 15);

	if (m_data->m_display_result_image)
		ShowImage(image);

	m_flare_result.m_result_image_name = GetResultImageName(result);

	SaveImage(image, m_flare_result.m_result_image_name);

	return m_flare_result;
}


std::string DetectionFlare::Judge()
{
	std::string final_result;
	if (m_flare_result.m_flare_result_set.laplacian_variance_value <= m_flare_result.m_flare_result_set.pass_variance)
	{
		m_flare_result.m_flare_result_set.variance_result = "FAIL";
		final_result = "FAIL";
	} 
	else
	{
		m_flare_result.m_flare_result_set.variance_result = "PASS";
		final_result = "PASS";
	}

	return final_result;
}
