#include "ApLogger.h"

ApLogger::ApLogger(TelecommunicationSystem* t_s_, int info_mode_, size_t GS_start_clk_)
{
	GS_time_of_start_gathering_stat = GS_start_clk_;
	log_ = "Simulation of telecomunicationsystem started \n ";
	saturation_ = "File created to show saturation of packages in BS'es \n";
	final_info_ = "";
	number_of_final_retransmision_info_ = 0;
	number_of_erroneous_transmision_info_ = 0;
	number_of_packages_encountering_collision_info_ = 0;
	number_of_packages_removed_after_max_retransmision_info_ = 0;
	for (int i = 0; i < 10; i++)
	{
		number_of_succesful_transmision_[i] = 0;
	}
	number_of_total_created_packages_ = 0;
	t_sys_ = t_s_;
	switch (info_mode_)
	{
	case 0:
		info_gathering_mode_ = FULL;
		break;
	case 1:
		info_gathering_mode_ = EVENTSONLY;
		break;
	case 2:
		info_gathering_mode_ = SYSTEMSTATE;
		break;
	case 3:
		info_gathering_mode_ = FINALRESULTONLY;
		break;
	default:
		info_gathering_mode_ = FULL;
		break;
	}
}

void ApLogger::SaveToFile(string file_name_)
{
	log_file_.open(file_name_);
	log_file_ << log_;
	log_file_.close();
	log_ = "";
}

void ApLogger::SaveToFileSaturationInfo(string file_name_)
{
	log_file_.open(file_name_);
	log_file_ << saturation_;
	log_file_.close();
	log_ = "";
}

void ApLogger::Dysplay()
{
	cout << log_ << endl;
	log_ = "";
}

void ApLogger::AddLine(string txt_)
{
	log_ +="\n"+ txt_;
}

void ApLogger::AddSystemState()
{
	for (int i = 0; i < 10; i++)
	{
		log_ += "System state: BS["+to_string(i)+"] number of packages: " + to_string(t_sys_->GetBaseStationVector()[i]->GetPackageVector().size()) + "\n";
		if (t_sys_->GetBaseStationVector()[i]->GetPackageVector().size() != 0)
		{
			log_ += "System state: BS[" + to_string(i) + "] first package ID: " + to_string(t_sys_->GetBaseStationVector()[i]->GetPackageVector()[0]->GetID())+ "\n";
			log_ += "System state: BS[" + to_string(i) + "] first package's retransmision number: " + to_string(t_sys_->GetBaseStationVector()[i]->GetPackageVector()[0]->GetRetransmision_r_Variable()) + "\n";
			log_ += "System state: BS[" + to_string(i) + "] first package's collision status: " + t_sys_->GetBaseStationVector()[i]->GetPackageVector()[0]->GetCollisionStatusAsString() + "\n";
			log_ += "System state: BS[" + to_string(i) + "] first package's transmission error status: " + t_sys_->GetBaseStationVector()[i]->GetPackageVector()[0]->GetErrorStatusAsString() + "\n";
		}
	}
	log_ += "\n ";
	log_ += "Channel status: " + t_sys_->GetChannel()->GetChannelOcupyStatusAsString() + "\n";
	log_ += "Number of packages in channel buffer: " + to_string(t_sys_->GetChannel()->GetChannelPackageBuffer().size()) + "\n";
	for (int i = 0; i < t_sys_->GetChannel()->GetChannelPackageBuffer().size(); i++)
	{
		log_ += "Package[" + to_string(i) + "]ID: " + to_string(t_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetID()) + "\n";
		log_ += "Transmission duration: " + to_string(t_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetCTPKTime() * 1000) + "\n";
		log_ += "CRP set to: " + to_string(t_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetCRPTime()) + "\n";
		log_ += "Potential retransmission at system time: " + to_string(t_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetCRPTime()+ t_sys_->Get_Clock()) +"\n";
		log_ += "Retransmission count: " + to_string(t_sys_->GetChannel()->GetChannelPackageBuffer()[i]->GetRetransmision_r_Variable()) + "\n";
	}
	log_ += "\n \n";
}

int ApLogger::GetInformationGatheringMode()
{
	switch (info_gathering_mode_)
	{
	case ApLogger::FULL:
		return 0;
		break;
	case ApLogger::EVENTSONLY:
		return 1;
		break;
	case ApLogger::SYSTEMSTATE:
		return 2;
		break;
	case ApLogger::FINALRESULTONLY:
		return 3;
		break;
	}
	return 0;
}

void ApLogger::AddFinalInfo()
{
	final_info_ += "Debug simmulation information: \n";
	final_info_ += "Total number of created packages: " + to_string(number_of_total_created_packages_) + "\n";
	final_info_ += "Total number of succesfull transmision per BS: \n";
		for (int i = 0; i < 10; i++)
		{
			final_info_ += "BS["+to_string(i)+"]: "+to_string(number_of_succesful_transmision_[i]) + "\n";
		}
	final_info_ += "Total number of retransmision attempt: " + to_string(number_of_final_retransmision_info_) + "\n";
	final_info_ += "Total number of errorenous transmision: " + to_string(number_of_erroneous_transmision_info_) + "\n";
	final_info_ += "Total number of packages encountering collision: " + to_string(number_of_packages_encountering_collision_info_) + "\n";
	final_info_ += "Total number of packages removed from BS becouse of reaching max retransmision number: " + to_string(number_of_packages_removed_after_max_retransmision_info_) + "\n";
	saturation_ += "\n";
	final_info_ += "Simmulation Result: \n";
	final_info_ += "Package sending speed: " + to_string((double)(GS_number_of_packages_generated_*1000000.0)/(double)(t_sys_->GetUserSetSimmulationTime()- GS_time_of_start_gathering_stat)) + " [packages/s] \n";// Przep³ywnoœæ pakietowa
	final_info_ += "Average number of retransmission: " + to_string(CountRetransmissionAverage()) + "\n";
	final_info_ += "Average time of package delay: " + to_string(GS_CalculateAverageTimeOfSuccesfulPackageTransmission(time_of_succesful_package_transmission_vector_) / 1000.0) + " [ms] \n";
	final_info_ += "Average time of wait for transmision: " + to_string(GS_CalculateAverageTimeOfStartOfPackageTransmission(time_of_start_of_package_transmission_vector_)/ 1000.0) + " [ms] \n";
	final_info_ += "Average package error ratio: " + to_string(GS_CalcualteAveragePackageErrorRatio() * 100.0) + " [%] \n";
	final_info_ += "Max package error ratio: " + to_string(GS_CalculateMaxPackageErrorRatio()*100.0) + " [%] \n";
	final_info_ += "Kernels to store: \n";
	final_info_ += t_sys_->GetGenerator()->GetOldKernelsAsString();
	log_ += final_info_;
}

void ApLogger::AddSaturationInfo(int samples_, short int mode)
{
	// 0 nasycenie dla parametru pakietowej stopy b³êdu, 1 dla œredniej liczby retransmisji dla ka¿dej BS 2, 2 dla œredniego opóŸnienia pakietu 3 nie rób nic
	//Tryb 1 i 2 stosowaæ tylko na koñcu symulacji!!! Jeœli trzeba zastosowaæ tryb 1 lub 2 to w pêtli symulacyjnej wywo³aæ t¹ funkcêw trybie 3!!! Gdy potrzeba stosowania 
	// Innych trybów ni¿ 1 lub 2 to w pêtli stosowaæ te tryby a na koñcu symulacji zastosowaæ tryb 3!!!
	size_t maxretransmissionPerPackagePerBSvectorsize_ = 0;
	
		switch (mode)
		{
		case 1:
			for (int i = 0; i < 10; i++)
			{
				if (maxretransmissionPerPackagePerBSvectorsize_ < GS_NumberOfPackageRetransmissionPerBS_[i].size())
				{
					maxretransmissionPerPackagePerBSvectorsize_ = GS_NumberOfPackageRetransmissionPerBS_[i].size();
				}
			}
			for (int q = 0; q < maxretransmissionPerPackagePerBSvectorsize_; q++)
			{
				if (sample_number_ % samples_ == 0)
				{
					if (system_clock_of_retransmission_.size() > q)
					{
						saturation_ += to_string(system_clock_of_retransmission_[q]) + "\t";
					}
					for (int i = 0; i < 10; i++)
					{
						if (GS_NumberOfPackageRetransmissionPerBS_[i].size() > q)
						{
							saturation_ += to_string(GS_NumberOfPackageRetransmissionPerBS_[i][q]) + "\t";
						}
						else
						{
							saturation_ += to_string(0.0) + "\t";
						}

					}
					saturation_ += "\n";
				}
				sample_number_++;
			}
			break;
		case 2:
			for (int i = 0; i< time_of_succesful_package_transmission_vector_.size(); i++)
			{
				if (sample_number_ % samples_ == 0)
				{
					saturation_ += to_string(time_of_succesful_package_transmission_vector_[i] / 1000.0) + "\t";
					saturation_ += to_string(system_clock_of_end_package_transmission_vector_[i]) + "\n";
				}
				sample_number_++;
			}
			saturation_ += "\n";
			break;
		case 3:
			break;
		case 0:
		default:
			if (sample_number_ % samples_ == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					if ((double)GS_NumberOfPackagesCorrectlyRecivedPerBS[i] == 0.0)
					{
						saturation_ += to_string((double)0.0) + "\t";
					}
					else
					{
						saturation_ += to_string((double)GS_NumberOfPackagesWithTransmissionError_[i] / (double)GS_NumberOfPackagesCorrectlyRecivedPerBS[i]) + "\t";
					}
				}
				saturation_ += "\n";
			}
			sample_number_++;
			break;
		}
	
}

void ApLogger::IncreaseNumberOfTotalRetransmisionInfo()
{
	number_of_final_retransmision_info_++;
}

void ApLogger::IncreaseNumberOfTotalErroneousTransmisionInfo()
{
	number_of_erroneous_transmision_info_++;
}

void ApLogger::IncreaseNumberOfTotalPackagesEncounteringCollisionInfo()
{
	number_of_packages_encountering_collision_info_++;
}

void ApLogger::IncreaseNumberOfTotalPackagesRemovedAfterMaxRetransmisionInfo()
{
	number_of_packages_removed_after_max_retransmision_info_++;
}

void ApLogger::IncreaseNumberOfSuccesfullTransmision(int bs_)
{
	number_of_succesful_transmision_[bs_]++;
}

void ApLogger::IncreaseNumberOfTotalCreatedPackages()
{
	number_of_total_created_packages_++;
}

size_t ApLogger::GetTelecomunicationSystemTime()
{
	return t_sys_->Get_Clock();
}

void ApLogger::StartGatheringStatistics()
{
	if (t_sys_->Get_Clock() >= GS_time_of_start_gathering_stat)
	{
		gathering_statistics_permission_ = true;
	}
}

bool ApLogger::GetGatheringStatisticsPermissionStatus()
{
	return gathering_statistics_permission_;
}

void ApLogger::GS_IncreseNumberOfPackagesGenerated()
{
	if (gathering_statistics_permission_ == true)
	{
		GS_number_of_packages_generated_++;
	}
}

void ApLogger::GS_GatherNumberOfPackagesInBSOfEveryMoment()
{
	GS_number_of_package_in_bs_1.push_back((int)t_sys_->GetBaseStationVector()[0]->GetPackageVector().size());
	GS_number_of_package_in_bs_2.push_back((int)t_sys_->GetBaseStationVector()[1]->GetPackageVector().size());
	GS_number_of_package_in_bs_3.push_back((int)t_sys_->GetBaseStationVector()[2]->GetPackageVector().size());
	GS_number_of_package_in_bs_4.push_back((int)t_sys_->GetBaseStationVector()[3]->GetPackageVector().size());
	GS_number_of_package_in_bs_5.push_back((int)t_sys_->GetBaseStationVector()[4]->GetPackageVector().size());
	GS_number_of_package_in_bs_6.push_back((int)t_sys_->GetBaseStationVector()[5]->GetPackageVector().size());
	GS_number_of_package_in_bs_7.push_back((int)t_sys_->GetBaseStationVector()[6]->GetPackageVector().size());
	GS_number_of_package_in_bs_8.push_back((int)t_sys_->GetBaseStationVector()[7]->GetPackageVector().size());
	GS_number_of_package_in_bs_9.push_back((int)t_sys_->GetBaseStationVector()[8]->GetPackageVector().size());
	GS_number_of_package_in_bs_10.push_back((int)t_sys_->GetBaseStationVector()[9]->GetPackageVector().size());
}

void ApLogger::GS_IncrementNumberOfRetransmissionPerBS(int bs_)
{
	if (gathering_statistics_permission_ == true)
	{
		GS_NumberOfRetransmissionPerBS_[bs_]++;
	}
}

void ApLogger::GS_DecreaseNumberOfRetransmissionPerBS(int bs_, int number_to_erease_)
{
	if (gathering_statistics_permission_ == true)
	{
		GS_NumberOfRetransmissionPerBS_[bs_] -= number_to_erease_;
	}
}

void ApLogger::GS_AddNewTimeOfSuccesfulPackageTransmission(size_t time_, TelecommunicationSystem* t_s_)
{
	if (gathering_statistics_permission_ == true)
	{
		time_of_succesful_package_transmission_vector_.push_back(time_);
		system_clock_of_end_package_transmission_vector_.push_back(t_s_->Get_Clock());
	}
}

double ApLogger::GS_CalculateAverageTimeOfSuccesfulPackageTransmission(vector<size_t>& time_)
{
	double av = 0.0;
	for (int i = 0; i < time_of_succesful_package_transmission_vector_.size(); i++)
	{
		av += time_of_succesful_package_transmission_vector_[i];
	}
	return av/ time_of_succesful_package_transmission_vector_.size();
}

void ApLogger::GS_AddNewTimeOfStartOfPackageTransmission(size_t time_)
{
	if (gathering_statistics_permission_ == true)
	{
		time_of_start_of_package_transmission_vector_.push_back(time_);
		
	}
}

double ApLogger::GS_CalculateAverageTimeOfStartOfPackageTransmission(vector<size_t>& time_)
{
	double av = 0.0;
	for (int i = 0; i < time_of_start_of_package_transmission_vector_.size(); i++)
	{
		av += time_of_start_of_package_transmission_vector_[i];
	}
	return av / time_of_start_of_package_transmission_vector_.size();
}

void ApLogger::GS_IncrementNumberOfPackagesWithTransmissionErrorPerBS(int sourceBS_)
{
	if (gathering_statistics_permission_ == true)
	{
		GS_NumberOfPackagesWithTransmissionError_[sourceBS_]++;
	}
}

void ApLogger::GS_IncrementNumberOfPackagesCorrectlyRecivedPerBS(int sourceBS_)
{
	if (gathering_statistics_permission_ == true)
	{
		GS_NumberOfPackagesCorrectlyRecivedPerBS[sourceBS_]++;
	}
}

double ApLogger::GS_CalcualteAveragePackageErrorRatio()
{
	int continue_counter = 0;
	double sum=0.0;
	for (int i = 0; i < 10; i++)
	{
		if (GS_NumberOfPackagesCorrectlyRecivedPerBS[i] == 0)
		{
			continue_counter++;
			continue;
		}
		sum += (double)GS_NumberOfPackagesWithTransmissionError_[i] / (double)GS_NumberOfPackagesCorrectlyRecivedPerBS[i];
	}
	return sum / (10.0-(double)continue_counter);
}

double ApLogger::GS_CalculateMaxPackageErrorRatio()
{
	int maxIindex = 0;
	double PackageErrorRatioTable[10] = { 0,0,0,0,0,0,0,0,0,0 };
	for (int i = 0; i < 10; i++)
	{
		if (GS_NumberOfPackagesCorrectlyRecivedPerBS[i] == 0)
		{
			continue;
		}
		PackageErrorRatioTable[i] = (double)GS_NumberOfPackagesWithTransmissionError_[i] / (double)GS_NumberOfPackagesCorrectlyRecivedPerBS[i];
		if (PackageErrorRatioTable[maxIindex] < PackageErrorRatioTable[i])
		{
			maxIindex = i;
		}
	}
	return PackageErrorRatioTable[maxIindex];
}


void ApLogger::GS_IncreaseNumberOfRetransmissionPerPackagePerBS(int station, int retransmission)
{
	if (gathering_statistics_permission_ == true)
	{
		GS_NumberOfPackageRetransmissionPerBS_[station].push_back(retransmission);
	}
}

void ApLogger::GS_IncreaseNumberOfRetransmissionPerPackagePerBS()
{
	if (gathering_statistics_permission_ == true)
	{
		for (int i = 0; i < 10; i++)
		{
			if (t_sys_->GetBaseStationVector()[i]->GetPackageVector().size() != 0)
			{
				GS_NumberOfPackageRetransmissionPerBS_[i].push_back(t_sys_->GetBaseStationVector()[i]->GetPackageVector()[0]->GetRetransmision_r_Variable());
			}
			else
			{
				GS_NumberOfPackageRetransmissionPerBS_[i].push_back(0);
			}
		}
		system_clock_of_retransmission_.push_back(t_sys_->Get_Clock());
	}
}


double ApLogger::CountRetransmissionAverage()
{
	vector<double> sums;
	double average = 0.0;
	double add = 0;
	for (int q = 0; q < GS_NumberOfPackageRetransmissionPerBS_[0].size(); q++)
	{
		for (int i = 0; i < 10; i++)
		{
			add += GS_NumberOfPackageRetransmissionPerBS_[i][q];
		}
		sums.push_back(add / 10.0);
		add = 0.0;
	}
	for (int i = 0; i < sums.size(); i++)
	{
		average += sums[i];
	}
	return average/sums.size()
		;
}
