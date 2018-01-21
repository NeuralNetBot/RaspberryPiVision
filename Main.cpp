#include "OI.h"

int main()
{
	std::shared_ptr<NetworkTable> m_pNetworkTable;
	NetworkTable::SetTeam(3674);
	NetworkTable::SetClientMode();
	NetworkTable::Initialize();
	m_pNetworkTable = NetworkTable::GetTable("vision");
	AligAlignWithTape* tapeAlignment = new AlignWithTape(0.0, 2.5, AlignWithTape::AlignDirection::NONE);
	tapeAlignment->Initialize();
	while(true)
	{
		if(pNetworkTable->GetNumber("start", 0.0) == 1)
		{
			break;
		}
		
	}
	while(!tapeAlignment->isFinished())
	{
		tapeAlignment->Execute();
	}
	system ("sudo shutdown -h now");
	return 0;
	
}