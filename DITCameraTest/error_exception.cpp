#include "error_exception.h"
#include "detection_image.h"
#include "detection_image_concrete.h"
#include "detection_shading.h"
#include "detection_blemish.h"
#include "detection_flare.h"
#include "terminal_data.h"

using namespace detection_app;

void ErrorException::CheckInputParameter(int argc)
{
	if (argc != 4)
	{
		throw invalid_argument("[InvalidInput Exception] \n"
			"  Please enter 3 arguments as following: \n"
			"  [-LS, -BL, -FL] [path of SPE file] [path of testing image]");
	}
}

void ErrorException::CheckNullPointer(LogData *pointer)
{
	if (pointer == NULL)
	{
		throw invalid_argument("[Null Exception]\n  NullPointer of LogData");
	}
}

void ErrorException::CheckNullPointer(TerminalData *pointer)
{
	if (pointer == NULL)
	{
		throw invalid_argument("[Null Exception]\n  NullPointer of TerminalData");
	}
}

void ErrorException::CheckNullPointer(DetectionShading *pointer)
{
	if (pointer == NULL)
	{
		throw invalid_argument("[Null Exception]\n  NullPointer of DetectionShading");
	}
}

void ErrorException::CheckNullPointer(DetectionBlemish *pointer)
{
	if (pointer == NULL)
	{
		throw invalid_argument("[Null Exception]\n  NullPointer of DetectionBlemish");
	}
}

void ErrorException::CheckNullPointer(DetectionFlare *pointer)
{
	if (pointer == NULL)
	{
		throw invalid_argument("[Null Exception]\n  NullPointer of DetectionFlare");
	}
}

void ErrorException::CheckNullPointer(DetectionImage *pointer)
{
	if (pointer == NULL)
	{
		throw invalid_argument("[Null Exception]\n  NullPointer of DetectionImage");
	}
}

void ErrorException::CheckFileExsist(std::string file_path)
{
	if (_access(file_path.c_str(), 0) == -1)
	{
		throw invalid_argument("[FileNotExsist Exception]\n"
							   "  File path: "+ file_path + " doesn't exsist");
	}
}
