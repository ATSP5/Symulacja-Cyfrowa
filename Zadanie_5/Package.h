#ifndef PACKAGE_H
#define PACKAGE_H
#include <string>
using namespace std;
/*
Klasa opisuje pakiet do nadania przez stacj� nadawcz�. Dost�p do obiekt�w klasy odbywa si� przez tablic� obiekt�w stacji nadawczych
obiektu system telekomunikacyjny. Np.: telecommunication_system_->GetBaseStationVector()[0]->CreatePackage(0,0,1);// Utw�rz obiekt pakietu dla stacji bazowej nr 0.
*/
class Package
{
private:

	int r_; //Zmienna opisuj�ca numer aktualnej retransmisji 
	int ctpk_;// Zmienna opisuj�ca czas trwania transmisji
	int soure_transmiter_;// Zmienna opisuj�ca z kt�rej ze stacji nadawczych wyslany by� dany pakiet
	int hR_; //zmienna pomocnicza do losowa
	size_t time_of_appear_;// Zmienna okre�laj�ca systemowy czas w kt�rym pojawi� si� pakiet
	size_t time_of_start_of_transmission_;//// Zmienna okre�laj�ca systemowy czas w kt�rym rozpocz�to nadawanie
	bool has_time_of_start_transmission_been_set_=false;//Flaga blokuj�ce wielokrotne zbieranie tej informacji
	size_t time_of_succesful_transmision_;// Zmienna okre�laj�ca systemowy czas w kt�rym zako�czono obs�ug� pakietu
	size_t crp_;//zmienna okre�laj�ca czas nast�pnej retransmisji
	size_t id_;//Zmienna okre�laj�ca ID pakietu
	bool ret_;//Zmienna okre�laj�ca czy pakiet ma by� retransmitowany czy jest nowy
	bool is_collision_;//zmienna okre�laj�ca czy dla danego pakietu wyst�pi�a kolizja
	bool is_error;//Zmienna okre�laj�ca czy dla danego pakietu wyst�pi� b��d transmisji
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




