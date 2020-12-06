
#include <iostream>
#include <conio.h>
#include <string>
#include "Channel.h"
#include "TelecommunicationSystem.h"
#include <assert.h>
#include <ctime>
#include "PackageGeneration.h"
#include "a5b_start.h"
#include "TransmisionOfPackage.h"
#include "IsChannelFree.h"
#include "StartOfACK.h"
#include "EndOfChannelQuestioning.h"
#include "EndOfACKTransmision.h"
#include "EndOfPackageTransmision.h"
#include "EndOfWaitForTransmision.h"
#include "ApLogger.h"

using namespace std;

void StartUpInteraction(TelecommunicationSystem* telecommunication_sys_, double& intenc_, bool& dbg_mode, int& info_gater_mode_, string& simulation_timing_info_, size_t& start_gathering_stat_clk_s);
void AdvanceSimmulatorsClock(TelecommunicationSystem* telecommunication_sys_, vector < PackageGeneration*>& pg, vector<EndOfChannelQuestioning* >& e_of_chan_quest_, vector<EndOfWaitForTransmision* >& e_of_wait_for_transm_, vector<EndOfPackageTransmision* >& e_of_pcg_transm_vector, vector<EndOfACKTransmision*>& e_of_ack_transm_);


int main()
{
	size_t main_loop_counter_ = 0;
	double package_intencity = 0.1;
	time_t current_win_time_1 = time(NULL);
	time_t current_win_time_2 = 0;
	time_t current_win_time_ = 0;
	time_t begin_time = 0;
	time_t end_time = 0;
	double progress_ = 0.0;
	size_t id_of_package_GV_=0;
	size_t start_gathering_statistics_clock_ = 0;
	string simulation_timing_inf="";
	int samples_mod_div_ = 1;
	srand(time(NULL));
	bool event_triggered = false;
	bool debug_mode_ = false;
	char choise_;

	string f_name_;
    
	int information_gathering_mode = 0;

	TelecommunicationSystem* telecommunication_system_ = new TelecommunicationSystem();

	telecommunication_system_->GetGenerator()->InitializeKernels();

	//Obiekt zdarzenia klasy generujπcej pakiety (Zdarzenie czasowe)
	vector <PackageGeneration*> package_generator_;
	//Obiekty zdarzeÒ klasy IsChannelFree
	vector<IsChannelFree* > is_channel_free_event;
	//Obiekty zdarzeÒ klasy A5B Start (Zdarzenie warunkowe)
	vector<A5B_Start*> a5b_start_;
	//Obiekty zdarzeÒ klasy TransmisionOfPackage
	vector<TransmisionOfPackage* > transmision_of_package_;
	//Obiekty zdarzeÒ klasy StartOfACK
	vector<StartOfACK* > start_of_ack_transmision_;
	//Obiekty zdarzeÒ klasy EndOfChannelQuestioning
	vector<EndOfChannelQuestioning* > end_of_channel_questioning_;
	//Obiekty zdarzeÒ klasy EndofPackageTransmisione
	vector<EndOfPackageTransmision* >end_of_package_transmision_;
	//Obiekty zdarzeÒ klasy EndOfWaitForTransmission
	vector<EndOfWaitForTransmision*> end_of_wait_for_transmision;
	//Obiekty zdarzeÒ klasy EndOfACKTransmision:
	vector<EndOfACKTransmision*> end_of_ACK_transmision_;
	//Inicjalizacja odpowiednich wektorÛw zdarzeÒ:
	for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
	{
		end_of_ACK_transmision_.push_back(new EndOfACKTransmision(i,telecommunication_system_));
		end_of_wait_for_transmision.push_back(new EndOfWaitForTransmision(i));
		end_of_channel_questioning_.push_back(new EndOfChannelQuestioning(i));
	}
	for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
	{
		end_of_package_transmision_.push_back(new EndOfPackageTransmision(i, telecommunication_system_, end_of_ACK_transmision_));
	}
	for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
	{
		is_channel_free_event.push_back(new IsChannelFree(i, telecommunication_system_, end_of_channel_questioning_, end_of_wait_for_transmision));
		transmision_of_package_.push_back(new TransmisionOfPackage(i, telecommunication_system_, end_of_package_transmision_, end_of_channel_questioning_));
		start_of_ack_transmision_.push_back(new StartOfACK(i, telecommunication_system_, end_of_ACK_transmision_));
	}
	for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
	{
		a5b_start_.push_back(new A5B_Start(i, telecommunication_system_, is_channel_free_event, end_of_channel_questioning_));
	}
	//Debug elements section 1

	//End of Debug elements section 1
	StartUpInteraction(telecommunication_system_, package_intencity, debug_mode_, information_gathering_mode, simulation_timing_inf, start_gathering_statistics_clock_);
	ApLogger* ap_logger_ = new ApLogger(telecommunication_system_, information_gathering_mode, start_gathering_statistics_clock_);
	ap_logger_->AddLine("Simulation duration represented in us was set to: "+ to_string(telecommunication_system_->GetUserSetSimmulationTime())+ "\n" + "Intencity of package generation set to: " + to_string(package_intencity)+ "\n");
	ap_logger_->AddLine("Simulation duration was set to " + simulation_timing_inf);
	for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
	{
		package_generator_.push_back(new PackageGeneration(i, telecommunication_system_, package_intencity));
	}
	cout << "Specify saturation samples MODULO divider: " << endl;
	cin >> samples_mod_div_;
	begin_time = time(NULL);
	//START OF SIMULATION PROGRAM
	while (telecommunication_system_->Get_Clock() <= telecommunication_system_->GetSimulationMaxTime() && telecommunication_system_->Get_Clock() <= telecommunication_system_->GetUserSetSimmulationTime())
	{
		main_loop_counter_++;
		do
		{
			event_triggered = false;
			//ANALIZUJ ZDARZENIA CZASOWE: TO DO Zapewnij (assert) øe wszystke zdarzenia czasowe siÍ wykonujπ tzn. ich czas zaplanowania nie jest mniejszy niø zegar symulacji!!!
			//Zdaøenie Generacja Pakietu***************************************************************************************************************************************************
			for (int i = 0; i < 10; i++)
			{
				if (telecommunication_system_->Get_Clock() == package_generator_[i]->GetTimeOfNextEvent())// warunek czasowy
				{
					assert(package_generator_[i]->GetTimeOfNextEvent()>=telecommunication_system_->Get_Clock());
					id_of_package_GV_++;
					package_generator_[i]->Execute(ap_logger_, id_of_package_GV_);
					event_triggered = true;
				}
			}
			//Koniec zdarzenia Generacja Pakietu*******************************************************************************************************************************************
			//Zdarzenie End Of Channel Question********************************************************************************************************************************************
			for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			{
				if (end_of_channel_questioning_[i]->GetTimeOfNextEvent() == telecommunication_system_->Get_Clock())
				{
					if (end_of_channel_questioning_[i]->Execute(ap_logger_) == true)
					{
						is_channel_free_event[i]->SetEventLock(false);
					}
					event_triggered = true;
				}
			}
			//Koniec zdarzenia*************************************************************************************************************************************************************
			//Zdarzenie Koniec oczekiwania na transmisjÍ***********************************************************************************************************************************
			for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			{
				if (end_of_wait_for_transmision[i]->GetTimeOfNextEvent() == telecommunication_system_->Get_Clock())
				{
					if (end_of_wait_for_transmision[i]->Execute(ap_logger_) == true)
					{
						transmision_of_package_[i]->SetEventLock(false);
					}
					event_triggered = true;
				}
			}
			//Koniec zdarzenia*************************************************************************************************************************************************************
			//Zdarzenie koÒca transmisji pakietu*******************************************************************************************************************************************
			for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			{
				if (end_of_package_transmision_[i]->GetTimeOfNextEvent() == telecommunication_system_->Get_Clock())
				{
					if (end_of_package_transmision_[i]->Execute(ap_logger_) == true)
					{
						start_of_ack_transmision_[i]->SetEventLock(false);
					}
					event_triggered = true;
				}
			}
			//Koniec zdarzenia*************************************************************************************************************************************************************
			//Zdarzenie koÒca transmisji ACK***********************************************************************************************************************************************
			for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			{
				if (end_of_ACK_transmision_[i]->GetTimeOfNextEvent() == telecommunication_system_->Get_Clock())
				{
					if (end_of_ACK_transmision_[i]->Execute(ap_logger_) == true)
					{
						a5b_start_[i]->SetEventLock(false);
					}
					event_triggered = true;
				}
			}
			//Koniec zdarzenia*************************************************************************************************************************************************************
			//KONIEC ANALIZY ZDARZE— CZASOWYCH, PRZEJDè DO ANALIZY ZDARZE— WARUNKOWYCH: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			// Zdaøenie A5B START**********************************************************************************************************************************************************
			for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			{
				if (a5b_start_[i]->GetEventLockedStatus() == false && telecommunication_system_->GetBaseStationVector()[i]->GetPackageVector().size() != 0)
				{
					a5b_start_[i]->Execute(ap_logger_);
					event_triggered = true;
				}
			}
			//Koniec obs≥ugi zdarzenia A5B START*******************************************************************************************************************************************
			// Zdarzenie sprawdzenia zajÍtoúci kana≥u***************************************************************************************************************************************
			 for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			 {
				 if (is_channel_free_event[i]->GetEventLockedStatus() == false)
				 {
					 is_channel_free_event[i]->Execute(ap_logger_);
					 event_triggered = true;
					 is_channel_free_event[i]->SetEventLock(true);
				 }
			 }
			//Koniec zdarzenia sprawdzenia zajÍtoúci kana≥u*********************************************************************************************************************************
			//Zdarzenie poczπtku prÛby transmisji*******************************************************************************************************************************************
			for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			{
				if (transmision_of_package_[i]->GetEventLockedStatus() == false)
				{
					transmision_of_package_[i]->Execute(ap_logger_);
					event_triggered = true;
					transmision_of_package_[i]->SetEventLock(true);
				}
			}
			//Koniec zdarzebia poczπtku prÛby transmisji*************************************************************************************************************************************
			//Zdarzenie poczπtku transmisji ACK
			for (int i = 0; i < telecommunication_system_->GetBaseStationVector().size(); i++)
			{
				if (start_of_ack_transmision_[i]->GetEventLockedStatus() == false)
				{
					start_of_ack_transmision_[i]->Execute(ap_logger_);
					event_triggered = true;
					start_of_ack_transmision_[i]->SetEventLock(true);
				}
			}
			//Koniec zdarzenia poczπtku transmisji ACK
			
		} while (event_triggered == true);

		
		if (debug_mode_ == true)// Wyúwietl wynik obrotu pÍtli jeúli debug mode
		{
			ap_logger_->Dysplay();
			_getch();
		}

		if (ap_logger_->GetInformationGatheringMode() == 0 || ap_logger_->GetInformationGatheringMode() == 2)//Dodaj stan systemu do logu jeúli tryb zbierania informacji 0 lub 2
		{
			ap_logger_->AddSystemState();
		}

		ap_logger_->GS_GatherNumberOfPackagesInBSOfEveryMoment();

		telecommunication_system_->GetGenerator()->UpdateKernels();// Sprawdü czy przekroczono 100 000 losowaÒ i jeúli tak wygeneruj nowe kernele zapamiÍtujπc stare (TO DO)

		ap_logger_->AddSaturationInfo(samples_mod_div_,3); //Dodaj informacje o nasyceniu.

		AdvanceSimmulatorsClock(telecommunication_system_,package_generator_, end_of_channel_questioning_,end_of_wait_for_transmision,end_of_package_transmision_,end_of_ACK_transmision_);//ZwiÍksz zegar systemowy

		ap_logger_->StartGatheringStatistics();// Rozpocznij zbieranie statystyk zgodnie z: start_gathering_statistics_clock_

		if (main_loop_counter_ >= ULLONG_MAX - 1)//Nie pozwÛl na przekroczenie zakresu 64 bit. przez main_loop_counter_
		{
			main_loop_counter_ = 0;
		}
		if (main_loop_counter_ % 1000 == 0)//Co 10 00 obrotÛw pÍtli wyúwietl progres symulacji w [%]
		{
			progress_ = (static_cast<double>(telecommunication_system_->Get_Clock()) / static_cast<double>(telecommunication_system_->GetUserSetSimmulationTime()) * 100);
			current_win_time_2 = time(NULL);
			current_win_time_ = current_win_time_2 - current_win_time_1;
			cout << "Simmulation progress: " << static_cast<int>(progress_) << " % " << " Real time of 1000 main loop steps: " << current_win_time_ << " s" << "\r";
			current_win_time_1 = current_win_time_2;
		}
		if (ap_logger_->GetInformationGatheringMode() != 3)
		{
			if (telecommunication_system_->Get_Clock() < telecommunication_system_->GetUserSetSimmulationTime())
			{
				ap_logger_->AddLine("Next Event will occur at system time: " + to_string(telecommunication_system_->Get_Clock()) + "\n");// Dodaj informacjÍ o czasiÍ wystπpienia nastÍpnego zdarzenia.
			}
			else
			{
				ap_logger_->AddLine("Next Event would occur at system time: " + to_string(telecommunication_system_->Get_Clock()) + " but simmulation has finnished. \n");// Dodaj informacjÍ o czasiÍ wystπpienia nastÍpnego zdarzenia.
			}
		}
	}
	//END OF SIMULATION PROGRAM
	end_time = time(NULL);
	cout << "Simmulation progress: " << progress_ << " %" << endl;//Wyúwietl koÒcowy progress symulacji
	cout << "Total simmulation time: " << end_time - begin_time << " [s]" << endl;
	ap_logger_->AddSaturationInfo(samples_mod_div_, 1); //Dodaj informacje o nasyceniu.
	ap_logger_->AddFinalInfo();
	//ap_logger_->AddSaturationInfo(); //Dodaj informacje o nasyceniu pakietami poszczegÛlnych stacji bazowych.
	if (debug_mode_ == false)
	{
		cout << "Your result is ready do you want to s-save it to a file or d-dysplay?" << endl;
		cin >> choise_;
		if (choise_ == 's')
		{
			cout << "Specify log file name: " << endl;
			cin >> f_name_;
			ap_logger_->SaveToFile(f_name_);
			cout << "Specify saturation file name: " << endl;
			cin >> f_name_;
			ap_logger_->SaveToFileSaturationInfo(f_name_);
		}
		else
		{
			ap_logger_->Dysplay();
		}
	}
	system("Pause");
	delete telecommunication_system_;
	delete ap_logger_;
	package_generator_.clear();
	is_channel_free_event.clear();
	a5b_start_.clear();
	transmision_of_package_.clear();
	start_of_ack_transmision_.clear();
	end_of_channel_questioning_.clear();
	end_of_package_transmision_.clear();
	end_of_wait_for_transmision.clear();
	end_of_ACK_transmision_.clear();
	return 0;
}


void StartUpInteraction(TelecommunicationSystem* telecommunication_sys_, double& intenc_, bool& dbg_mode, int& info_gater_mode_, string& simulation_timing_info_, size_t& start_gathering_stat_clk_)
{
	cout << "Before we start you have to tell how long the simmulation period should be." << endl;
	bool good_unit_ = false;
	char unit, db_mode_, info_gm_;
	cout << "First choose unit. s-seconds m-minutes, h-hours, d-days, o-months, y-years?" << endl;
	cin >> unit;
	while (good_unit_ == false)
	{
		switch (unit)
		{
		case 's':
			int16_t seconds_time_;
			cout << "Type number of seconds: " << endl;
			cin >> seconds_time_;
			simulation_timing_info_ = to_string(seconds_time_) + " seconds \n \n";
			telecommunication_sys_->SetSimmulationTime(seconds_time_);
			good_unit_ = true;
			break;
		case 'm':
			size_t minutes_time_;
			cout << "Type number of minutes: " << endl;
			cin >> minutes_time_;
			simulation_timing_info_ = to_string(minutes_time_) + " minutes \n \n";
			telecommunication_sys_->SetSimmulationTime(minutes_time_);
			good_unit_ = true;
			break;
		case 'h':
			uint16_t hours_time_;
			cout << "Type number of hours" << endl;
			cin >> hours_time_;
			simulation_timing_info_ = to_string(hours_time_) + " hours \n \n";
			telecommunication_sys_->SetSimmulationTime(hours_time_);
			good_unit_ = true;
			break;
		case 'd':
			int days_time_;
			cout << "Type number of days" << endl;
			cin >> days_time_;
			simulation_timing_info_ = to_string(days_time_) + " days \n \n";
			telecommunication_sys_->SetSimmulationTime(days_time_);
			good_unit_ = true;
			break;
		case 'o':
			size_t months_time_;
			int num_of_days_;
			cout << "Type number of months" << endl;
			cin >> months_time_;
			cout << "Type number of days in single month" << endl;
			cin >> num_of_days_;
			simulation_timing_info_ = to_string(months_time_) + " months with " + to_string(num_of_days_) + " days in month \n \n";
			telecommunication_sys_->SetSimmulationTime(months_time_, num_of_days_);
			good_unit_ = true;
			break;
		case 'y':
			int num_of_leap_years_;
			int num_of_non_leap_years;
			cout << "Type number of non leap years: " << endl;
			cin >> num_of_non_leap_years;
			cout << "Type number of leap years" << endl;
			cin >> num_of_leap_years_;
			simulation_timing_info_ = to_string(num_of_leap_years_) + " leap years and " + to_string(num_of_non_leap_years) + " non leap years \n \n";
			telecommunication_sys_->SetSimmulationTime(num_of_leap_years_, num_of_non_leap_years);
			good_unit_ = true;
			break;
		default:
			cout << "Wrong unit! Choose unit. m-minutes, h-hours, d-days, o-months, y-years? or q-quit? ";
			cin >> unit;
			if (unit == 'q')
			{
				exit(0);
			}
			break;
		}
	}
	cout << "Choose to enable debug mode: y-yes, n-no." << endl;
	cin >> db_mode_;
	if (db_mode_ == 'y')
	{
		cout << "Debug mode activated. Loop will continue excution on ENTER press." << endl;
		dbg_mode = true;
	}
	else
	{
		cout << "Debug mode deactivated" << endl;
		dbg_mode = false;
	}
	cout << "Set log gathering mode: 0-FULL, 1-EVENTSONLY, 2-SYSTEMSTATE, 3-FINALRESULTONLY Any other number will cause full gathering mode." << endl;
	cin >> info_gm_;
	info_gater_mode_ = atoi(&info_gm_);
	cout << "Set time to start gathering statistics [ms]: " << endl;
	cin >> start_gathering_stat_clk_;
	start_gathering_stat_clk_ = start_gathering_stat_clk_ * 1000;
	cout << "Now set intencity of package appear.: " << endl;
	cin >> intenc_;

}
void AdvanceSimmulatorsClock(TelecommunicationSystem* telecommunication_sys_, vector < PackageGeneration*>& pg, vector<EndOfChannelQuestioning* >& e_of_chan_quest_, vector<EndOfWaitForTransmision* >& e_of_wait_for_transm_, vector<EndOfPackageTransmision* >& e_of_pcg_transm_vector, vector<EndOfACKTransmision*>& e_of_ack_transm_)
{
	size_t timer_ = ULLONG_MAX;
	//Dla kaødego kolejnego zdarzenia czasowego sprawdü czy nieda siÍ znaleüÊ mniejszego czasu!!!

	for (int i = 0; i < 10; i++)
	{
		if (pg[i]->GetTimeOfNextEvent() > telecommunication_sys_->Get_Clock() && timer_ > pg[i]->GetTimeOfNextEvent())
		{
			timer_ = pg[i]->GetTimeOfNextEvent();
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (e_of_chan_quest_[i]->GetTimeOfNextEvent() < timer_ && e_of_chan_quest_[i]->GetTimeOfNextEvent() > telecommunication_sys_->Get_Clock())
		{
			timer_ = e_of_chan_quest_[i]->GetTimeOfNextEvent();
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (e_of_wait_for_transm_[i]->GetTimeOfNextEvent() < timer_ && e_of_wait_for_transm_[i]->GetTimeOfNextEvent() > telecommunication_sys_->Get_Clock())
		{
			timer_ = e_of_wait_for_transm_[i]->GetTimeOfNextEvent();
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (e_of_pcg_transm_vector[i]->GetTimeOfNextEvent() < timer_ && e_of_pcg_transm_vector[i]->GetTimeOfNextEvent() > telecommunication_sys_->Get_Clock())
		{
			timer_ = e_of_pcg_transm_vector[i]->GetTimeOfNextEvent();
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (e_of_ack_transm_[i]->GetTimeOfNextEvent() < timer_ && e_of_ack_transm_[i]->GetTimeOfNextEvent() > telecommunication_sys_->Get_Clock())
		{
			timer_ = e_of_ack_transm_[i]->GetTimeOfNextEvent();
		}
	}
	assert(timer_ != 0);
	telecommunication_sys_->SetTimeClock(timer_);
	//ZwiÍksz czas zegara szczelin czasowych
	telecommunication_sys_->SetChannelClock(telecommunication_sys_->Get_Clock() - telecommunication_sys_->Get_Clock() % 1000 + 1000);
}