#pragma once
#ifndef ERROR_H 
#define ERROR_H 

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "detection_image.h"
#include "detection_shading.h"
#include "detection_blemish.h"
#include "detection_flare.h"
#include "terminal_data.h"

namespace detection_app
{
	class ErrorException
	{
	public:
		void CheckInputParameter(int argc);
		void CheckNullPointer(LogData *pointer);
		void CheckNullPointer(TerminalData *pointer);
		void CheckNullPointer(DetectionShading *pointer);
		void CheckNullPointer(DetectionBlemish *pointer);
		void CheckNullPointer(DetectionFlare *pointer); 
		void CheckNullPointer(DetectionImage *pointer);
		void CheckFileExsist(std::string file_path);

	};

}

#endif // ERROR_H