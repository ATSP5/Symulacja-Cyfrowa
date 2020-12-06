#ifndef ISCHANNELFREE_H
#define ISCHANNELFREE_H

#include "TelecommunicationSystem.h"
#include "EndOfChannelQuestioning.h"
#include "EndOfWaitForTransmision.h"
#include <assert.h>
#include "ApLogger.h"
class IsChannelFree
{
private:
	bool event_locked_;
	int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	TelecommunicationSystem* telecommunication_system_; 
	vector<EndOfChannelQuestioning*> end_of_channel_questioning_;
	vector<EndOfWaitForTransmision*> end_of_wait_for_transmision;
public:
	void  Execute(ApLogger* ap_log_);
	IsChannelFree(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<EndOfChannelQuestioning*> &end_of_channel_questioning_V, vector<EndOfWaitForTransmision*> &end_of_wait_for_transmision_V);
	//Get Methods:
	bool GetEventLockedStatus();
	//Set methods:
	void SetEventLock(bool lock_);
};

#endif // !ISCHANNELFREE_H
