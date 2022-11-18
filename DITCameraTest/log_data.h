#pragma once
#ifndef LOG_DATA_H 
#define LOG_DATA_H 

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace detection_app
{
	class LogData
	{
	public:
		vector<string> m_item;
		int m_status = 0;
		string m_final_result = "";
		string m_spec_name = "";
		string m_date_time = "";
		string m_obj_name = "";
		string m_others = "";
		string m_command = "";
		string m_result_image_name = "";
		struct ShadingResults
		{
			double top_left_value = 0;
			double top_right_value = 0;
			double bottom_left_value = 0;
			double bottom_right_value = 0;
			double center_value = 0;
			double diff_value = 0;
			double pass_level_up = 0;
			double pass_level_low = 0;
			double pass_diff = 0;
			double pass_center_up = 0;
			double pass_center_low = 0;
			string tl_result = "";
			string tr_result = "";
			string bl_result = "";
			string br_result = "";
			string center_result = "";
			string diff_result = "";
		} m_shade_result_set;
		struct BlemishResults
		{
			double largest_region_value = 0;
			double pass_region = 0;
			string region_result = "";
		} m_blemish_result_set;
		struct FlareResults
		{
			double laplacian_variance_value = 0;
			double pass_variance = 0;
			string variance_result = "";
		} m_flare_result_set;

	};
}

#endif // LOG_DATA_H