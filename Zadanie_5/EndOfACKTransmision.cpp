#include "EndOfACKTransmision.h"


bool EndOfACKTransmision::Execute(ApLogger* ap_log_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("End of ACK transmision. System clock: " + to_string(tel_sys_->Get_Clock()) + "Source of event base station: " + to_string(source_));
	}
	next_event_time_ = 0;//Zablokuj mozliwo�� wykonania tego zdarzenia po jego wykonaniu 
	
	//Ostateczne sprawdzenie warunk�w zako�czenia przebiegu. Decyduj� si� pozostawi� to bez zmian, poniewa� (po konsultacji z dr. Srok�) 
	//doszed�em do wniosku, �e nie ma sensu tworzy� odr�bnego zdarzenia warunkowego maj�cego na celu jedynie sprawdzenie czy ostatecznie by�a poprawna transmisja
	//czy nie, szczeg�lnie �e nie jestem w stanie zapewni� �e przed tym sprawdzeniem nie wykona si� inne zdarzenie warunkowe. Jest to o tyle istotne, �e to zdarzenie
	// jest ostatnim mo�liwym zdarzeniem w ci�gu. W celu upo��dkowania zdecydowa�em si� u�y� tu metody prywatnej RunFinalCheck, kt�r� mo�na by traktowa� jako zdarzenie 
	//warunkowe o natychmiastowym czasie rozpocz�cia.
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
	if (tel_sys_->GetBaseStationVector()[source_]->GetACKTransmission() == true)//Je�li stacja nadawcza mia�a ustawiony znacznik o nadawaniu ACK
	{
		tel_sys_->GetBaseStationVector()[source_]->SetACKTransmission(false);//Wy��cz ten znaczik ACK ze stacji nadawczej
		if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() == 0)//Je�li bufor kana�u by� pusty
		{
			// Zapisz do debbugera b��d
			ap_log_->AddLine("Debbug ERROR: no package in the channel! \n");
			assert(tel_sys_->GetChannel()->GetChannelPackageBuffer().size() != 0);
		}
		else if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() == 1)// Je�li w buforze kana�u by� tylko 1 pakiet
		{
			//Je�li pakiet mia� ustawione zmienne �e nie by�o b��du transmisji ani kolizji
			if (tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsErrorStatus() == false && tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsCollisionStatus() == false)
			{
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetTimeOfSuccesfulTransmission(tel_sys_->Get_Clock());
				ap_log_->GS_AddNewTimeOfSuccesfulPackageTransmission(tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetTimeOfSuccesfulTransmission()- tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetTimeOfAppear(),tel_sys_);
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usu� pakiet z kana�u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(false);//Ustaw ret_ na false
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana�
				//ap_log_->GS_IncreaseNumberOfRetransmissionPerPackagePerBS(source_, tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetRetransmision_r_Variable());
				ap_log_->GS_IncreaseNumberOfRetransmissionPerPackagePerBS();
				tel_sys_->GetBaseStationVector()[source_]->EreasePackageFromVectorAtIndex(0);//Usu� pakiet z stacji nadawczej
				tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw �e nie wyst�pi�a kolizja w kanale
				if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
				{
					ap_log_->AddLine("Package sent SUCCESFULLY :) \n");
				}
				ap_log_->IncreaseNumberOfSuccesfullTransmision(source_);
				ap_log_->GS_IncrementNumberOfPackagesCorrectlyRecivedPerBS(source_);// Zwi�ksz liczb� poprawnie odebranych pakiet�w w ka�dej BS
				
			}
			else if (tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsErrorStatus() == false && tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsCollisionStatus() == true) // Je�li pakiet mia� ustawione zmienne �e by�a kolizja
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usu� pakiet z kana�u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana�
				tel_sys_->GetChannel()->SetCollisionIncydent(true);//Ustaw �e wyst�pi�a kolizja w kanale
				ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
			}
			else if (tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsErrorStatus() == true && tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetIsCollisionStatus() == false) // Je�li pakiet mia� ustawione zmienne �e by�a kolizja
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usu� pakiet z kana�u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana�
				tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw �e nie wyst�pi�a kolizja w kanale
			}
			else
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(0);// Usu� pakiet z kana�u 
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana�
				tel_sys_->GetChannel()->SetCollisionIncydent(true);//Ustaw �e wyst�pi�a kolizja w kanale
				ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
			}
		}
		else//Je�li w buforze kana�u by�o wi�cej ni� 1 pakiet
		{

			tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true  
			tel_sys_->GetChannel()->SetCollisionIncydent(true);// Ustaw status kana�u na wyst�pienie kolizji 
			for (int i = 0; i < tel_sys_->GetChannel()->GetChannelPackageBuffer().size(); i++)// Ustaw wyst�pienie kolizji we wszystkich pakietach w BS kt�rych kopie s� w kanale
			{
				for (int q = 0; q < 10; q++)
				{
					if (tel_sys_->GetBaseStationVector()[q]->GetPackageVector().size() != 0)
					{
						if (tel_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetID() == tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->GetID())
						{
							tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetIsCollision(true);// Ustaw kolizj�
							//tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetRet_var(true);// Ustaw zmienn� retransmisji
						}
					}
				}
			}
			ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
			int index = FindPackageByID(tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetID());// Znajd� pakiet po jego ID
			assert(index >= 0);
			if (index >= 0)// Je�li znaleziono
			{
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(index);// Usu� pakiet z kana�u 
			}
			else//Je�li nie znaleziono pakietu pomimo �e bufor kana�u nie jest pusty!!!
			{
				if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
				{
					ap_log_->AddLine("In the buffer 1 or more package present but FindPackageById returned -1 <ERROR!!!>");
				}
			}
		}
	}
	else//Je�li stacja nadawcza nie mia�a ustawionego znacznika o nadawaniu ACK
	{
		if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() != 0)// Sprawd� na pocz�tku czy kana� nie jest pusty
		{
			int index = FindPackageByID(tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->GetID());// Znajd� pakiet po jego ID
			assert(index >= 0);
			if (index >= 0)
			{
				if (tel_sys_->GetChannel()->GetChannelPackageBuffer()[index]->GetIsCollisionStatus() == true)//SPRAWDZI� CZY TO ELIMINUJE B��D!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				{
					ap_log_->IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
				}
				tel_sys_->GetChannel()->EreasePackageFromBufferAtIndex(index);// Usu� pakiet z kana�u 
			}
			else//Je�li nie znaleziono pakietu pomimo �e bufor kana�u nie jest pusty!!!
			{
				if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
				{
					ap_log_->AddLine("In the buffer 1 or more package present but FindPackageById returned -1 <ERROR!!!>");
				}
			}

			if (tel_sys_->GetChannel()->GetChannelPackageBuffer().size() == 0)// Je�li kana� jest pusty 
			{
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana�
				tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw �e nie wyst�pi�a kolizja
			}
			else //Je�li kana� ma 1 pakiet lub wi�cej
			{
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetRet_var(true);//Ustaw ret_ na true
				tel_sys_->GetBaseStationVector()[source_]->GetPackageVector()[0]->SetIsCollision(true);//Ustaw w BS �e wyst�pi�a kolizja
				tel_sys_->GetChannel()->SetCollisionIncydent(true);//Ustaw w kanale �e wyst�pi�a kolizja
				for (int i = 0; i < tel_sys_->GetChannel()->GetChannelPackageBuffer().size(); i++)// Ustaw wyst�pienie kolizji we wszystkich pakietach w BS kt�rych kopie s� w kanale
				{
					for (int q = 0; q < 10; q++)
					{
						if (tel_sys_->GetBaseStationVector()[q]->GetPackageVector().size() != 0)
						{
							if (tel_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetID() == tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->GetID())
							{
								tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetIsCollision(true);// Ustaw kolizj�
								//tel_sys_->GetBaseStationVector()[q]->GetPackageVector()[0]->SetRet_var(true);// Ustaw zmienn� retransmisji
							}
						}
					}
				}
			}
		}
		else
		{
			tel_sys_->GetChannel()->SetChannelStatus(true);// Zwolnij kana�
			tel_sys_->GetChannel()->SetCollisionIncydent(false);//Ustaw �e nie wyst�pi�a kolizja
		}
	}
}
