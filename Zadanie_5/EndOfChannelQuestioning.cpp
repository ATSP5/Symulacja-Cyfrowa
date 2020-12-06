#include "EndOfChannelQuestioning.h"


bool EndOfChannelQuestioning::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("End of channel questioning. System time: " + to_string(ap_log_->GetTelecomunicationSystemTime())+ "Source of event base station: " + to_string(source_));
	}
	next_event_time_ = 0;//Zablokuj mozliwoœæ wykonania tego zdarzenia po jego wykonaniu 
	//Zezwól na zdarzenie Sprawdzenie zajêtoœci kana³u
	return true;
}

size_t EndOfChannelQuestioning::GetTimeOfNextEvent()
{
	return next_event_time_;
}

EndOfChannelQuestioning::EndOfChannelQuestioning(int source_V)
{
	next_event_time_ = 0;
	source_ = source_V;
}

void EndOfChannelQuestioning::PlanNextChannelQuestioning(size_t time_)
{
	next_event_time_ = time_;
}
