#pragma once
#ifndef  DETECTION_IMAGE_CONCRETE_H 
#define DETECTION_IMAGE_CONCRETE_H 

#include <iostream>
#include "detection_image.h"
#include "terminal_data.h"

using namespace std;
using namespace detection_app;

namespace detection_app
{
	class DetectionImageConcrete
	{
	private:
		DetectionImage *m_image_detection = NULL;
		TerminalData *m_input_data = NULL;
		LogData m_output_data;
		vector< vector<string> > m_output_data_string;

	public:
		void SetDetectionImage(DetectionImage *image_detection);
		void SetTerminalData(TerminalData *input_data);
		void DetectAndLog();
		void CreateLogFile();
		void CreateRootPath(string file_path);
		string SetCurrentTime(bool is_detail);
		vector< vector<string> > GenerateLogString();
		void WriteLog();
	};
}

#endif // DETECTION_IMAGE_CONCRETE_H