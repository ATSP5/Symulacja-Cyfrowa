#include "TelecommunicationSystem.h"

TelecommunicationSystem::TelecommunicationSystem()
{
	/*
	Konstruktor inicjalizuje zmienne, w tym wektor stacji bazowych oraz kana³. U¿ywaæ jako domyœlny
	*/
	channel_ = new Channel();
	station_number_ = 0;
	for (int i = 0; i < kNumberOfStation; i++)
	{
		
		station_.push_back(new BaseStation());
	}
	clock_ = 1;
	channel_clock_ = 0;
	time_of_simmulation_set = 0;
	generator_ = new Generators();
}

TelecommunicationSystem::~TelecommunicationSystem()
{
	/*
	Destruktor obiektu systemu telekomunikacyjnego. Usuwa elementy wektora oraz usuwa wskaŸnik.
	*/
	station_.clear();	
	delete generator_;
	delete channel_;
}

vector<BaseStation*> TelecommunicationSystem::GetBaseStationVector()
{
	/*
	Funkcja dostêpu do wektora stacji nadawczej u¿ywaæ jak funkcji zwracaj¹cej tablice 
	Np: telecommunication_system_->GetBaseStationVector()[0]->CreatePackage(0,0,1);
	*/
	return station_;
}

Channel* TelecommunicationSystem::GetChannel()
{
	return channel_;
}

Generators* TelecommunicationSystem::GetGenerator()
{
	return generator_;
}

size_t TelecommunicationSystem::Get_Clock()
{
	return clock_;
}

size_t TelecommunicationSystem::GetSimulationMaxTime()
{
	return kSimulationMaxTime;
}

size_t TelecommunicationSystem::GetUserSetSimmulationTime()
{
	return time_of_simmulation_set;
}

int TelecommunicationSystem::GetPermitedRetransmisionNumber()
{
	return kLr;
}

size_t TelecommunicationSystem::GetChannelClock()
{
	return channel_clock_;
}

void TelecommunicationSystem::SetSimmulationTime(int16_t seconds)
{
	time_of_simmulation_set = seconds * 1000 * 1000;
}

void TelecommunicationSystem::SetChannelClock(size_t tim_)
{
	channel_clock_ = tim_;
}

void TelecommunicationSystem::SetSimmulationTime(size_t minutes)
{
	time_of_simmulation_set = minutes * 60 * 1000 * 1000;
}
void TelecommunicationSystem::SetSimmulationTime(uint16_t hours)
{
	time_of_simmulation_set = static_cast<size_t>(hours) * 60 * 60 * 1000 * 1000;
}
void TelecommunicationSystem::SetSimmulationTime(int days)
{
	time_of_simmulation_set = static_cast<size_t>(days)*24 * 60 * 60 * 1000 * 1000;
}
void TelecommunicationSystem::SetSimmulationTime(size_t months, int number_of_days_in_months)
{
	time_of_simmulation_set = static_cast<size_t>(months)* static_cast<size_t>(number_of_days_in_months) * 24 * 60 * 60 * 1000 * 1000;
}

void TelecommunicationSystem::SetSimmulationTime(int leap_years, int non_leap_years)
{
	time_of_simmulation_set = static_cast<size_t>(leap_years) * 24 * 60 * 60 * 1000 * 1000*366+ static_cast<size_t>(non_leap_years) * 24 * 60 * 60 * 1000 * 1000 * 366;
}

void TelecommunicationSystem::SetTimeClock(size_t time_clock)
{
	clock_ = time_clock;
}

