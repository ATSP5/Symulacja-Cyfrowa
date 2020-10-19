#ifndef A5B_START_H
#define A5B_START_H

#include "TelecommunicationSystem.h"
#include "IsChannelFree.h"
#include "EndOfChannelQuestioning.h"
#include "ApLogger.h"
#include <assert.h>
class A5B_Start
{
private:
	bool event_locked_;
	int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	TelecommunicationSystem* telecommunication_system_;
	vector<EndOfChannelQuestioning* > end_of_channel_questioning_;
	vector<IsChannelFree*> ischannelfree_ptr_;
public:
	void  Execute(ApLogger* ap_log_);
	A5B_Start(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<IsChannelFree*> ischannelfree_p_, vector<EndOfChannelQuestioning* > &end_of_channel_questioning_V);
	bool GetEventLockedStatus();
	void SetEventLock(bool ev_l);
};

#endif // !A5B_START