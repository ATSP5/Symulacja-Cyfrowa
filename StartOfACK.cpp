#include "StartOfACK.h"

void StartOfACK::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("Attempting to start ACK transmision. System clock: " + to_string(telecommunication_system_->Get_Clock()) + "Source of event base station: " + to_string(source_) +"\n");
	}
	if (telecommunication_system_->GetChannel()->GetChannelPackageBuffer().size() == 0)// Gdy brak pakietu w kanale
	{
		ap_log_->AddLine("Debbug ERROR: no package in the channel! \n");
		assert(telecommunication_system_->GetChannel()->GetChannelPackageBuffer().size() != 0);
	}
	else if (telecommunication_system_->GetChannel()->GetChannelPackageBuffer().size() == 1 && telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsCollisionStatus()==false)//Prawid³owa transmisja
	{
		telecommunication_system_->GetGenerator()->IncreaseTERKernelCounter(source_);
		if (telecommunication_system_->GetGenerator()->RndZeroOne(0.8,telecommunication_system_->GetGenerator()->kernel_for_TER_[source_])!=0.0)// Wylosuj b³¹d TER 
		{
			//Ustaw flagê o dobiorze ACK na true:
			telecommunication_system_->GetBaseStationVector()[source_]->SetACKTransmission(true);
			// Zaplanuj koniec transmisji ACK
			end_of_ack_tr_[source_]->PlanNextACKTransmision(telecommunication_system_->Get_Clock() + 1000);
			assert(end_of_ack_tr_[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas jest >= czasowi symulacji
			if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
			{
				ap_log_->AddLine("Starting ACK transmision. <SUCCESS> \n");
			}
			
		}
		else// Powsta³ b³¹d transmisji
		{
			if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
			{
				ap_log_->AddLine("Error of transmision detected! <WARNING> \n");
			}
			ap_log_->IncreaseNumberOfTotalErroneousTransmisionInfo();
			telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);// Ustaw zmienn¹ retransmisji
			telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetIsError(true);//Ustaw wyst¹pienie b³êdu transmisji
			
			//Ustaw flagê o dobiorze ACK na false:
			telecommunication_system_->GetBaseStationVector()[source_]->SetACKTransmission(false);
			// Zaplanuj koniec transmisji ACK
			end_of_ack_tr_[source_]->PlanNextACKTransmision(telecommunication_system_->Get_Clock() + 1000);
			assert(end_of_ack_tr_[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas jest >= czasowi symulacji
			
		}

	}
	else// Powsta³a kolizja
	{
		if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
		{
			ap_log_->AddLine("Collision detected! <WARNING> \n");
		}
		telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);// Ustaw zmienn¹ retransmisji
		telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetIsCollision(true);//Ustaw wyst¹pienie kolizji
		
		//Ustaw flagê o dobiorze ACK na false:
		telecommunication_system_->GetBaseStationVector()[source_]->SetACKTransmission(false);
		// Zaplanuj koniec transmisji ACK
		end_of_ack_tr_[source_]->PlanNextACKTransmision(telecommunication_system_->Get_Clock() + 1000);
		assert(end_of_ack_tr_[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas jest >= czasowi symulacji
	}
}

StartOfACK::StartOfACK(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<EndOfACKTransmision* >& end_of_ack_tr_V)
{
	event_locked_ = true;
	source_ = source_station_;
	telecommunication_system_ = telecommunication_sys_;
	end_of_ack_tr_ = end_of_ack_tr_V;
}

bool StartOfACK::GetEventLockedStatus()
{
	return event_locked_;
}

void StartOfACK::SetEventLock(bool lock_stat_)
{
	event_locked_ = lock_stat_;
}
