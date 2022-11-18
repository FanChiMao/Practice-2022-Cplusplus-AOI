#pragma once
#ifndef  DETECTION_SHADING_H 
#define DETECTION_SHADING_H 

#include "detection_image.h"

using namespace detection_app;

namespace detection_app
{
	class DetectionShading : public DetectionImage
	{
	private:
		LogData m_shade_result;

	public:
		virtual void Initialization(TerminalData *data);
		virtual LogData RunDetection();
		virtual string Judge();
		double FindDiff(double, double, double, double);

	};
}

#endif // DETECTION_SHADING_H