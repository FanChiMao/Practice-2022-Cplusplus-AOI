#include <iostream>
#include "opencv2/opencv.hpp"
#include "detection_image_concrete.h"
#include "detection_image.h"
#include "detection_shading.h"
#include "detection_blemish.h"
#include "detection_flare.h"
#include "terminal_data.h"
#include <string.h>
#include "error_exception.h"

using namespace detection_app;
using namespace std;

typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

hash_t hash_(char const* str)
{
	// convert string to single interger value for switch statement
	hash_t ret{basis};

	while (*str)
	{
		ret ^= *str;
		ret *= prime;
		str++;
	}
	return ret;
}

constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
{
	return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}


int main(int argc, char *argv[])
{
	try
	{
		ErrorException error_code;
		error_code.CheckInputParameter(argc);


		TerminalData *p_data = new TerminalData(); // input data from user
		DetectionImage *p_mission = NULL; // virtual interface

		error_code.CheckNullPointer(p_data);
		switch ((hash_(argv[1])))
		{
		case hash_compile_time("-LS"):
			p_data->m_detection_task = "Shading";
			p_mission = new DetectionShading();
			break;

		case hash_compile_time("-BL"):
			p_data->m_detection_task = "Blemish";
			p_mission = new DetectionBlemish();
			break;

		case hash_compile_time("-FL"):
			p_data->m_detection_task = "Flare";
			p_mission = new DetectionFlare();
			break;

		default:
			throw invalid_argument("Please enter supported task detection! (-LS, -BL, -FL)");
			return 1;
		}

		error_code.CheckNullPointer(p_data);
		p_data->m_spec_file_path = argv[2];
		p_data->m_input_image_path = argv[3];

		DetectionImageConcrete *p_context = new DetectionImageConcrete();
		p_context->SetTerminalData(p_data);
		p_context->CreateLogFile();
		p_context->SetDetectionImage(p_mission);
		p_context->DetectAndLog();


		error_code.CheckNullPointer(p_data);
		error_code.CheckNullPointer(p_mission);
		delete p_data;
		delete p_mission;
		delete p_context;
		p_data = NULL;
		p_mission = NULL;

		return 0;
		system("pause");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}

}
