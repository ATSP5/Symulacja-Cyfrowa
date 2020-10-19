#include "EndOfPackageTransmision.h"

bool EndOfPackageTransmision::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("End of package transmision. System clock: " + to_string(tel_sys_->Get_Clock()) + "Source of event base station: " + to_string(source_));
	}
	next_event_time_ = 0;//Zablokuj mozliwoœæ wykonania tego zdarzenia po jego wykonaniu 

	//Zezwól na wyst¹pienie zdarzenia pocz¹tek transmisji ACK:
	return true;
}

size_t EndOfPackageTransmision::GetTimeOfNextEvent()
{
	return next_event_time_;
}

EndOfPackageTransmision::EndOfPackageTransmision(int source_V, TelecommunicationSystem* tel_sys__V, vector<EndOfACKTransmision*>& end_of_ACK_transm_V)
{
	source_ = source_V;
	end_of_ACK_transm_ = end_of_ACK_transm_V;
	tel_sys_ = tel_sys__V;
}

void EndOfPackageTransmision::PlanNextWaitForTransmision(size_t time_)
{
	next_event_time_ = time_;
	
}
