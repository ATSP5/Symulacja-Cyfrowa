# Symulacja-Cyfrowa


Projekt. Symulacja cyfrowa.

















Symulacja cyfrowa sieci bezprzewodowej metodą przeglądania zdarzeń. Zadanie nr 21.

Adam Prukała. ESPIO. 116225.


Spis treści
Projekt. Symulacja cyfrowa.	1
2. Treść zadania.	3
3. Opis modelu symulacyjnego:	4
3. Opis przydzielonej metody symulacyjnej:	7
4. Parametry wywołania programu.	10
5. Generatory.	11
6. Opis metody testowania i weryfikacji poprawności działania programu.	16
7. Wyniki symulacji.	17
8. Wnioski.	21



















2. Treść zadania.
	W sieci bezprzewodowej stacje nadawcze konkurują o dostęp do łącza. W losowych odstępach czasu CGPk k-ta stacja nadawcza generuje pakiety gotowe do wysłania. Po uzyskaniu dostępu do łącza zgodnie z algorytmem A, k-ty terminal podejmuje próbę transmisji najstarszego pakietu ze swojego bufora. Czas transmisji wiadomości z k-tej stacji nadawczej do k-tej stacji odbiorczej wynosi CTPk. Jeśli transmisja pakietu zakończyła się sukcesem, stacja odbiorcza przesyła potwierdzenie ACK (ang. Acknowledgment) poprawnego odebrania wiadomości. Czas transmisji ACK wynosi CTIZ. Jeśli transmisja pakietu nie powiodła się, stacja odbiorcza nie przesyła ACK. Odbiór pakietu uznajemy za niepoprawny, jeśli w kanale transmisyjnym wystąpiła kolizja lub błąd. Przez kolizję rozumiemy nałożenie się jakiejkolwiek części jednego pakietu na inny pakiet (pochodzący z innego nadajnika). Dodatkowo każda transmisja pakietu może zakończyć się błędem TER. Brak wiadomości ACK po czasie (CTPk + CTIZ) od wysłania pakietu jest dla stacji nadawczej sygnałem o konieczności retransmisji pakietu. Każdy pakiet może być retransmitowany maksymalnie LR razy. Dostęp do łącza w przypadku retransmisji opiera sie na tych samych zasadach co transmisja pierwotna. Jeśli mimo LR-krotnej próby retransmisji pakietu nie udało się poprawnie odebrać, wówczas stacja nadawcza odrzuca pakiet i - jeśli jej bufor nie jest pusty - przystępuje do próby transmisji kolejnego pakietu.
Opracuj symulator sieci bezprzewodowej zgodnie z metodą M.
Za pomocą symulacji wyznacz:
- Wartość parametru L, która zapewni średnią pakietową stopę błędów (uśrednioną po K odbiornikach) nie większą niż 0.1, a następnie:
-Pakietową stopę błędów w każdym z odbiorników mierzoną jako iloraz liczby pakietów straconych do liczby przesłanych pakietów.
-Średnią liczbę retransmisji pakietów. 
-Przepływność systemu mierzoną liczbą poprawnie odebranych pakietów w jednostce czasu. 
-Średnie opóźnienie pakietu, tzn. czas jaki upływa między pojawieniem się pakietu w buforze a jego poprawnym odebraniem.
-Średni czas oczekiwania, tzn. czas między pojawieniem się pakietu w buforze, a jego opuszczeniem.
-Sporządź wykres zależności średniej liczby retransmisji pakietów od parametru P.
Sporządź wykres zależności przepływności systemu oraz średniej i maksymalnej pakietowej stopy błędów w zależności od wartości L.
Parametry:
CKPk - zmienna losowa o rozkładzie wykładniczym i intensywności L.
CTPk - zmienna losowa o rozkładzie jednostajnym w przedziale {1, 2, ..., 10} ms
R - zmienna losowa o rozkładzie jednostajnym w przedziale <0, (2^r-1)>
T  - zmienna losowa o rozkładzie jednostajnym w przedziale <0, (2^t-1)>
TER - zmienna losowa o rozkładzie zero-jedynkowym z prawdopodobieństwem sukcesu P
CTIZ = 1ms
CSC = 1ms
P =0.8.
3. Opis modelu symulacyjnego:

a. Schemat modelu symulacyjnego:
 
Rysunek 1 Schemat modelu symulacyjnego.
b. Opis klas wchodzących w skład systemu i ich atrybutów:
Obiekt	Nazwa klasy implementującej obiekt	Opis	Atrybuty
System
telekomunikacyjny	TelecomunicationSystem	Klasa gromadząca wszystkie elementy systemu	-kLr stała typu const int opisująca liczbę dozwolonych retransmisji
-station_number_ zmienna opisująca numer stacji 
- wskaźnik na wektor stacji nadawczych typu vector<BaseStation*>
station_ przechowujący tablicę stacji nadawczych
-clock_ zmienna typu unsigned long long int określająca ogólny czas systemowy
-channel_clock_ zmienna typu unsigned long long int określająca czas kolejnych dozwolonych szczelin czasowych w kanale
-kCsc stała typu const int opisująca szerokość szczeliny
-kSimulationMaxTime stała typu const unsigned long long int limitująca czas trwania symulacji
-kNumberOfStation- stała typu int opisująca liczbę stacji nadawczych w systemie
Stacja nadawcza	BaseStation	Klasa reprezentująca stację nadawczą. Generująca w swoim buforze pakiet w odsępach czasu CGPk . Stacja realizuje algorytm dostępu A5b. Nasłuchuje komunikatu ACK i ewentualnie retransmituje swój pakiet. Jeśli nie udało się odebrać poprawnie pakietu, stacja usuwa go ze swojego bufora.	-wskaźnik na wektor pakietów typu vector<Package*> package_vect_ przechowujący tablicę pakietów do nadania
- kCtiz_ stała typu const int opisująca interwał czasu odebrania ACK. 
-cgpk_ Zmienna typu unsigned long long int opisująca czas generacji pakietu
-kPt_stała opisująca prawdopodobieństwo transmisji w szczelinie czasowej
-transmiting_ Zmienna boolowska wskazująca czy dana stacja rozpoczęła transmisję
-question_schedule_ Zmienna typu unsigned long long int opisująca czas systemowy do kolejnego odpytania zajętości kanału
-question_period_ zmienna typu int określająca okres odpytywania kanału
-ack_transmision- Zmienna typu bool mówiąca o tym że rozpoczęto transmisję ACK
Pakiet	Package	Klasa reprezentująca przesyłany pakiet. Zawiera informacje ile dozwolonych retransmisji pozostało, długość pakietu oraz źródło pakietu	-r_Zmienna lokalna typu int opisująca numer aktualnej retransmisji 
 -time_of_appear_ Zmienna typu unsigned long long int opisująca czas pojawienia się w systemie
-ctpk_Zmienna opisująca czas trwania transmisji
-soure_transmiter_ Zmienna typu int opisująca z której ze stacji nadawczych wyslany był dany pakiet
-crp_ zmienna typu unsigned long long int określająca czas następnej retransmisji
-hR_zmienna typu int pomocna zmienna losowa
-id_ pakietu zmienna typu unsigned long long int określająca nadawany pakiet 
-ret_ zmienna typu bool informująca czy aktualny pakiet ma być retransmitowany
-is_collision-  zmienna typu bool wskazująca że nastąpiła kolizja
-is_error- zmienna typu bool wskazująca że wystąpił błąd transmisji
Kanał	Channel	Klasa reprezentująca kanał. Klasa sprawdza wystąpienie kolizji oraz błędu TER, a także zarządza szczelinami czasowymi	-free_ Zmienna boolowska określająca zajętość kanału
-collision_ Zmienna boolowska określająca czy nastąpiła kolizja
-vector<Package*> package_buffer_- wektor wskaźników na Package, przechowujący kopię pakietu podczas transmisji (pakietu oraz ACK)



3. Opis przydzielonej metody symulacyjnej:

a. Schemat blokowy pętli głównej:
 
Rysunek 2 Schemat blokowy pętli głównej.
b. Lista zdarzeń czasowych i warunkowych:
Opis zdarzeń czasowych:
Zdarzenie	Opis	Algorytm
Generacja pakietu dla k'tej stacji (CGPk) 	Zdarzenie generacji nowego pakietu dla k'tej stacji nadawczej (każda stacja ma własny generator)	1) Umieść pakiet na końcu kolejki
2) Zaplanuj generację kolejnego pakietu zgodnie z wartością zmiennej CGPk
Zakończenie transmisji pakietu 	Zdarzenie generowane przez upływ czasu CTPk. 	1) Zezwól na wystąpienie zdarzenia Początek transmisji ACK.
Koniec transmisji ACK 	Zdarzenie generowane przez upływ czasu ActualCLK + CTPk + CTIZ. Potwierdza ono poprawne dostarczenie pakietu do stacji odbiorczej i kontroluje decyzje stacji nadawczej w przypadku wystąpienia kolizji. Ponieważ to zdarzenie jest ostatnim w ciągu zdarzeń symulacyjnych zdecydowano się na podejmowanie działań sprawdzających warunki wewnątrz ciała tego zdarzenia. 	1) Sprawdź czy zmienna ack_transmision  jest true.
2) Jeśli nie Usuń pakiet z bufora kanału, flaga free_ na true, ret_ na true, odblokuj START
3) Jeśli tak ustaw zmienną ack_transmision  na false i sprawdź czy powstała kolizja.
(Ilość pakietów w buforze kanału musi wynosić 1. Jeśli 0 to prześlij błąd do debbugera, jeśli więcej to kolizja) {kolizja pakietu ACK z innym pakietem}
4.1) Jeśli tak Usuń pakiet z bufora kanału, flaga free_ na true, ret_ na true, _is_collision_ na true odblokuj START
3) Jeśli nie było kolizji sprawdź zmienne _is_error_ i _is_collision_  są false.
4.1) Jeśli są, Usuń pakiet z bufora kanału, Usuń pakiet z bufora stacji nadawczej, Zwolnij kanał. Odblokuj  START.
4.2) Jeśli nie są false:  Usuń pakiet z bufora kanału, flaga free_ na true, ret_ na true, odblokuj START

Koniec oczekiwania na kolejne odpytanie kanału 	Zdarzenie generowane przez upływ czasu oczekiwania na kolejne odpytanie. 	1) Zezwól na zdarzenie Sprawdzenie zajętości kanału
Koniec oczekiwania na transmisję pakietu 	Zdarzenie generowane przez upływ czasu oczekiwania na koniec transmisji.	1) Zezwól na zdarzenie Transmisja pakietu


Opis zdarzeń warunkowych:

Zdarzenie	Opis	Algorytm
A5B Start 	Na podstawie flagi Ret klasy Package podejmij decyzję czy pakiet jest nowy czy retransmitowany	1.1) Jeśli kolejka nie jest pusta 
 Zablokuj możliwość wystąpienia tego zdarzenia, sprawdź czy ret true or false.
1.2) Jeśli kolejka jest pusta zablokuj to zdarzenie w obrębie bieżącej iteracji pętli symulacyjnej. (przy kolejnej iteracji ma być odblokowane)
2) Jeśli ret jest false ustaw question_period_ na 500us i zezwól na zdarzenie sprawdzenie zajętości kanału 
3) Jeśli true ustaw zmienna _is_error_ i _is_collision_ na false  inkrementuj, zmienną r sprawdź czy r_>= kLR _
3.1) Jeśli tak to usuń pakiet z kolejki. Odblokuj START
3.2) Jeśli nie wylosuj nowe CRP. Ustaw question_period_  na 500us, i zaplanuj koniec oczekiwania na odpytanie kanału zgodnie z czasem CRP.
Sprawdzenie zajętości kanału 	Na podstawie flagi Free klasy kanał sprawdzana jest zajętość kanału. Algorytm decyduje o możliwości transmisji lub konieczności czekania na kanał	1) Sprawdź czy flaga free_ klasy Channel jest true.
2) Jeśli nie,  zaplanuj koniec oczekiwania na odpytanie kanału zgodnie z ustawioną zmienną question_schedule_  (systemowy czas na kolejne odpytanie) 
3) Jesli flaga free_ klasy Channel jest true, zaplanuj koniec oczekiwania na transmisję pakietu zgodnie z dozwolonymi szczelinami czasowymi

Transmisja pakietu 	Jeśli udało się znaleźć wolną szczelinę czasową rozpocznij transmisję w odpowiedniej szczelinie, zgodnie z zegarem szczelin - ChannelClock. 	1) Sprawdź czy udało się transmitować (losuj zgodnie z prawdopodobieństwem PT)
2.1) Jeśli tak, ustaw flagę free_ klasy Chanel na false(zajmij kanał), skopiuj obiekt pakietu do bufora w kanale (zmienna channel_buffer_)
zaplanuj koniec transmisji pakietu. 
2.2) Jesli nie ustaw question_period_  na 1000us, zaplanuj koniec oczekiwania na odpytanie kanału 
Początek transmisji ACK 	Zdarzenie jest początkiem transmisji pakietu ACK. 	
1) Sprawdź czy powstała kolizja.
(Ilość pakietów w buforze kanału musi wynosić 1. Jeśli 0 to prześlij błąd do debbugera, jeśli więcej to kolizja)
2.1) Jeśli tak, ret_ na true, zmienna _is_collision_ na true Ustaw flagę o dobiorze ACK na false, zaplanuj koniec transmisji ACK.
3) Jeśli nie było kolizji sprawdź czy powstał błąd TER.
3.1) Jeśli tak  , ret_ na true, zmiena _is_error_ na true, Ustaw flagę o dobiorze ACK na false zaplanuj koniec transmisji ACK.
4) Jeśli nie było błędu, Ustaw flagę o dobiorze ACK na true zezwól na wystąpienie zdarzenia Początek transmisji ACK.


4. Parametry wywołania programu.
	Użytkownik w pierwszej kolejności podaje nazwę pliku zawierającego ziarna w postaci nazwa.smf ,który jest plikiem tekstowym, którego każdy zestaw ziaren jest obwarowany znacznikami <Ln> <EOLn>, gdzie n jest numerem zestawu. Każdy zestaw posiada stałą liczbę ziaren i musi być taka jak w podanym pliku, ze względu na liczbę generatorów w programie. Użytkownik wpisuje ręcznie z którego zestawu chce skorzystać i jest to równoznaczne z kolejnym numerem symulacji. Jeśli potrzeba więcej symulacji, przygotowane jest 15 takich plików. Następnie użytkownik wpisuje czas trwania symulacji ( czas symulacyjny a nie czas wykonywania programu), w postaci jednostki, z których dostępne są: sekundy, minuty, godziny, dni, miesiące, lata. Zalecane jest aby nie przekraczać kilku dni. Po określeniu liczby sekund / minut etc, użytkownik proszony jest o podanie trybu pracy symulatora. Dostępne tryby to ciągły i krokowy. Symulator generuje dwa pliki. Plik logu oraz nasycenia. Dla pliku logu wybieramy spośród trybów: FULL - zbierane są dane o stanie systemu przy każdej zmianie zegara systemowego oraz dane o zdarzeniach czasowych i warunkowych przy każdym ich wystąpieniu. EVENTSONLY - zbierane są dane tylko o zdarzeniach czasowych i warunkowych. SYSTEMSTATE- zbierane są dane tylko o stanie systemu przy każdej zmianie zegara systemowego. FINALRESULTONLY - System zachowuje tylko końcowe statystyki. Jest to tryb najszybszy. Należy zauważyć, że niezależnie w każdym z tych trybów zbierane są końcowe statystyki. Dalej użytkownik podaje czas od początku którego system ma zbierać statystyki. Należy tu podać punkt nasycenia symulacji lub 1 gdy się go dopiero wyznacza. W dalszej kolejności program prosi o podanie intensywności L którą należy podać. Ostatnią rzeczą przed rozpoczęciem symulacji jest podanie ogranicznika liczby próbek dla pliku nasycenia. Tzn. co n 'ta próbka będzie zapisana do pliku. Jest to podyktowane faktem, iż program dla niektórych parametrów symulacji podaje znaczną liczbę danych i programy do obróbki danych gł. (Excel w wersji 32bit) nie radzą sobie z tak dużą liczbą próbek. Program w trakcie trwania symulacji podaje na bieżąco procent postępu symulacji oraz czas obiegu 1000 powtórzeń pętli głównej. Znaczący przyrost tego czasu może wskazywać na powstający problem (np. zawieszanie) w przebiegu symulacji. Po skończeniu przebiegu symulacyjnego można wyświetlić logi lub zapisać je do pliku a także należy zapisać plik nasycenia. 


5. Generatory.
Generatory zostały stworzone na podstawie pseudokodu podanego przez prowadzącego. Poniżej przedstawiam fragmenty klasy generators:
#ifndef GENERATORS_H
#define GENERATORS_H
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <ostream>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
class Generators
{
public:
	double Random(double &kernel_);
	double Random(int precision_, double& kernel_);
	double Random(double min, double max, double& kernel_);
	double Random(double min, double max, int precision_, double& kernel_);
	double RndExp(double lambda, int precision_, double& kernel_);
	int RndZeroOne(double p, double& kernel_);
	Generators();
	~Generators();
...
//Metody zwiększające o 1 liczbę wystąpień odpowiednich kerneli: 
	void IncreaseCGPkKernelCounter(int index);
	void IncreaseCTPkKernelCounter();
	void IncreaseRKernelConuter();
	void IncreaseTERKernelCounter();
	void IncreasePTKernelCounter();
...
private:
	void LineToMarkups(int line, string& markup_begin, string& markup_end);
	bool ParseFile(vector<double>& values, fstream& file_, string begin_, string end);
	double Floor(double number_);
	int FloorToInt(double number_);
	size_t FloorToSizeT(double number_);
	double SetPrecision(double value_, int numbers_after_coma_);
	double M = 2147483647.0;
	double A = 16807.0;
	double Q = 127773.0;
	double R = 2836;
...
};



#endif // !GENERATORS_H

#include "Generators.h"


double Generators::Random(double& kernel_)
{
	double h = Floor(kernel_ / Q);
	kernel_ = A * (kernel_ - Q * h) - R * h;
	if (kernel_ < 0.0)
	{
		kernel_ += M;
	}
	return kernel_ / M;
}

double Generators::Random(int precision_, double& kernel_)
{
	double h = Floor(kernel_ / Q);
	kernel_ = A * (kernel_ - Q*h) - R*h;
	if (kernel_ < 0.0)
	{
		kernel_ += M;
	}
	return SetPrecision( kernel_ / M, precision_);
}

double Generators::Random(double min, double max, int precision_, double& kernel_)
{
	return SetPrecision(Random(kernel_) * (max - min) +min, precision_);
}

double Generators::Random(double min, double max, double& kernel_)
{
	return Random(kernel_) * (max - min) + min;
}

double Generators::RndExp(double lambda, int precision_, double& kernel_)
{
	double k = Random(kernel_);
	return SetPrecision(-(1.0/ lambda) * (double)log(k), precision_);
}

int Generators::RndZeroOne(double p, double& kernel_)
{
	double k = Random(kernel_);
	if (k < p)
		return 1;
	else
		return 0;
}
...
void Generators::LineToMarkups(int line, string& markup_begin, string& markup_end)
{
	markup_begin = "<L"+to_string(line)+">";
	markup_end = "<EOL"+to_string(line)+">";
}
bool Generators::ParseFile(vector<double>& values, fstream& file_, string begin_, string end)
{
	string txt_ = "";
	while (txt_ != begin_)
	{
		file_ >> txt_;
		if (txt_ == "<EOF>")
		{
			return false;
		}
	}
	size_t sz;
	file_ >> txt_;//Skip one markup
	while (txt_ != end)
	{
		values.push_back(stod(txt_, &sz));
		file_ >> txt_;
	}
	return true;
}

double Generators::Floor(double number_)
{
	double fraction_ = number_;
	double integer_ = 0;
	fraction_ = modf(number_, &integer_);
	if (number_ >= 0.0)
	{
		return integer_;
	}
	else
	{
		return integer_ - 1.0;
	}

}

int Generators::FloorToInt(double number_)
{
	if (number_ >= 0.0)
	{
		return static_cast<int> (number_);
	}
	else
	{
		return static_cast<int> (number_ - 1.0);
	}
}

size_t Generators::FloorToSizeT(double number_)
{
	if (number_ >= 0.0)
	{
		return static_cast<size_t> (number_);
	}
	else
	{
		return static_cast<size_t> (number_ - 1.0);
	}
}

double Generators::SetPrecision(double value_, int numbers_after_coma_)
{
	int multiplier = 1;
	for (int i = 0; i < numbers_after_coma_; i++)
	{
		multiplier = multiplier * 10;
	}
	value_ = value_ * (double)multiplier;
	value_ = Floor(value_);
	value_ = value_ / (double)multiplier;
	return value_;
}

Generator rozkładu równomiernego jest generatorem opartym na liczbie tzw. ziarnie. Ziarno jest podawane jako losowa liczba, przed pierwszym losowaniem, jest zapisywana do zmiennej i sukcesywnie na nowo obliczana z każdym losowaniem. Generator rozkładu wykładniczego jest generatorem opartym o generator rozkładu równomiernego i działa na zasadzie odwracania dystrybuanty. Generator rozkładu zero-jedynkowego również jest oparty o generator rozkładu równomiernego i działa na zasadzie zwracania 0 jeśli wylosowana liczba jest >= od podanego prawdopodobieństwa lub zwraca 1 w przeciwnym razie.
W celu uzyskania dobrych wyników działania generatorów wprowadzono funkcje Floor oraz SetPrecision. Funkcja Floor jest niezbędna do działania generatora rozkładu równomiernego, ponieważ wprowadza zmienność do obliczanej liczby ziarna przez zaokrąglanie w dół jego wartości. Funkcja SetPrecision pozwala na wygenerowanie rozkładu wykładniczego dla małej liczby losowań w przypadku tego generatora tzn. (poniżej 30 000 losowań) i pozwala w łatwy sposób stworzyć histogram. Funkcja ta przyjmuje jako jeden z parametrów liczbę miejsc po przecinku jaka należy zostawić w zwracanym wyniku. Okazuje się że dla małej liczby losowań generator ten nie generuje pożądanego histogramu (ze względu na niewielkie zmienności na odległych miejscach po przecinku). 
W celu przetestowania generatorów napisano program generujący histogramy dla generatorów rozkładu równomiernego, wykładniczego i zero - jedynkowego:
 
Rysunek 3 Histogram generatora rozkładu równomiernego dla zakresu 0 - 1

 
Rysunek 4 Histogram liczb zwracanych przez generator rozkładu równomiernego z przedziału 1-100.

 
Rysunek 5 Histogram dla generatora rozkładu wykładniczego dla L=20.
 
Rysunek 6 Histogram generatora o rozkładzie zero - jedynkowym i prawdopodobieństwie sukcesu 0,8.
b) Wyjaśnienie sposobu zapewnienia niezależności sekwencji losowych w różnych symulacjach.
	W celu generacji ziaren napisano program generujący losowe ziarna. Na jeden przebieg symulacyjny przewidziano 50 ziaren. (5 generatorów X 10 stacji nadawczych) Ziarna te są umieszczone w plikach KernelsN.smf, gdzie N jest numerem pliku (będącym częścią nazwy). Plik ten jest plikiem tekstowym o konstrukcji zbliżonej do składni XML. Każdy z plików zawiera 10 zestawów ziaren ( na 10 symulacji), natomiast plików załączono również w liczbie 15'stu co łącznie daje możliwość przeprowadzenia 150 przebiegów symulacyjnych, co łącznie daje 7500 ziaren do wykorzystania. Ziarna z pliku tekstowego są wczytywane do wektora, który następnie rozdysponuje ziarnami do poszczególnych generatorów. (Również do wektorów tym razem przypisanych poszczególnym generatorom. ) 
6. Opis metody testowania i weryfikacji poprawności działania programu. 

	W celu weryfikacji poprawności działania programu przeprowadzono wielokrotnie symulacje dla stałych ziaren. Zmieniano parametry takie jak długość trwania symulacji, parametr lambda, TER, CRP. Wyniki symulacji zapisywane są do tekstowego pliku logu, w którym określone są warunki początkowe, stany systemu po każdym obrocie głównej pętli symulacyjnej, a także działania podejmowane przez każde zdarzenie czasowe oraz warunkowe. Na końcu pliku logu podawane są wyniki symulacji (określone w zadaniu). Na podstawie pliku logu oraz trybu debbug symulacji a także narzędzi analizy działania kodu dostarczonych przez Visual Studio 2019 ( w tym breakpoints, Debbuger, Analizator wydajności, Stos wywołań ) przeprowadzono dogłębną analizę zgodności działania programu z założonym algorytmem symulacyjnym, w wyniku której poprawiono nieliczne błędy. Ostatecznie dzięki generowaniu przez symulację pliku nasycenia udowodniono nasycanie się symulacji (spośród parametrów: pakietowa stopa błędu, średnia liczba retransmisji oraz średni czas oczekiwania pakietów dwa pierwsze wykazywały cechy nasycania wraz z upływem czasu co pozostawało w zgodnie z oczekiwaniami) 
7. Wyniki symulacji.

 a) Wyznaczenie czasu nasycenia(fazy początkowej)
	W celu wyznaczenia fazy początkowej wykonano po dziesięć przebiegów symulacyjnych z różnymi ziarnami, dla parametrów intensywności L: 0,008; 0,01; 0,1; 0,5 oraz stałym czasem symulacji ustawionym na 2 minuty. Wyniki przycięto do najmniejszej liczby próbek i sporządzono wykres zależności średniej liczby retransmisji (uśrednionej po 10'ciu przebiegach dla każdego parametru L) od czasu systemowego. 
 
	Z przebiegu wykresów wynika , iż najpewniejszą wartością czasu tego systemu na stabilizację wyników jest czas 55s. Zatem dalsze wyniki będą zbierane po tym czasie. 
	Dla zadanego systemu, nawet dla intensywności 0,5 pakietowa stopa błędu (średnia i maksymalna) były zbliżone do 0% lub wynosiły dokładnie 0%. Dla tego Aby uzyskać mniej więcej poziom 10% zmniejszono liczbę dozwolonych retransmisji do 7. Postawiono hipotezę, że symulator przy liczbie retransmisji zmniejszonej do 7 wejdzie w stan nasycenia do czasu 55s, czasu symulacyjnego. Aby potwierdzić tą tezę wykonano po trzy dodatkowe symulacje dla każdej wartości parametru L i uśredniono rysując wykres.
 
Rysunek 7 Zależność liczby retransmisji od czasu dla zmniejszonej do 7 dozwolonej liczby retransmisji pakietów.
	Analiza wykresu wskazuje, że w tym przypadku nasycenie występuje szybciej, czyli już po 36s, czasu systemowego, co potwierdza postawioną tezę. Dalsze symulację prowadzono dla 7 dozwolonych retransmisji oraz 5 minut czasu systemowego.
b)Wyznaczenie parametru L.
	Wyznaczono parametr L dla 7 dozwolonych retransmisji 5’cio minutowej symulacji oraz 55s czasu nasycania:
 

Rysunek 8 Średnia pakietowa stopa błędu.
C) Wykresy maksymalnej pakietowej stopy błędu oraz przepływności w zależności od parametru L.
 
Rysunek 9 Wykres zależności maksymalnej pakietowej stopy błędu w zależności od wartości parametry L
 
Rysunek 10 Wykres zależności przepływności pakietowej od wartości parametru L.

	Analiza powyższych wykresów wskazuje, że wartość parametru L powinna się mieścić w zakresie 0,007. Dla takiej wartości średnia pakietowa stopa błędu mieści się w zadanym zakresie maksymalnym 0,1 uzyskując zadowalającą przepływność ok. 70 pakietów / s. Z nachylenia prostej (biorąc pod uwagę odcinki między punktami) wynika, że w przedziale około 0,007 parametru L parametry systemu (tj. pakietowa stopa błędu średnia i maksymalna oraz przepływność systemu) nie ulegają znaczącym zmianom. Należy zaznaczyć, że gdyby testy prowadzone były dla 15 dozwolonych retransmisji pakietowa stopa błędu (maksymalna i średnia) była by o rząd wielkości mniejsza. 

D) Tabelka z wynikami symulacji dla każdego przebiegu.
 
Tabela 1 Wyniki 10'ciu przebiegów symulacji dla r=7, L=0,007 czas 5min.
E) Wyniki końcowe w postaci w postaci uśrednionych wyników po wszystkich przebiegach oraz przedziały ufności.
 
Tabela 2 Uśrednione wyniki oraz ich przedziały ufności.
F) Wykres zależności średniej liczby retransmisji od parametru P dla wyznaczonej liczby retransmisji.

 
Rysunek 11 Zależność średniej liczby retransmisji od wartości parametru P dla L=0,007 i 7’miu dozwolonych retransmisji.
8. Wnioski.
	
	Bazując na metodach testowania symulatora a także na zwracanych wynikach można stwierdzić, że program symulacyjny działa poprawnie. Nasycenie względem parametru średniej liczby retransmisji odbywa się w spodziewany sposób. Ciekawym zjawiskiem jest fakt osiągania przez symulator maksymalnego nasycenia przy danej wartości dozwolonych retransmisji (nakładające sie na siebie wykresy dla L=0,1 i L=0,5). Dla systemu z 10'cioma stacjami nadawczymi i 15'stoma dozwolonymi retransmisjami bardzo trudne okazuje się odnalezienie takiej wartości parametru L aby, średnia pakietowa stopa błędu wahała się w granicach 10%, przy algorytmie CSMA p. Przebieg maksymalnej pakietowej stopy błędu naśladuje przebieg średniej pakietowej stopy błędu, a zależność przepływności od intensywności jest rosnąca co jest zgodne z oczekiwaniami. Wartość parametru P mocno wpływa na średnią liczbę retransmisji, co jest spowodowane że przy niskich wartościach utrudnione jest poprawne nadawanie pakietów, do tego stopnia, że dla wartości P=0.0, stacje nadawcze nie przesyłają żadnych pakietów. Spadek średniej liczby retransmisji poniżej 1 dla P=1.0, świadczy o tym, że kolizje dla algorytmu CSMA p, występują stosunkowo rzadko. Projekt dał mi dobry pogląd na metody symulacji cyfrowych, zastosowanie statystyki opisowej oraz podniósł moje umiejętności programistyczne. Zmiana opcji Visual Studio z trybu debbug na release powoduje silny wzrost szybkości działania programu symulatora.
