#ifndef TELECOMMUNICATIONSYSTEM_H
#define TELECOMMUNICATIONSYSTEM_H
#include <vector>
#include "BaseStation.h"
#include "Channel.h"
#include "Generators.h"

using namespace std;
/*
Klasa opisuj¹ca system telekomunikacyjny. Klasa grupuje w jeden obiekt, obiekty klas vektor stacji bazowych i vektor pakietów w odpowiednich stacjach bazowych.
obiektu klasy nale¿y u¿ywaæ przy ka¿dym przeszukaniu zdarzeñ.
*/
class TelecommunicationSystem
{
private:
	int station_number_; //Zmienna opisuj¹ca numer stacji
	vector<BaseStation*> station_;//Wektor stacji bazowych
	Channel* channel_;//Zmienna wskaŸnikowa na kana³
	size_t clock_;// Zmienna okreœlaj¹ca ogólny czas systemowy
	size_t time_of_simmulation_set;//Zmienna limituj¹ca maksymalny czas symulacji podany przez u¿ytkownika
	size_t channel_clock_;//Zmienna okreœlaj¹ca czas systemowy kolejnych dozwolonych szczelin czasowych
	const int kLr = 7; //Sta³a opisuj¹ca dozwolon¹ liczbê retransmisji ################################################################################ W zadaniu 15!!!!
	const int kNumberOfStation = 10;
	const int kCsc = 1000;//Sta³a okreœlaj¹ca szerokoœæ szczeliny czasowej
	const size_t kSimulationMaxTime = ULLONG_MAX-1; //Wartoœæ 18 446 744 073 709 551 614 Dla jednostki 1us=1 jest to oko³o 593 066,6176 lat.	
	Generators* generator_;
public:
	TelecommunicationSystem();
	~TelecommunicationSystem();
	//Get Methods:
	vector<BaseStation*> GetBaseStationVector();
	Channel* GetChannel();
	Generators* GetGenerator();
	size_t Get_Clock();
	size_t GetSimulationMaxTime();
	size_t GetUserSetSimmulationTime();
	int GetPermitedRetransmisionNumber();
	size_t GetChannelClock();
	//Set Methods:
	void SetSimmulationTime(int16_t seconds);
	void SetChannelClock(size_t tim_);
	void SetSimmulationTime(size_t minutes);
	void SetSimmulationTime(uint16_t hours);
	void SetSimmulationTime(int days);
	void SetSimmulationTime(size_t months, int number_of_days_in_months);
	void SetSimmulationTime(int leap_years, int non_leap_years);
	void SetTimeClock(size_t time_clock);
};

#endif // !TelecommunicationSystem


