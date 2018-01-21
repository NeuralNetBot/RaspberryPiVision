#ifndef AlignWithTape_H
#define AlignWithTape_H


class AlignWithTape
{
private:
	OI* oi = OI();
	NetworkTable::SetIPAddress("10.20.53.21");
	NetworkTable::SetClientMode();
	NetworkTable::Initialize();
	std::shared_ptr<NetworkTable> datatransfer;
	datatransfer = NetworkTable::GetTable("vision");
	enum AlignMode
	{
		ROTATE,
		APPROACH
	} m_alignMode;

	const float m_SENSITIVITY_X = 2.0f;
	const float m_SENSITIVITY_Y = 0.5f;
	const float m_SENSITIVITY_ALIGN = 4.0f;
	const float m_MAX_THROTTLE = 1.0f;//0.25f;//0.4f;
	const float m_MIN_THROTTLE = 0.175f;//0.15f;//0.2f;
	const float m_MAX_ROTATION = 0.790f;
	const float m_TARGET_LOST_TIMEOUT = 6;
	const int m_UPDATE_RATE = 50;

	int m_alignFrames;
	int m_approachFrames;

	float m_throttle;
	float m_rotation;

	int m_frameCount;
	int m_framesLost;

	int m_defaultAlignDirection;

public:

	enum AlignDirection
	{
		NONE = 0,
		CW = 1,
		CCW = 2
	};

	AlignWithTape(float alignTime, float timeoutInSeconds, AlignDirection defaultAlignDirection = AlignDirection::NONE);
	void Initialize();
	void Execute();
	bool IsFinished();
};

#endif
