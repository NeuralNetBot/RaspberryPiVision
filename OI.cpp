#include "OI.h"

OI::OI() {
	m_Contours = std::vector<Contour>();
	m_pGripNetworkTable = NetworkTable::GetTable("GRIP/tape");
	m_pAlignment = new grip::TapeAlignment();
	//m_Camera.SetExposureManual(16);
	cap.set(CV_CAP_PROP_EXPOSURE , 16);
	NetworkTable::SetTeam(3674);
	NetworkTable::SetClientMode();
	NetworkTable::Initialize();
	m_pNetworkTable = NetworkTable::GetTable("vision");
	m_Thread = std::thread(std::bind(&OI::AlignmentThread, this));
	m_Thread.detach();
	// Process operator interface input here.
}

void OI::ProcessAlignment()
{
	m_Contours = GenerateContours(m_pAlignment->getfilterContoursOutput());

	//std::cout << m_gearContours.size() << std::endl;
}

std::vector<OI::Contour> OI::GenerateContours(std::vector<std::vector<cv::Point>>* points)
{
	std::vector<Contour> contours;

	for (unsigned int i = 0; i < points->size(); i++)
	{
		Contour c;

		for (unsigned int j = 0; j < points->at(i).size(); j++)
		{
			cv::Point p = (cv::Point)points->at(i).at(j);

			c.left = std::min(c.left, p.x);
			c.right = std::max(c.right, p.x);
			c.top = std::min(c.top, p.y);
			c.bottom = std::max(c.bottom, p.y);
		}

		c.centerX = (c.left + c.right) / 2;
		c.centerY = (c.top + c.bottom / 2);

		contours.push_back(c);
	}

	return contours;
}

void OI::AlignmentThread()
{
	m_pVisionRunner->RunForever();
}

cv::Point OI::GetCenterContours(std::vector<Contour> contours)
{
	cv::Point avg;
	int numPoints = contours.size();

	if (numPoints == 0)
		return avg;

	for (int i = 0; i < numPoints; i++)
	{
		avg.x += contours[i].centerX;
		avg.y += contours[i].centerY;
	}

	avg.x /= numPoints;
	avg.y /= numPoints;
	avg.x -= CAMERA_X_RES / 2;
	avg.y -= CAMERA_Y_RES / 2;

	return avg;
}

cv::Point OI::GetCenter()
{
	return GetCenterContours(m_Contours);
}


std::vector<OI::Contour> OI::GetContours()
{
	return m_Contours;
}

grip::TapeAlignment* OI::GetAlignment()
{
	return m_pAlignment;
}

std::vector<double> OI::GetContourValues(std::string value)
{
	return m_pGripNetworkTable->GetNumberArray(value, llvm::ArrayRef<double>());
}

