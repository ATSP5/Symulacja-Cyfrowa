#ifndef ENDOFWAITFORTRANSMISION_H
#define ENDOFWAITFORTRANSMISION_H

#include <vector>
#include "TelecommunicationSystem.h"
#include "ApLogger.h"
using namespace std;
class EndOfWaitForTransmision 
{
private:
	int source_;// Numer stacji bazowej dla kt�rej to zdarzenie wyst�puje
	size_t next_event_time_;
public:
	bool  Execute(ApLogger* ap_log_);
	size_t GetTimeOfNextEvent();
	EndOfWaitForTransmision(int source_V);
	void PlanNextWaitForTransmision(size_t time_);

};

#endif // !ENDOFWAITFORTRANSMISION_H