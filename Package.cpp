#include "Package.h"
#include <stdlib.h>
#include <ctime>
#include <math.h>

Package::Package(size_t time_of_appear_pass_, int sourcetransmiter_, size_t _id_, int CTPk_)
{
	/*
	Konstruktor inicjalizuje zmienne. Nale¿y ustawiæ czas pojawienia siê pakietu w systemie time_of_appear_, numer nadajnika Ÿród³owego sourcetransmiter_,
	oraz numer ID pakietu _id_. 
	*/
	srand(static_cast<unsigned int>(time(NULL)));
	r_ = 0;
	ret_ = false;
	is_collision_ = false;
	is_error = false;
	ctpk_ = CTPk_;
	soure_transmiter_ = sourcetransmiter_;
	time_of_appear_ = time_of_appear_pass_;
	hR_ = 0; /////////////////////////////// hR_ = rand() % static_cast<int>((pow(2.0, static_cast<double>(r_)) - 1)); Struktura gramatyczna do wykorzystania na póŸniej.
	crp_ = r_ * ctpk_;
	id_ = _id_;
}

size_t Package::GetTimeOfAppear()
{
	return time_of_appear_;
}

size_t Package::GetID()
{
	//Metoda zwraca 64'bitowy ID pakietu
	return id_;
}

Package::~Package()
{
}

bool Package::GetRetransmisionStatus()
{
	return ret_;
}

int Package::GetRetransmision_r_Variable()
{
	return r_;
}

int Package::GetCTPKTime()
{
	return ctpk_;
}

int Package::Get_hR_Variable()
{
	return hR_;
}

size_t Package::GetCRPTime()
{
	return crp_;
}

bool Package::GetIsCollisionStatus()
{
	return is_collision_;
}

bool Package::GetIsErrorStatus()
{
	return is_error;
}

string Package::GetCollisionStatusAsString()
{
	if (is_collision_ == true)
	{
		return "Collsion detected! <WARNING!> ";
	}
	else
	{
		return "No collision detected. ";
	}
}

string Package::GetErrorStatusAsString()
{
	if (is_error == true)
	{
		return "Transmission error detected! <WARNING!> ";
	}
	else
	{
		return "No error detected.";
	}
}

size_t Package::GetTimeOfSuccesfulTransmission()
{
	return time_of_succesful_transmision_;
}

size_t Package::GetTimeOfStartTransmission()
{
	return time_of_start_of_transmission_;
}

bool Package::GetPackageTransmissionProblem()
{
	return has_this_package_encountered_transmission_problem_;
}

void Package::SetPackageTransmissionProblem(bool problem_)
{
	has_this_package_encountered_transmission_problem_ = problem_;
}

bool Package::SetTimeOfStartTransmission(size_t time_)
{
	bool status = has_time_of_start_transmission_been_set_;
	if (has_time_of_start_transmission_been_set_ == false)
	{
		time_of_start_of_transmission_ = time_;
	}
	has_time_of_start_transmission_been_set_ = true;
	return status;
}

void Package::SetTimeOfSuccesfulTransmission(size_t time_)
{
	time_of_succesful_transmision_ = time_;
}

void Package::SetIsCollision(bool col_)
{
	is_collision_ = col_;
}

void Package::SetIsError(bool err_)
{
	is_error = err_;
}

void Package::IncrementR()
{
	r_++;
}

void Package::SetCRPTime(size_t crp_t)
{
	crp_ = crp_t;//crp 
}

void Package::Set_hR_Variable(int hr_v)
{
	hR_ = hr_v;
}

void Package::SetRet_var(bool retr_V)
{
	ret_ = retr_V;
}
