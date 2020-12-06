#ifndef STARTOFACK_H
#define STARTOFACK_H

#include "TelecommunicationSystem.h"
#include "EndOfACKTransmision.h"
#include "ApLogger.h"
class StartOfACK
{
private:
	bool event_locked_;
	int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	TelecommunicationSystem* telecommunication_system_;
	vector<EndOfACKTransmision* > end_of_ack_tr_;
public:
	void  Execute(ApLogger* ap_log_);
	StartOfACK(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<EndOfACKTransmision* > &end_of_ack_tr_V);
	bool GetEventLockedStatus();
	void SetEventLock(bool lock_stat_);
};



#endif // !STARTOFACK_H

