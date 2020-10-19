#ifndef APLOGGER_H
#define APLOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include "TelecommunicationSystem.h"
using namespace std;
class ApLogger
{

public:
	enum InformationGatheringMode { FULL=0, EVENTSONLY, SYSTEMSTATE, FINALRESULTONLY};
	ApLogger(TelecommunicationSystem* t_s_, int info_mode_, size_t GS_start_clk_);
	void SaveToFile(string file_name_);
	void SaveToFileSaturationInfo(string file_name_);
	void Dysplay();
	void AddLine(string txt_);
	void AddSystemState();
	int GetInformationGatheringMode();
	void AddFinalInfo();
	void AddSaturationInfo(int samples_, short int mode);
	void IncreaseNumberOfTotalRetransmisionInfo();
	void IncreaseNumberOfTotalErroneousTransmisionInfo();
	void IncreaseNumberOfTotalPackagesEncounteringCollisionInfo();
	void IncreaseNumberOfTotalPackagesRemovedAfterMaxRetransmisionInfo();
	void IncreaseNumberOfSuccesfullTransmision(int bs_);
	void IncreaseNumberOfTotalCreatedPackages();
	size_t GetTelecomunicationSystemTime();
	//Methods for gathering statistics:
	void StartGatheringStatistics();// Wydaje zgodê na zbieranie statystyk
	bool GetGatheringStatisticsPermissionStatus();//Zwraca flagê zgody na zbieranie zstatystyk
	void GS_IncreseNumberOfPackagesGenerated();//Zwiêksz liczbê pakietów wygenerowanych od chwili zbierania statystyk
	void GS_GatherNumberOfPackagesInBSOfEveryMoment();//Zbierz (w danym momencie czasowym) liczbê pakietów w ka¿dej stacji nadawczej
	void GS_IncrementNumberOfRetransmissionPerBS(int bs_);//Zwiêksz liczbê zanotowanych retransmisji o 1 dla bs_
	void GS_DecreaseNumberOfRetransmissionPerBS(int bs_, int number_to_erease_);//Zmniejsz  liczbê zanotowanych retransmisji o number_to_erease_ dla bs_
	void GS_AddNewTimeOfSuccesfulPackageTransmission(size_t time_, TelecommunicationSystem* t_s_);
	double GS_CalculateAverageTimeOfSuccesfulPackageTransmission(vector<size_t>& time_);
	void GS_AddNewTimeOfStartOfPackageTransmission(size_t time_);
	double GS_CalculateAverageTimeOfStartOfPackageTransmission(vector<size_t>& time_);
	void GS_IncrementNumberOfPackagesWithTransmissionErrorPerBS(int sourceBS_);
	void GS_IncrementNumberOfPackagesCorrectlyRecivedPerBS(int sourceBS_);
	double GS_CalcualteAveragePackageErrorRatio();
	double GS_CalculateMaxPackageErrorRatio();
	void GS_IncreaseNumberOfRetransmissionPerPackagePerBS(int station, int retransmission);
	void GS_IncreaseNumberOfRetransmissionPerPackagePerBS();
private:
	double CountRetransmissionAverage();
	size_t number_of_total_created_packages_;
	size_t number_of_final_retransmision_info_;
	size_t number_of_erroneous_transmision_info_;
	size_t number_of_packages_encountering_collision_info_;
	size_t number_of_packages_removed_after_max_retransmision_info_;
	size_t number_of_succesful_transmision_[10];
	ofstream log_file_;
	ofstream saturation_file_;
	string log_;
	string saturation_;
	string final_info_;
	TelecommunicationSystem* t_sys_;
	InformationGatheringMode info_gathering_mode_;
	size_t sample_number_ = 0;
	//Variables for gathering statistics:
	vector<int> GS_number_of_package_in_bs_1;
	vector<int> GS_number_of_package_in_bs_2;
	vector<int> GS_number_of_package_in_bs_3;
	vector<int> GS_number_of_package_in_bs_4;
	vector<int> GS_number_of_package_in_bs_5;
	vector<int> GS_number_of_package_in_bs_6;
	vector<int> GS_number_of_package_in_bs_7;
	vector<int> GS_number_of_package_in_bs_8;
	vector<int> GS_number_of_package_in_bs_9;
	vector<int> GS_number_of_package_in_bs_10;
	bool gathering_statistics_permission_;// Zgoda na zbieranie statystyk
	size_t GS_number_of_packages_generated_;// Liczba wygenerowanych pakietów (do statystyki) 
	size_t GS_time_of_start_gathering_stat;//Czas pocz¹tku zbierania statystyk
	int GS_NumberOfRetransmissionPerBS_[10] = {0,0,0,0,0,0,0,0,0,0};// Liczba retransmisji numer indeksu tablicy = numer stacji bazowej
	vector<size_t> time_of_succesful_package_transmission_vector_;// Wektor liczb (czasów) ró¿nic miêdzy czasem pojawienia siê w systemie pakietu a jego czasem poprawnego odebrania
	vector<size_t> time_of_start_of_package_transmission_vector_;// Wektor liczb (czasów) ró¿nic miêdzy czasem pojawienia siê w systemie pakietu a jego czasem rozpoczêcia nadawania
	vector<size_t> system_clock_of_end_package_transmission_vector_;//Wektor czasów systemowych dla time_of_end_of_package_transmission_vector_
	vector<size_t> system_clock_of_retransmission_;//Wektor czasów retransmisji
	size_t GS_NumberOfPackagesWithTransmissionError_[10] = { 0,0,0,0,0,0,0,0,0,0 };// Liczba b³êdnych pakietów
	size_t GS_NumberOfPackagesCorrectlyRecivedPerBS[10] = { 0,0,0,0,0,0,0,0,0,0 };// Liczba poprawnych pakietów
	vector<vector<size_t>> GS_NumberOfPackageRetransmissionPerBS_ { {},{},{},{},{},{},{},{},{},{} };// Liczba rentransmisji na pakiet i na stacjê bazow¹
};

#endif // !APLOGGER_H
