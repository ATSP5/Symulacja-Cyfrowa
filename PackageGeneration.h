#ifndef PACKAGEGENERATION_H
#define PACKAGEGENERATION_H

#include <vector>
#include "TelecommunicationSystem.h"
#include "ApLogger.h"
using namespace std;
class PackageGeneration
{
private:
	double l_intensity;
	 size_t local_id_;//Id kolejnych pakietów
	 size_t next_event_time_;
	 int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	 TelecommunicationSystem* telecommunication_system_;
public:
	 void  Execute(ApLogger* ap_log_, size_t Package_ID_);
	 PackageGeneration(int source_station_, TelecommunicationSystem* telecommunication_sys_, double intensity_of_package_appear_);
	 //Set methods:
	 void SetLIntencity(size_t intencity);
	 //Get methods:
	 size_t GetTimeOfNextEvent();
	 size_t GetLIntencity();
};

#endif // !PACKAGEGENERATION_H