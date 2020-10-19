#ifndef BASESTATION_H
#define BASESTATION_H

#include <vector>
#include "Package.h"
using namespace std;
/*
Klasa stacji bazowych. Opisuje funkcjonalnoœæ stacji bazowych w systemie. Klasa steruje tak¿e algorytmem dostêpu do kana³u.
Uzywaæ jej obiektów nale¿y przez wektor stacji bazowych w obiekcie Telecommunication system.

*/
class BaseStation
{
private:

	vector<Package*> package_vect_;// Wektor pakietów 
	Package* package_;// Zmienna wymagana do tworzenia pakietu i uzupe³niania wektora
	int question_period_;// Zmienna opisuj¹ca okres odpytywania kana³u czy 1ms czy 0.5ms
	size_t cgpk_;//Zmienna opisuj¹ca systemowy czas generacji pakietu
	size_t question_schedule_;// Zmienna opisuj¹ca czas systemowy do kolejnego odpytania zajêtoœci kana³u
	bool transmiting_; //Zmienna boolowska oznaczaj¹ca ¿e dana stacja rozpoczê³a transmisjê
	const int kPt_ = 40;// Sta³a opisuj¹ca prawdopodobieñstwo w [%] transmisji podczas zajêcia kana³u i próby transmisji
	const int kCtiz_ = 1000;// Sta³a opisuj¹ca czas odebrania ACK
	bool ack_transmision_; //Zmienna wskazuj¹ca na pocz¹tek transmisji ACK
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


