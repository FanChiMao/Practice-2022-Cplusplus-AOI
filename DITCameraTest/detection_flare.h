#pragma once
#ifndef  DETECTION_FLARE_H 
#define DETECTION_FLARE_H 

#include "detection_image.h"
#include <string.h>

using namespace detection_app;

namespace detection_app
{
	class DetectionFlare : public DetectionImage
	{
	private:
		LogData m_flare_result;

	public:
		virtual void Initialization(TerminalData *data);
		virtual LogData RunDetection();
		virtual string Judge();

	};
}

#endif // DETECTION_FLARE_H