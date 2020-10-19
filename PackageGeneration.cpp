#include "PackageGeneration.h"
#include <assert.h>
#include <ctime>
#include <cassert>
void PackageGeneration::Execute(ApLogger* ap_log_, size_t Package_ID_)
{
	if (ap_log_->GetInformationGatheringMode() == 0 || ap_log_->GetInformationGatheringMode() == 1)
	{
		ap_log_->AddLine("Creating new package. System clock: " + to_string(telecommunication_system_->Get_Clock()) + "Source of event base station: " + to_string(source_));
	}
	ap_log_->IncreaseNumberOfTotalCreatedPackages();
	ap_log_->GS_IncreseNumberOfPackagesGenerated();
	local_id_ = Package_ID_;
	assert(source_ >= 0 && source_ < 10);
	//Stwórz pakiet i wrzuæ go do kolejki odpowiedniej stacji nadawczej.
	telecommunication_system_->GetBaseStationVector()[source_]->CreatePackage(telecommunication_system_->Get_Clock(), source_, local_id_, (int)telecommunication_system_->GetGenerator()->Random(1.0,10.0,telecommunication_system_->GetGenerator()->kernel_for_CTPk[source_]));
	telecommunication_system_->GetGenerator()->IncreaseCTPkKernelCounter(source_);
	//Set CGPk for next event occurance
	telecommunication_system_->GetBaseStationVector()[source_]->SetCGPk((size_t)telecommunication_system_->GetGenerator()->RndExp(l_intensity,0, telecommunication_system_->GetGenerator() ->kernels_for_CGPk_[source_]) * 1000 );
	//telecommunication_system_->GetBaseStationVector()[source_]->SetCGPk((size_t)telecommunication_system_->GetGenerator()->Random(0, 100 * 1000, telecommunication_system_->GetGenerator()->kernels_for_CGPk_[source_]));
	telecommunication_system_->GetGenerator()->IncreaseCGPkKernelCounter(source_);
	next_event_time_ = telecommunication_system_->Get_Clock() + telecommunication_system_->GetBaseStationVector()[source_]->GetCGPk();
	assert(next_event_time_ >= telecommunication_system_->Get_Clock());
}
PackageGeneration::PackageGeneration(int source_station_, TelecommunicationSystem* telecommunication_sys_, double intensity_of_package_appear_)
{
	source_ = source_station_;
	telecommunication_system_ = telecommunication_sys_;
	l_intensity = intensity_of_package_appear_;
	//Set CGPk for next event occurance
	if (telecommunication_system_->Get_Clock() != 1)
	{
		telecommunication_system_->GetBaseStationVector()[source_]->SetCGPk((size_t)telecommunication_system_->GetGenerator()->RndExp(l_intensity, 0, telecommunication_system_->GetGenerator()->kernels_for_CGPk_[source_]) * 1000);
		//telecommunication_system_->GetBaseStationVector()[source_]->SetCGPk((size_t)telecommunication_system_->GetGenerator()->Random(0, 100 * 1000, telecommunication_system_->GetGenerator()->kernels_for_CGPk_[source_]));
		telecommunication_system_->GetGenerator()->IncreaseCGPkKernelCounter(source_);
		next_event_time_ = telecommunication_system_->Get_Clock() + telecommunication_system_->GetBaseStationVector()[source_]->GetCGPk();
	}
	else
	{
		do
		{
			telecommunication_system_->GetBaseStationVector()[source_]->SetCGPk((size_t)telecommunication_system_->GetGenerator()->RndExp(l_intensity, 0, telecommunication_system_->GetGenerator()->kernels_for_CGPk_[source_]) * 1000 );
			//telecommunication_system_->GetBaseStationVector()[source_]->SetCGPk((size_t)telecommunication_system_->GetGenerator()->Random(0, 100 * 1000, telecommunication_system_->GetGenerator()->kernels_for_CGPk_[source_]));
			telecommunication_system_->GetGenerator()->IncreaseCGPkKernelCounter(source_);
			next_event_time_ = telecommunication_system_->GetBaseStationVector()[source_]->GetCGPk();
		} while (next_event_time_==0);
	}
	assert(next_event_time_ >= telecommunication_system_->Get_Clock());
	local_id_ = 0;
}

void PackageGeneration::SetLIntencity(size_t intencity)
{
	l_intensity = intencity;
	next_event_time_ = l_intensity;
}

size_t PackageGeneration::GetTimeOfNextEvent()
{
	return next_event_time_;
}

size_t PackageGeneration::GetLIntencity()
{
	return l_intensity;
}

