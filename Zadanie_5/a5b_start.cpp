#include "a5b_start.h"
#include <math.h>


void A5B_Start::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("A5B algorithm start. System time: " + to_string(telecommunication_system_->Get_Clock()) + "Source of event base station: " + to_string(source_));
	}
	if (event_locked_ == false) //Punkt 1.1 (patrz zad1 v2)
	{
		event_locked_ = true;// Je�li raz wykonano to zda�enie nie wykonuj ponownie a� inne zda�enie go nie odblokuje!!! Trzeba ten warunek te� sprawdza� w g��wnej p�tli!!!
	}
	if (telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetRetransmisionStatus() == false)//Je�li nowy pakiet
	{
		
		telecommunication_system_->GetBaseStationVector()[source_]->SetQuestionPeriod(500);// Punkt 2 zad1 V2
		//Zezw�l na zdarzenie sprawdzenie zaj�to�ci kana�u //
		ischannelfree_ptr_[source_]->SetEventLock(false);
	}
	else // Je�li pakiet do retransmisji
	{
		ap_log_->IncreaseNumberOfTotalRetransmisionInfo();
		
		/*
		Je�li pakiety b��dne powinny by� liczone jako liczba pakiet�w do�wiadczaj�cych b��d TER lub kolizj� (ka�dy pakiet tylko raz) odkomentuj ten kod i zakomentuj lini� 43.
		W przeciwnym razie ten fragment powinien by� nieaktywny.
		if (telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetPackageTransmissionProblem() == false)// Je�li ten pakiet jeszcze nie zaliczy� retransmisji
		{
			ap_log_->GS_IncrementNumberOfPackagesWithTransmissionErrorPerBS(source_);// Policz pakiet jako b��dny.
		}
		*/
		telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetPackageTransmissionProblem(true);// Ustaw �e w danym pakiecie wyst�pi� problem (kolizja lub b��d) [do statystyk]

		telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetIsCollision(false);
		telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetIsError(false);
		telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->IncrementR();
		if (telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetRetransmision_r_Variable() > telecommunication_system_->GetPermitedRetransmisionNumber())// Punkt 3.1
		{
			ap_log_->GS_DecreaseNumberOfRetransmissionPerBS(source_, telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetRetransmision_r_Variable());// w BS source_ zmniejsz liczb� retransmisji o liczb� retransmisji aktualnie usuwanego pakietu
			telecommunication_system_->GetBaseStationVector()[source_]->EreasePackageFromVectorAtIndex(0);//Usu� pakiet z kolejki (ma to by� pierwszy pakiet Index=0)
			ap_log_->IncreaseNumberOfTotalPackagesRemovedAfterMaxRetransmisionInfo();
			ap_log_->GS_IncrementNumberOfPackagesWithTransmissionErrorPerBS(source_);// Policz pakiet jako b��dny, do statystyki pakietowej stopy b��du tylko gdy dany pakiet zostanie odrzucony.
			event_locked_ = false;//Odblokuj START
		}
		else
		{ 
			//Wylosuj nowe CRP
			int hr_to_set = telecommunication_system_->GetGenerator()->Random(0, pow(2.0, static_cast<double>((telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetRetransmision_r_Variable())))-1,telecommunication_system_->GetGenerator()->kernel_for_R_[source_]); 
			//Zwi�ksz licznik losowa� generatora R
			telecommunication_system_->GetGenerator()->IncreaseRKernelConuter(source_);
			//int hr_to_set = telecommunication_system_->GetGenerator()->Random(0, 50, telecommunication_system_->GetGenerator()->kernel_for_R_);
			telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->Set_hR_Variable(hr_to_set);//Ustaw wylosowan� zmienn� R z przedzia�u <0, (2^r)-1>
			telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetCRPTime(1000 * telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetCTPKTime() * telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->Get_hR_Variable());//Ustaw nowe CRP
			telecommunication_system_->GetBaseStationVector()[source_]->SetQuestionPeriod(500);// Punkt 3.2 zad1 V2
			//Zaplanuj koniec oczekiwania na odpytanie kana�u zgodnie z czasem CRP
			end_of_channel_questioning_[source_]->PlanNextChannelQuestioning(telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetCRPTime() + telecommunication_system_->Get_Clock());
			assert(end_of_channel_questioning_[source_]->GetTimeOfNextEvent() >= telecommunication_system_->Get_Clock());//Zapewnij �e zaplanowany czas jest > ni� czas symulacji
			ap_log_->GS_IncrementNumberOfRetransmissionPerBS(source_);
			if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
			{
				ap_log_->AddLine("CRP time set to: " + to_string(telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetCRPTime()) + " Next try planned within system time: " + to_string(telecommunication_system_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetCRPTime() + telecommunication_system_->Get_Clock())+ "\n");
			}
		}
	}
}

A5B_Start::A5B_Start(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<IsChannelFree*> ischannelfree_p_, vector<EndOfChannelQuestioning* >& end_of_channel_questioning_V)
{
	ischannelfree_ptr_ = ischannelfree_p_;
	event_locked_ = false;// Domyslnie to zda�enie jest dozwolone dla wszystkich stacji na pocz�tku
	source_ = source_station_;
	telecommunication_system_ = telecommunication_sys_;
	end_of_channel_questioning_ = end_of_channel_questioning_V;
}

bool A5B_Start::GetEventLockedStatus()
{
	return event_locked_;
}

void A5B_Start::SetEventLock(bool ev_l)
{
	event_locked_ = ev_l;
}
