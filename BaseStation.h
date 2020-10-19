#ifndef BASESTATION_H
#define BASESTATION_H

#include <vector>
#include "Package.h"
using namespace std;
/*
Klasa stacji bazowych. Opisuje funkcjonalno�� stacji bazowych w systemie. Klasa steruje tak�e algorytmem dost�pu do kana�u.
Uzywa� jej obiekt�w nale�y przez wektor stacji bazowych w obiekcie Telecommunication system.

*/
class BaseStation
{
private:

	vector<Package*> package_vect_;// Wektor pakiet�w 
	Package* package_;// Zmienna wymagana do tworzenia pakietu i uzupe�niania wektora
	int question_period_;// Zmienna opisuj�ca okres odpytywania kana�u czy 1ms czy 0.5ms
	size_t cgpk_;//Zmienna opisuj�ca systemowy czas generacji pakietu
	size_t question_schedule_;// Zmienna opisuj�ca czas systemowy do kolejnego odpytania zaj�to�ci kana�u
	bool transmiting_; //Zmienna boolowska oznaczaj�ca �e dana stacja rozpocz�a transmisj�
	const int kPt_ = 40;// Sta�a opisuj�ca prawdopodobie�stwo w [%] transmisji podczas zaj�cia kana�u i pr�by transmisji
	const int kCtiz_ = 1000;// Sta�a opisuj�ca czas odebrania ACK
	bool ack_transmision_; //Zmienna wskazuj�ca na pocz�tek transmisji ACK
public:
	BaseStation();
	~BaseStation();
	void CreatePackage(unsigned long long int time_of_appear_, int sourcetransmiter_, unsigned long long int _id_, int CTPk_);
	//Get Methods:
	vector<Package*> GetPackageVector();
	size_t GetCGPk();
	int GetPTconst();
	int GetQuestionPeriod();
	bool GetACKTransmission();
	//Set Methods:
	void SetQuestionPeriod(int question_);
	void SetCGPk(size_t CGPk_time_);
	void SetACKTransmission(bool ack_transmision_var);
	void EreasePackageFromVectorAtIndex(int index);
};
#endif // !BaseStation


