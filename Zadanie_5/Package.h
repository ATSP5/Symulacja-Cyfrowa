#ifndef PACKAGE_H
#define PACKAGE_H
#include <string>
using namespace std;
/*
Klasa opisuje pakiet do nadania przez stacjê nadawcz¹. Dostêp do obiektów klasy odbywa siê przez tablicê obiektów stacji nadawczych
obiektu system telekomunikacyjny. Np.: telecommunication_system_->GetBaseStationVector()[0]->CreatePackage(0,0,1);// Utwórz obiekt pakietu dla stacji bazowej nr 0.
*/
class Package
{
private:

	int r_; //Zmienna opisuj¹ca numer aktualnej retransmisji 
	int ctpk_;// Zmienna opisuj¹ca czas trwania transmisji
	int soure_transmiter_;// Zmienna opisuj¹ca z której ze stacji nadawczych wyslany by³ dany pakiet
	int hR_; //zmienna pomocnicza do losowa
	size_t time_of_appear_;// Zmienna okreœlaj¹ca systemowy czas w którym pojawi³ siê pakiet
	size_t time_of_start_of_transmission_;//// Zmienna okreœlaj¹ca systemowy czas w którym rozpoczêto nadawanie
	bool has_time_of_start_transmission_been_set_=false;//Flaga blokuj¹ce wielokrotne zbieranie tej informacji
	size_t time_of_succesful_transmision_;// Zmienna okreœlaj¹ca systemowy czas w którym zakoñczono obs³ugê pakietu
	size_t crp_;//zmienna okreœlaj¹ca czas nastêpnej retransmisji
	size_t id_;//Zmienna okreœlaj¹ca ID pakietu
	bool ret_;//Zmienna okreœlaj¹ca czy pakiet ma byæ retransmitowany czy jest nowy
	bool is_collision_;//zmienna okreœlaj¹ca czy dla danego pakietu wyst¹pi³a kolizja
	bool is_error;//Zmienna okreœlaj¹ca czy dla danego pakietu wyst¹pi³ b³¹d transmisji
	bool has_this_package_encountered_transmission_problem_=false;
public:
	Package(size_t time_of_appear_, int sourcetransmiter_, size_t _id_, int CTPk_);
	~Package();
	//Get Methods:
	size_t GetTimeOfAppear();
	size_t GetID();
	bool GetRetransmisionStatus();
	int GetRetransmision_r_Variable();
	int GetCTPKTime();
	int Get_hR_Variable();
	size_t GetCRPTime();
	bool GetIsCollisionStatus();
	bool GetIsErrorStatus();
	string GetCollisionStatusAsString();
	string GetErrorStatusAsString();
	size_t GetTimeOfSuccesfulTransmission();
	size_t GetTimeOfStartTransmission();
	bool GetPackageTransmissionProblem();
	//Set Methods:
	void SetPackageTransmissionProblem(bool problem_);
	bool SetTimeOfStartTransmission(size_t time_);
	void SetTimeOfSuccesfulTransmission(size_t time_);
	void SetIsCollision(bool col_);
	void SetIsError(bool err_);
	void IncrementR();
	void SetCRPTime(size_t crp_t);
	void Set_hR_Variable(int hr_v);
	void SetRet_var(bool retr_V);
};
#endif // !Package




