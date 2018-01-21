#ifndef OI_H
#define OI_H
#include <ntcore.h>
#include <networktables/NetworkTable.h>
#include <thread>
#include "TapeAlignment.h"

class OI {
private:

public:
	cv::VideoCapture cap(0);
	cv::Mat frame;
	
	struct Contour
	{
		int left;
		int right;
		int top;
		int bottom;
		int centerX;
		int centerY;

		Contour()
		{
			left = cap.get(CV_CAP_PROP_FRAME_WIDTH);
			right = 0;
			top = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
			bottom = 0;
			centerX = top / 2;
			centerY = bottom / 2;
		}
	};

	OI();

	grip::TapeAlignment* GetAlignment();

	std::vector<double> GetContourValues(std::string value);
	std::vector<Contour> GetContours();
	cv::Point GetCenter();

	cs::UsbCamera m_Camera;

	grip::TapeAlignment* m_pAlignment;

	std::shared_ptr<NetworkTable> m_pNetworkTable;
	std::shared_ptr<NetworkTable> m_pGripNetworkTable;

	std::thread m_thread;
	
	std::vector<Contour> m_Contours;

	void AlignmentThread();

	void ProcessAlignment();

	std::vector<Contour> GenerateContours(std::vector<std::vector<cv::Point>>* points);
	cv::Point GetCenterContours(std::vector<Contour> contours);

};

#endif  // OI_H
