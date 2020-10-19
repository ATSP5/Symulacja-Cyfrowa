#include "EndOfWaitForTransmision.h"

bool EndOfWaitForTransmision::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("End of wait for transmision. System time: "+ to_string(ap_log_->GetTelecomunicationSystemTime())+"Source of event base station: " + to_string(source_));
	}
	next_event_time_ = 0;//Zablokuj mozliwoœæ wykonania tego zdarzenia po jego wykonaniu 
	return true;// Zezwól na zdarzenie TransmisionOfPackage
}

size_t EndOfWaitForTransmision::GetTimeOfNextEvent()
{
	return next_event_time_;
}

EndOfWaitForTransmision::EndOfWaitForTransmision(int source_V)
{
	source_ = source_V;
	next_event_time_ = 0;
}

void EndOfWaitForTransmision::PlanNextWaitForTransmision(size_t time_)
{
	next_event_time_ = time_;
}

