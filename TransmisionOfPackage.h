#ifndef TRANSMISIONOFPACKAGE_H
#define TRANSMISIONOFPACKAGE_H

#include "TelecommunicationSystem.h"
#include "EndOfPackageTransmision.h"
#include "EndOfChannelQuestioning.h"
#include "ApLogger.h"
class TransmisionOfPackage
{
private:
	bool event_locked_;
	int source_;// Numer stacji bazowej dla której to zdarzenie wystêpuje
	TelecommunicationSystem* telecommunication_system_;
	vector<EndOfChannelQuestioning* > end_of_channel_questioning_;
	vector<EndOfPackageTransmision* > end_of_package_transmision_;
public:
	void  Execute(ApLogger* ap_log_);
	TransmisionOfPackage(int source_station_, TelecommunicationSystem* telecommunication_sys_, vector<EndOfPackageTransmision* > &end_of_package_transmision_V, vector<EndOfChannelQuestioning* > &end_of_channel_questioning_V_);
	bool GetEventLockedStatus();
	void SetEventLock(bool lock_stat_);
};

#endif // !TRANSMISIONOFPACKAGE_H


