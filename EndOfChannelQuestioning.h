#ifndef ENDOFCHANNELQUESTIONING_H
#define ENDOFCHANNELQUESTIONING_H
#include "TelecommunicationSystem.h"
#include "ApLogger.h"
#include <vector>
using namespace std;
class EndOfChannelQuestioning 
{
private:
	size_t next_event_time_;
	int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	friend class IsChannelFree;
public:
	bool  Execute(ApLogger* ap_log_);
	size_t GetTimeOfNextEvent();
	EndOfChannelQuestioning(int source_V);
	void PlanNextChannelQuestioning(size_t time_);
};

#endif