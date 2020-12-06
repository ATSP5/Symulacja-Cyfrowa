#ifndef ENDOFACKTRANSMISION_H
#define  ENDOFACKTRANSMISION_H

#include <vector>
#include "TelecommunicationSystem.h"
#include "ApLogger.h"
#include <assert.h>
using namespace std;
class EndOfACKTransmision 
{
private:
	size_t next_event_time_;
	TelecommunicationSystem* tel_sys_;
	int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	int FindPackageByID(size_t id_v_);
	void RunFinalCheck(ApLogger* ap_log_);
public:
	bool  Execute(ApLogger* ap_log_);
	size_t GetTimeOfNextEvent();
	EndOfACKTransmision(int source_V, TelecommunicationSystem* tel_sys__V);
	void PlanNextACKTransmision(size_t time_);
};

#endif // !ENDOFACKTRANSMISION_H
