#include "detection_image.h"
#include "error_exception.h"

using namespace detection_app;

cv::Mat DetectionImage::ReadImage(std::string image_path)
{
	ErrorException error_code;
	error_code.CheckFileExsist(image_path);

	cv::Mat image = cv::imread(image_path);

	return image;
}

std::string DetectionImage::GetResultImageName(std::string pass_or_not)
{
	ErrorException error_code;

	error_code.CheckNullPointer(m_data);

	std::string input_path = m_data->m_input_image_path;
	std::string output_path = m_data->m_save_path;
	std::string output_image_name = m_data->m_serial_name + "-" + m_data->m_date_time + "-" + pass_or_not + "-" + m_data->m_detection_task + "-Debug.jpg";
	std::string save_path = output_path + '/' + output_image_name;

	return save_path;
}


void DetectionImage::SaveImage(cv::Mat result_image, std::string result_image_path)
{
	try
	{
		cv::imwrite(result_image_path, result_image);
	}
	catch (exception &e)
	{
		cout << "exception: " << e.what() << "\n";
	}
}


void DetectionImage::ShowImage(cv::Mat image)
{
	if (!image.data)
	{
		cout << "Failed to load image!" << endl;
		exit(-1);
	}
	cv::namedWindow("Result", cv::WINDOW_NORMAL);
	cv::imshow("Result", image);
	cv::waitKey(0);
}
