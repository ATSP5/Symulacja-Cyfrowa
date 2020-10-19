#include "BaseStation.h"

BaseStation::BaseStation()
{
	/*
	Konstruktor inicjalizuje zmienne. U¿ywaæ jako domyœlny
	*/
	cgpk_ = 0;
	question_schedule_ = 0;
	question_period_ = 500;
	transmiting_ = false;
	ack_transmision_ = false;
}

BaseStation::~BaseStation()
{
	/*
	Destruktor czyœci pamiêæ po wszystkich elementach wektora oraz zmiennej wskaŸnikowej
	*/
	package_vect_.clear();
	delete package_;
}

void BaseStation::CreatePackage(unsigned long long int time_of_appear_, int sourcetransmiter_, unsigned long long int _id_, int CGPk_)
{
	/*
	Metoda tworzy nowe pakiety i umieszcza je w wektorze pakietów obiektu stacji bazowej.
	time_of_appear_ - Czas pojawienia siê w systemie, sourcetransmiter_ - numer nadajnika Ÿród³owego, _id_ - unikalny w skali stacji bazowej identyfikator paketu
	*/
	package_ = new Package( time_of_appear_,  sourcetransmiter_,  _id_, CGPk_);
	package_vect_.push_back(package_);
}

vector<Package*> BaseStation::GetPackageVector()
{
	/*
	Metoda dostêpowa do wektora pakietów. Zwraca wektor pakietów.
	*/
	return package_vect_;
}

size_t BaseStation::GetCGPk()
{
	return cgpk_;
}

int BaseStation::GetPTconst()
{
	return kPt_;
}

int BaseStation::GetQuestionPeriod()
{
	return question_period_;
}

bool BaseStation::GetACKTransmission()
{
	return ack_transmision_;
}



void BaseStation::SetQuestionPeriod(int question_)
{
	question_period_ = question_;
}

void BaseStation::SetCGPk(size_t CGPk_time_)
{
	cgpk_ = CGPk_time_;
}

void BaseStation::SetACKTransmission(bool ack_transmision_var)
{
	ack_transmision_ = ack_transmision_var;
}

void BaseStation::EreasePackageFromVectorAtIndex(int index)
{
	package_vect_.erase(this->package_vect_.begin() + index);
}
