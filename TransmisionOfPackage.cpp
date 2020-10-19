#include "TransmisionOfPackage.h"

void TransmisionOfPackage::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("Starting package transmision. System clock: " + to_string(telecommunication_system_->Get_Clock()) + "Source of event base station: " + to_string(source_) +"\n");
	}
	int pt_ = telecommunication_system_->GetBaseStationVector()[0]->GetPTconst();//Pobierz sta³¹ PT (w ka¿dej stacji ma tak¹ sam¹ wartoœæ.
	int pt_thrown = telecommunication_system_->GetGenerator()->Random(0,100,telecommunication_system_->GetGenerator()->kernel_for_PT_[source_]);
	telecommunication_system_->GetGenerator()->IncreasePTKernelCounter(source_);
	//pt_thrown = 0;//*************************************************************************************************************************To powinno byæ nieaktywne. Tylko do testu!!!
	if (pt_thrown > pt_)//Jeœli wylosowano (1-PT)
	{
		if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
		{
			ap_log_->AddLine("Package sending 1ms delayed! <WARNING> \n");
		}
		telecommunication_system_->GetBaseStationVector()[source_]->SetQuestionPeriod(1000);// Ustaw odpytywanie kana³u na 1ms
		//Zaplanuj koniec oczekiwania na odpytanie kana³u 
		end_of_channel_questioning_[source_]->PlanNextChannelQuestioning(telecommunication_system_->GetBaseStationVector()[source_]->GetQuestionPeriod()+telecommunication_system_->Get_Clock());
		assert(end_of_channel_questioning_[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas >= ni¿ czas symulacji
	}
	else//Jeœli wylosowano (PT)
	{
		if (telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetTimeOfStartTransmission(telecommunication_system_->Get_Clock()) == false)// True tylko jeœli pierwszy raz wywo³ano t¹ funkcjê dla danego pakietu
		{
			ap_log_->GS_AddNewTimeOfStartOfPackageTransmission( telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetTimeOfStartTransmission() - telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetTimeOfAppear());
		}
		if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
		{
			ap_log_->AddLine("Package sending succesfully started. \n");
		}
		telecommunication_system_->GetChannel()->SetChannelStatus(false);// Zajmij kana³
		telecommunication_system_->GetChannel()->CopyPackageToBuffer(telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]);// Skopiuj pakiet z bufora stacji nadawczej do bufora kana³u
		// SprawdŸ kolizjê!!! 
		if (telecommunication_system_->GetChannel()->GetChannelPackageBuffer().size() > 1)
		{
			for (int i = 0; i < telecommunication_system_->GetChannel()->GetChannelPackageBuffer().size(); i++)// Ustaw wyst¹pienie kolizji we wszystkich pakietach w BS których kopie s¹ w kanale
			{
				for (int q = 0; q < 10; q++)
				{
					if(telecommunication_system_->GetBaseStationVector()[q]->GetPackageVector().size()!=0)
					{
						if (telecommunication_system_->GetChannel()->GetChannelPackageBuffer()[i]->GetID() == telecommunication_system_->GetBaseStationVector()[q]->GetPackageVector()[0]->GetID())
						{
							telecommunication_system_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetIsCollision(true);// Ustaw kolizjê
							//tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetRet_var(true);// Ustaw zmienn¹ retransmisji
						}
					}
				}
			}
		}
		//Zaplanuj koniec transmisji pakietu.
		end_of_package_transmision_[source_]->PlanNextWaitForTransmision(telecommunication_system_->Get_Clock()+(telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetCTPKTime()*1000));
		assert(end_of_package_transmision_[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij ¿e zaplanowany czas >= ni¿ czas symulacji
		if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
		{
			ap_log_->AddLine("Sending package. Duration of transfer: "+to_string(telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetCTPKTime() *1000 )+ "Transfer will be finished at system time: "+to_string(end_of_package_transmision_[source_]->GetTimeOfNextEvent())+ "\n ");
		}
	}
}

TransmisionOfPackage::TransmisionOfPackage(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<EndOfPackageTransmision* > &end_of_package_transmision_V, vector<EndOfChannelQuestioning* > &end_of_channel_questioning_V_)
{
	event_locked_ = true;
	source_ = source_station_;
	telecommunication_system_ = telecommunication_sys_;
	end_of_channel_questioning_ = end_of_channel_questioning_V_;
	end_of_package_transmision_ = end_of_package_transmision_V;
}

bool TransmisionOfPackage::GetEventLockedStatus()
{
	return event_locked_;
}

void TransmisionOfPackage::SetEventLock(bool lock_stat_)
{
	event_locked_ = lock_stat_;
}
