#include "IsChannelFree.h"

void IsChannelFree::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("Checking if channel is free. System clock: " + to_string(telecommunication_system_->Get_Clock()) + "Source of event base station: " + to_string(source_));
	}
	if (telecommunication_system_->GetChannel()->GetChannelStatus() == true)
	{
		//Zaplanuj koniec oczekiwania na transmisjê pakietu zgodnie z dozwolonymi szczelinami czasowymi
		//if (telecommunication_system_->Get_Clock() < telecommunication_system_->GetChannelClock())
		if(telecommunication_system_->Get_Clock()%1000==0)
		{
			end_of_wait_for_transmision[source_]->PlanNextWaitForTransmision(telecommunication_system_->Get_Clock());// Najbli¿sza szczelina czasowa (natychmiastowa)
			assert(end_of_wait_for_transmision[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas jest > ni¿ czas symulacji
		}
		else
		{
			end_of_wait_for_transmision[source_]->PlanNextWaitForTransmision(telecommunication_system_->GetChannelClock());//Kolejna szczelina czasowa
			assert(end_of_wait_for_transmision[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas jest > ni¿ czas symulacji
		}
		
	}
	else
	{
		//Zaplanuj koniec odpytywania kana³u zgodnie ze zmienn¹ question_schedule_
		end_of_channel_questioning_[source_]->PlanNextChannelQuestioning(telecommunication_system_->GetBaseStationVector()[source_]->GetQuestionPeriod() + telecommunication_system_->Get_Clock());
		assert(end_of_channel_questioning_[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas jest > ni¿ czas symulacji
	}
}

IsChannelFree::IsChannelFree(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<EndOfChannelQuestioning*>& end_of_channel_questioning_V, vector<EndOfWaitForTransmision*>& end_of_wait_for_transmision_V)
{
	source_ = source_station_;
	telecommunication_system_ = telecommunication_sys_;
	end_of_channel_questioning_ = end_of_channel_questioning_V;
	end_of_wait_for_transmision = end_of_wait_for_transmision_V;
	event_locked_ = true;
}

bool IsChannelFree::GetEventLockedStatus()
{
	return event_locked_;
}

void IsChannelFree::SetEventLock(bool lock_)
{
	event_locked_ = lock_;
}
