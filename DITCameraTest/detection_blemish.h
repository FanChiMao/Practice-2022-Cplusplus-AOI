#pragma once
#ifndef  DETECTION_BLEMISH_H 
#define DETECTION_BLEMISH_H 

#include "detection_image.h"

using namespace detection_app;

namespace detection_app
{
	class DetectionBlemish : public DetectionImage
	{
	private:
		LogData m_blemish_result;

	public:
		virtual void Initialization(TerminalData *data);
		virtual LogData RunDetection();
		virtual string Judge();

	};
}

#endif // DETECTION_BLEMISH_H 