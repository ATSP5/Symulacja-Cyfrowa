#ifndef ENDOFPACKAGETRANSMISION_H
#define ENDOFPACKAGETRANSMISION_H

#include <vector>
#include "TelecommunicationSystem.h"
#include "EndOfACKTransmision.h"
#include "ApLogger.h"
#include <assert.h>
using namespace std;
class EndOfPackageTransmision
{
private:
	int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	size_t next_event_time_;
	TelecommunicationSystem* tel_sys_;
	vector<EndOfACKTransmision*> end_of_ACK_transm_;
public:
	bool  Execute(ApLogger* ap_log_);
	size_t GetTimeOfNextEvent();
	EndOfPackageTransmision(int source_V, TelecommunicationSystem* tel_sys__V, vector<EndOfACKTransmision*>& end_of_ACK_transm_V);
	void PlanNextWaitForTransmision(size_t time_);
};

#endif