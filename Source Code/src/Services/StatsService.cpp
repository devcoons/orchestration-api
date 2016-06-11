#include "StatsService.h"

	StatsService::StatsService()
	{

	}

	StatsService::~StatsService()
	{

	}

	void StatsService::start(StatsDevice _device)
	{
		device = _device;
	}
	
	string StatsService::retrieveUsage()
	{
		string temp="";
			switch(device)
			{
				case StatsDevice::Juno:
					temp = junoStats.retrieveUsage();
					break;
				case StatsDevice::Xeon:
					break;
				case StatsDevice::VMachine:
					break;
			}	
		return temp;			
	}
	
	string StatsService::retrieveSensors()
	{
		string temp="";
			switch(device)
			{
				case StatsDevice::Juno:			
					temp = junoStats.retrieveSensors();
					break;
				case StatsDevice::Xeon:
					break;
				case StatsDevice::VMachine:
					break;
			}	
		return temp;			
	}