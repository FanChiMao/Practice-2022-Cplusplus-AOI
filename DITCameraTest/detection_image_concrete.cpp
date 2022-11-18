#include "opencv2/opencv.hpp"
#include "detection_image_concrete.h"
#include "fstream"
#include <windows.h>
#include "error_exception.h"

using namespace detection_app;


void DetectionImageConcrete::SetTerminalData(TerminalData *input_data)
{
	ErrorException error_code;
	error_code.CheckNullPointer(input_data);
	m_input_data = input_data;
	
	m_input_data->m_serial_name = "TFXXXT_0000";
	m_input_data->m_date = SetCurrentTime(false);
	m_input_data->m_date_time = SetCurrentTime(true);
	m_input_data->m_version = "1.0.9.4";
	m_input_data->m_project_name = "DIT_Jonathan_Demo_version";

	m_input_data->m_save_path = m_input_data->m_output_image_path + m_input_data->m_date + "/" + m_input_data->m_serial_name + "/";
	m_input_data->m_save_csv_name = "Report-" + m_input_data->m_serial_name + "-" + m_input_data->m_date_time + ".csv";
} 


void DetectionImageConcrete::SetDetectionImage(DetectionImage *image_detection)
{
	ErrorException error_code;
	error_code.CheckNullPointer(image_detection);
	m_image_detection = image_detection;
	m_image_detection->Initialization(m_input_data);
}


void DetectionImageConcrete::DetectAndLog()
{
	vector< vector<string> > result_string;
	m_output_data = m_image_detection->RunDetection();
	m_output_data_string = GenerateLogString();
	WriteLog();
}

void DetectionImageConcrete::CreateLogFile()
{
	ErrorException error_code;

	ofstream myfile;
	string log_file = m_input_data->m_save_path + m_input_data->m_save_csv_name;
	
	bool b_path_exsit = !(_access(log_file.c_str(), 0) == -1);
	if (!b_path_exsit)
		CreateRootPath(log_file);

	myfile.open(log_file, ios_base::app);

	error_code.CheckFileExsist(log_file);
	if (!b_path_exsit)
	{
		myfile << "Report_Version:1.0.0.0,Project_Name:DIT_C++_Training" << endl;
		myfile << "ITEM,STATUS,VALUE,UCL,LCL,RESULT,SPEC_NAME,DATE_TIME,OBJ_NAME,COMMENT,OTHERS,IMG" << endl;
	}
	
	myfile.close();
}

void DetectionImageConcrete::CreateRootPath(string file_path)
{
	size_t len = file_path.length();
	char tmp_path[256] = { NULL };
	for (int i = 0; i < len; i++)
	{
		tmp_path[i] = file_path[i];
		if (tmp_path[i] == '\\' || tmp_path[i] == '/')
		{
			if (_access(tmp_path, 0) == -1)
			{
				if (_mkdir(tmp_path) != -1)
					_mkdir(tmp_path);
				else
					return;
			}
		}
	}
}

string DetectionImageConcrete::SetCurrentTime(bool is_detail)
{
	SYSTEMTIME date;
	GetLocalTime(&date);

	char date_char[20] = { NULL };

	if (is_detail)
	{ 
		sprintf_s(date_char, "%4d%02d%02d%02d%02d%02d", date.wYear, date.wMonth, date.wDay , date.wHour, date.wMinute, date.wSecond);
	}
	else
	{ 
		sprintf_s(date_char, "%4d%02d%02d", date.wYear, date.wMonth, date.wDay);
	}

	return date_char;
}

void DetectionImageConcrete::WriteLog()
{
	ErrorException error_code;

	cout << "===========================================" << endl;
	ofstream myfile;
	string log_file = m_input_data->m_save_path + m_input_data->m_save_csv_name;

	error_code.CheckNullPointer(m_input_data);
	if (m_input_data->m_display_result_info)
	{
		cout << "ATD Line: " << m_output_data_string[1].size() << endl;
		for (int i = 0; i < m_output_data_string[1].size(); i++)
		{
			cout << m_output_data_string[1][i] << endl;
		}
	}
	
	error_code.CheckFileExsist(log_file);
	myfile.open(log_file, ios_base::app);

	for (int i = 0; i < m_output_data_string[0].size(); i++)
	{
		myfile << m_output_data_string[0][i] << endl;
	}

	myfile.close();
	cout << "===========================================" << endl;
}

vector< vector<string> > DetectionImageConcrete::GenerateLogString()
{
	ErrorException error_code;
	error_code.CheckNullPointer(m_input_data);

	vector< vector<string> > result_string(2); // <for Log, for terminal>

	string date_time = SetCurrentTime(true).insert(8, " ");

	string detection_info_log = m_input_data->m_detection_task + ",-1,0,0,0," + m_output_data.m_final_result + ","
		+ m_output_data.m_spec_name + "," + date_time + "," + m_input_data->m_detection_task + "," + m_output_data.m_others + ",";
	string file_name_info_log = m_input_data->m_detection_task + "_FileName,,,,,,,,,," + m_input_data->m_input_image_path + ",";
	string draw_image_info_log = m_input_data->m_detection_task + "_DrawImg,,,,,,,,,," + m_output_data.m_result_image_name + ",";

	string detection_info_terminal = m_input_data->m_detection_task + ",0,0,0," + m_output_data.m_final_result;

	result_string[0].push_back(detection_info_log);
	result_string[1].push_back(detection_info_terminal);

	if (m_input_data->m_detection_task == "Shading")
	{
		string tl_string_log
			= "Shading_TL,0,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.top_left_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.tl_result + ","
			+ m_output_data.m_spec_name + ","
			+ date_time + ",,,,";
		string tl_string_terminal
			= "Shading_TL,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.top_left_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.tl_result;
		string tr_string_log
			= "Shading_TR,0,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.top_right_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.tr_result + ","
			+ m_output_data.m_spec_name + ","
			+ date_time + ",,,,";
		string tr_string_terminal
			= "Shading_TR,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.top_right_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.tr_result;
		string bl_string_log
			= "Shading_BL,0,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.bottom_left_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.bl_result + ","
			+ m_output_data.m_spec_name + ","
			+ date_time + ",,,,";
		string bl_string_terminal
			= "Shading_BL,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.bottom_left_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.bl_result;
		string br_string_log
			= "Shading_BR,0,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.bottom_right_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.br_result + ","
			+ m_output_data.m_spec_name + ","
			+ date_time + ",,,,";
		string br_string_terminal
			= "Shading_BR,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.bottom_right_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_level_low)) + ","
			+ m_output_data.m_shade_result_set.br_result;
		string center_string_log
			= "Shading_Center,0,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.center_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_center_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_center_low)) + ","
			+ m_output_data.m_shade_result_set.center_result + ","
			+ m_output_data.m_spec_name + ","
			+ date_time + ",,,,";
		string center_string_terminal
			= "Shading_Center,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.center_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_center_up)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_center_low)) + ","
			+ m_output_data.m_shade_result_set.center_result;
		string diff_string_log
			= "Shading_Diff,0,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.diff_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_diff)) + ","
			+ to_string(0) + ","
			+ m_output_data.m_shade_result_set.diff_result + ","
			+ m_output_data.m_spec_name + ","
			+ date_time + ",,,,";
		string diff_string_terminal
			= "Shading_Diff,"
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.diff_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_shade_result_set.pass_diff)) + ","
			+ to_string(0) + ","
			+ m_output_data.m_shade_result_set.diff_result;
		result_string[0].push_back(tl_string_log);
		result_string[1].push_back(tl_string_terminal);
		result_string[0].push_back(tr_string_log);
		result_string[1].push_back(tr_string_terminal);
		result_string[0].push_back(bl_string_log);
		result_string[1].push_back(bl_string_terminal);
		result_string[0].push_back(br_string_log);
		result_string[1].push_back(br_string_terminal);
		result_string[0].push_back(center_string_log);
		result_string[1].push_back(center_string_terminal);
		result_string[0].push_back(diff_string_log);
		result_string[1].push_back(diff_string_terminal);

	}
	else if (m_input_data->m_detection_task == "Blemish")
	{
		string single_string_log
			= "Blemish_LargestRegion,0,"
			+ to_string(static_cast<int>(m_output_data.m_blemish_result_set.largest_region_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_blemish_result_set.pass_region)) + ","
			+ "0" + ","
			+ m_output_data.m_blemish_result_set.region_result + ","
			+ m_output_data.m_spec_name + ","
			+ date_time + ","
			+ ","
			+ ","
			+ ",";

		string single_string_terminal
			= "Blemish_LargestRegion,"
			+ to_string(static_cast<int>(m_output_data.m_blemish_result_set.largest_region_value)) + ","
			+ to_string(static_cast<int>(m_output_data.m_blemish_result_set.pass_region)) + ","
			+ "0" + ","
			+ m_output_data.m_blemish_result_set.region_result;

		result_string[0].push_back(single_string_log);
		result_string[1].push_back(single_string_terminal);
	}
	else if (m_input_data->m_detection_task == "Flare")
	{
		string single_string_log
			= "Flare_Variance,0,"
			+ to_string(static_cast<int>(m_output_data.m_flare_result_set.laplacian_variance_value)) + ","
			+ "0" + ","
			+ to_string(static_cast<int>(m_output_data.m_flare_result_set.pass_variance)) + ","
			+ m_output_data.m_flare_result_set.variance_result + ","
			+ m_input_data->m_spec_file_path.substr(m_input_data->m_spec_file_path.find_last_of("\\/") + 1) + ","
			+ SetCurrentTime(true).insert(8, " ") + ","
			+ ","
			+ ","
			+ ",";

		string single_string_terminal
			= "Flare_Variance,"
			+ to_string(static_cast<int>(m_output_data.m_flare_result_set.laplacian_variance_value)) + ","
			+ "0" + ","
			+ to_string(static_cast<int>(m_output_data.m_flare_result_set.pass_variance)) + ","
			+ m_output_data.m_flare_result_set.variance_result;

		result_string[0].push_back(single_string_log);
		result_string[1].push_back(single_string_terminal);
	}
		
	result_string[0].push_back(file_name_info_log);
	result_string[0].push_back(draw_image_info_log);
	
	return result_string;
}

