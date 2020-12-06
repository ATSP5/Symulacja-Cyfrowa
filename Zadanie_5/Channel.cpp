#include "Channel.h"

Channel::Channel()
{
	/*
	Konstruktor inicjalizuje zmienne. U¿ywaæ jako domyœlny
	*/
	free_ = true;
	collision_ = false;
}

Channel::~Channel()
{
	//Destruktor czyœci miejsce po wskaŸnikach na Pakiet
	package_buffer_.clear();
}

bool Channel::GetChannelStatus()
{
	return free_;
}

bool Channel::GetCollisionFlag()
{
	return collision_;
}

vector<Package*> Channel::GetChannelPackageBuffer()
{
	return package_buffer_;
}

string Channel::GetChannelOcupyStatusAsString()
{
	if (free_ == true)
	{
		return "Channel is free.";
	}
	else
	{
		return "Channel is not free!";
	}
}

void Channel::SetChannelStatus(bool status_)
{
	free_ = status_;
}

void Channel::SetCollisionIncydent(bool incydent_)
{
	collision_ = incydent_;
}

void Channel::CopyPackageToBuffer(Package* package_buffer_V)
{
	package_buffer_.push_back(package_buffer_V);
}

void Channel::EreasePackageFromBufferAtIndex(int index)
{
	package_buffer_.erase(this->package_buffer_.begin() + index);
}

void Channel::PopBackFromPackageBuffer()
{
	package_buffer_.pop_back();
}
