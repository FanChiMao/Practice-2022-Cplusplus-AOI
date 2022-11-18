#pragma once
#ifndef  TERMINAL_DATA_H 
#define TERMINAL_DATA_H 

#include <iostream>

using namespace std;

namespace detection_app
{
	class TerminalData
	{
	public:
		string m_detection_task = "";
		string m_input_image_path = "";
		string m_output_image_path = "./ASUS_Camera_Reports_DLL/";
		string m_spec_file_path = "";
		bool m_display_result_image = false;
		bool m_display_result_info = true;

		string m_serial_name = "TFXXXT_0000";
		string m_date = "";
		string m_date_time = "";
		string m_project_name = "DIT_Jonathan_Demo_version";
		string m_version = "1.0.9.4";
		string m_save_path = "";
		string m_save_csv_name = "";
	};
}

#endif // TERMINAL_DATA_H