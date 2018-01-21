#include "AlignWithTape.h"

AlignWithTape::AlignWithTape(float alignTime, float timeoutInSeconds, AlignDirection defaultAlignDirection)
	:  m_alignMode(alignTime == 0.0f ? AlignMode::APPROACH : AlignMode::ROTATE),
	   m_alignFrames((int)(alignTime * m_UPDATE_RATE)), m_approachFrames((int)(timeoutInSeconds * m_UPDATE_RATE)),
	m_throttle(0), m_rotation(0), m_frameCount(0),
	m_framesLost(0), m_defaultAlignDirection(defaultAlignDirection)
{
//	Requires(CommandBase::driveSubsystem.get());
}

// Called just before this Command runs the first time
void AlignWithTape::Initialize()
{
	//qdatatransfer = NetworkTable::GetTable("datatable");
//	CommandBase::driveSubsystem->Drive(0.0f, 0.0f);

	m_throttle = 0.0f;
	m_rotation = 0.0f;

	m_frameCount = 0;
	m_framesLost = 0;

	std::cout << "Aligning\n";
}

// Called repeatedly when this Command is scheduled to run
void AlignWithTape::Execute()
{
	oi->ProcessAlignment();
	cv::Point center = oi->GetCenter();

	switch (m_alignMode)
	{
	case AlignMode::ROTATE:
		std::cout << m_alignFrames << std::endl;
		m_throttle = 0;

		if (oi->GetContours().size() >= 2)
		{
			m_rotation = fmax(fmin((m_SENSITIVITY_ALIGN * ((float)center.x / (float)OI::CAMERA_X_RES)), m_MAX_ROTATION), -m_MAX_ROTATION);
		}
		else
		{
			switch (m_defaultAlignDirection)
			{
			case AlignDirection::NONE:
				m_rotation = 0;
				break;
			case AlignDirection::CW:
				m_rotation = m_MAX_ROTATION;
				break;
			case AlignDirection::CCW:
				m_rotation = -m_MAX_ROTATION;
				break;
			}
		}

		if (m_frameCount > m_alignFrames)
		{
			m_alignMode = AlignMode::APPROACH;
			m_frameCount = 0;
		}

		break;
	case AlignMode::APPROACH:
		//std::cout << "Aligning\n";
		if (oi->GetContours().size() >= 2)
		{
			m_throttle = .5;
			//m_throttle = fmax(fmax(OI::CAMERA_Y_RES - center.y, 0) / (float)OI::CAMERA_Y_RES * m_MAX_THROTTLE, m_MIN_THROTTLE);
			m_rotation = fmax(fmin((m_SENSITIVITY_X * ((float)center.x / (float)OI::CAMERA_X_RES)), m_MAX_ROTATION), -m_MAX_ROTATION);

			m_framesLost = 0;
		}
		else
		{
			m_framesLost++;
		}
		break;
	}
	networktable->PutNumber("speed", m_throttle);
	networktable->PutNumber("turn", m_rotation);

	m_frameCount++;

	std::cout << m_rotation << std::endl;
}

// Make this return true when this Command no longer needs to run execute()
bool AlignWithTape::IsFinished()
{
	//return false;
	return m_frameCount > m_approachFrames || m_framesLost > m_TARGET_LOST_TIMEOUT;
}
