#include "EndOfACKTransmision.h"


bool EndOfACKTransmision::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("End of ACK transmision. System clock: " + to_string(tel_sys_->Get_Clock()) + "Source of event base station: " + to_string(source_));
	}
	next_event_time_ = 0;//Zablokuj mozliwoœæ wykonania tego zdarzenia po jego wykonaniu 
	
	//Ostateczne sprawdzenie warunków zakoñczenia przebiegu. Decydujê siê pozostawiæ to bez zmian, poniewa¿ (po konsultacji z dr. Srok¹) 
	//doszed³em do wniosku, ¿e nie ma sensu tworzyæ odrêbnego zdarzenia warunkowego maj¹cego na celu jedynie sprawdzenie czy ostatecznie by³a poprawna transmisja
	//czy nie, szczególnie ¿e nie jestem w stanie zapewniæ ¿e przed tym sprawdzeniem nie wykona siê inne zdarzenie warunkowe. Jest to o tyle istotne, ¿e to zdarzenie
	// jest ostatnim mo¿liwym zdarzeniem w ci¹gu. W celu upo¿¹dkowania zdecydowa³em siê u¿yæ tu metody prywatnej RunFinalCheck, któr¹ mo¿na by traktowaæ jako zdarzenie 
	//warunkowe o natychmiastowym czasie rozpoczêcia.
	RunFinalCheck(ap_log_);

	return true;
}

size_t EndOfACKTransmision::GetTimeOfNextEvent()
{
	return next_event_time_;
}

EndOfACKTransmision::EndOfACKTransmision(int source_V, TelecommunicationSystem* tel_sys__V)
{
	source_ = source_V;
	next_event_time_ = 0;
	tel_sys_ = tel_sys__V;
}

void EndOfACKTransmision::PlanNextACKTransmision(size_t time_)
{
	next_event_time_ = time_;
}

int EndOfACKTransmision::FindPackageByID(size_t id_v_)
{
	if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() != 0)
	{
		for (int i = 0; i < tel_sys_->GetChannel()->GetChannelPackageBuffer().size(); i++)
		{
			if (tel_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetID() == id_v_)
			{
				if (i <= tel_sys_->GetChannel()->GetChannelPackageBuffer().size())
				{
					return i;
				}
				else
				{
					return 0;
				}
			}
		}
		return 0;
	}
	
	return -1;
}

void EndOfACKTransmision::RunFinalCheck(ApLogger* ap_log_)
{
	if (tel_sys_->GetBaseStationVector()[source_]->GetACKTransmission() == true)//Jeœli stacja nadawcza mia³a ustawiony znacznik o nadawaniu ACK
	{
		tel_sys_->GetBaseStationVector()[source_]->SetACKTransmission(false);//Wy³¹cz ten znaczik ACK ze stacji nadawczej
		if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() == 0)//Jeœli bufor kana³u by³ pusty
		{
			// Zapisz do debbugera b³¹d
			ap_log_->AddLine("Debbug ERROR: no package in the channel! \n");
			assert(tel_sys_->GetChannel()->GetChannelPackageBuffer().size() != 0);
		}
		else if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() == 1)// Jeœli w buforze kana³u by³ tylko 1 pakiet
		{
			//Jeœli pakiet mia³ ustawione zmienne ¿e nie by³o b³êdu transmisji ani kolizji
			if (tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsErrorStatus() == false && tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsCollisionStatus() == false)
			{
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetTimeOfSuccesfulTransmission(tel_sys_->Get_Clock());
				ap_log_->GS_AddNewTimeOfSuccesfulPackageTransmission(tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetTimeOfSuccesfulTransmission()- tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetTimeOfAppear(),tel_sys_);
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usuñ pakiet z kana³u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(false);//Ustaw ret_ na false
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana³
				//ap_log_->GS_IncreaseNumberOfRetransmissionPerPackagePerBS(source_, tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetRetransmision_r_Variable());
				ap_log_->GS_IncreaseNumberOfRetransmissionPerPackagePerBS();
				tel_sys_->GetBaseStationVector()[source_]->EreasePackageFromVectorAtIndex(0);//Usuñ pakiet z stacji nadawczej
				tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw ¿e nie wyst¹pi³a kolizja w kanale
				if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
				{
					ap_log_->AddLine("Package sent SUCCESFULLY :) \n");
				}
				ap_log_->IncreaseNumberOfSuccesfullTransmision(source_);
				ap_log_->GS_IncrementNumberOfPackagesCorrectlyRecivedPerBS(source_);// Zwiêksz liczbê poprawnie odebranych pakietów w ka¿dej BS
				
			}
			else if (tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsErrorStatus() == false && tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsCollisionStatus() == true) // Jeœli pakiet mia³ ustawione zmienne ¿e by³a kolizja
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usuñ pakiet z kana³u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana³
				tel_sys_->GetChannel()->SetCollisionIncydent(true);//Ustaw ¿e wyst¹pi³a kolizja w kanale
				ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
			}
			else if (tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsErrorStatus() == true && tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsCollisionStatus() == false) // Jeœli pakiet mia³ ustawione zmienne ¿e by³a kolizja
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usuñ pakiet z kana³u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana³
				tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw ¿e nie wyst¹pi³a kolizja w kanale
			}
			else
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usuñ pakiet z kana³u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana³
				tel_sys_->GetChannel()->SetCollisionIncydent(true);//Ustaw ¿e wyst¹pi³a kolizja w kanale
				ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
			}
		}
		else//Jeœli w buforze kana³u by³o wiêcej ni¿ 1 pakiet
		{

			tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true  
			tel_sys_->GetChannel()->SetCollisionIncydent(true);// Ustaw status kana³u na wyst¹pienie kolizji 
			for (int i = 0; i < tel_sys_->GetChannel()->GetChannelPackageBuffer().size(); i++)// Ustaw wyst¹pienie kolizji we wszystkich pakietach w BS których kopie s¹ w kanale
			{
				for (int q = 0; q < 10; q++)
				{
					if (tel_sys_->GetBaseStationVector()[q]->GetPackageVector().size() != 0)
					{
						if (tel_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetID() == tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->GetID())
						{
							tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetIsCollision(true);// Ustaw kolizjê
							//tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetRet_var(true);// Ustaw zmienn¹ retransmisji
						}
					}
				}
			}
			ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
			int index = FindPackageByID(tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetID());// ZnajdŸ pakiet po jego ID
			assert(index >= 0);
			if (index >= 0)// Jeœli znaleziono
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(index);// Usuñ pakiet z kana³u 
			}
			else//Jeœli nie znaleziono pakietu pomimo ¿e bufor kana³u nie jest pusty!!!
			{
				if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
				{
					ap_log_->AddLine("In the buffer 1 or more package present but FindPackageById returned -1 <ERROR!!!>");
				}
			}
		}
	}
	else//Jeœli stacja nadawcza nie mia³a ustawionego znacznika o nadawaniu ACK
	{
		if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() != 0)// SprawdŸ na pocz¹tku czy kana³ nie jest pusty
		{
			int index = FindPackageByID(tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetID());// ZnajdŸ pakiet po jego ID
			assert(index >= 0);
			if (index >= 0)
			{
				if (tel_sys_->GetChannel()->GetChannelPackageBuffer()[index]->GetIsCollisionStatus() == true)//SPRAWDZIÆ CZY TO ELIMINUJE B£¥D!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				{
					ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
				}
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(index);// Usuñ pakiet z kana³u 
			}
			else//Jeœli nie znaleziono pakietu pomimo ¿e bufor kana³u nie jest pusty!!!
			{
				if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
				{
					ap_log_->AddLine("In the buffer 1 or more package present but FindPackageById returned -1 <ERROR!!!>");
				}
			}

			if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() == 0)// Jeœli kana³ jest pusty 
			{
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana³
				tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw ¿e nie wyst¹pi³a kolizja
			}
			else //Jeœli kana³ ma 1 pakiet lub wiêcej
			{
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetIsCollision(true);//Ustaw w BS ¿e wyst¹pi³a kolizja
				tel_sys_->GetChannel()->SetCollisionIncydent(true);//Ustaw w kanale ¿e wyst¹pi³a kolizja
				for (int i = 0; i < tel_sys_->GetChannel()->GetChannelPackageBuffer().size(); i++)// Ustaw wyst¹pienie kolizji we wszystkich pakietach w BS których kopie s¹ w kanale
				{
					for (int q = 0; q < 10; q++)
					{
						if (tel_sys_->GetBaseStationVector()[q]->GetPackageVector().size() != 0)
						{
							if (tel_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetID() == tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->GetID())
							{
								tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetIsCollision(true);// Ustaw kolizjê
								//tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetRet_var(true);// Ustaw zmienn¹ retransmisji
							}
						}
					}
				}
			}
		}
		else
		{
			tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana³
			tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw ¿e nie wyst¹pi³a kolizja
		}
	}
}
