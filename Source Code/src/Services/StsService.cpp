#include "StsService.h"

	StsService::StsService()
	{
		device = StatsDevice::Juno;
	}

	StsService::~StsService()
	{

	}

	void StsService::start(StatsDevice _device)
	{
		device = _device;
		std::thread upd(StsService::update,this);
		upd.detach();
	}
	
	void StsService::update(void * object)
	{
		StsService * self = static_cast<StsService*>(object);
		std::chrono::steady_clock::time_point begin;
		std::chrono::steady_clock::time_point end;
		auto deltaTime = std::chrono::milliseconds(250);
		while(1)
		{
			switch(self->device)
			{
				case StatsDevice::Juno:
					self->mux1.lock();
					begin = std::chrono::steady_clock::now();
					self->junoStats.update();
					end = std::chrono::steady_clock::now();
					self->mux1.unlock();
					deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
					deltaTime = std::chrono::milliseconds(250) - deltaTime > std::chrono::milliseconds(5) ? std::chrono::milliseconds(250) - deltaTime : std::chrono::milliseconds(5);
					break;
				case StatsDevice::Xeon:
					self->mux1.lock();
					begin = std::chrono::steady_clock::now();
					self->junoStats.update();
					end = std::chrono::steady_clock::now();
					self->mux1.unlock();
					deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
					deltaTime = std::chrono::milliseconds(250) - deltaTime > std::chrono::milliseconds(5) ? std::chrono::milliseconds(250) - deltaTime : std::chrono::milliseconds(5);
					break;
				case StatsDevice::VMachine:
									self->mux1.lock();
					begin = std::chrono::steady_clock::now();
					self->junoStats.update();
					end = std::chrono::steady_clock::now();
					self->mux1.unlock();
					deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
					deltaTime = std::chrono::milliseconds(250) - deltaTime > std::chrono::milliseconds(5) ? std::chrono::milliseconds(250) - deltaTime : std::chrono::milliseconds(5);
					break;
			}
			std::this_thread::sleep_for(deltaTime);	
		}
	}
		
	string StsService::retrieveUsage()
	{
		string temp="";
		switch(device)
		{
			case StatsDevice::Juno:
				mux1.lock();
				temp = junoStats.retrieveUsage();
				mux1.unlock();
				break;
			case StatsDevice::Xeon:
				break;
			case StatsDevice::VMachine:
				break;
		}	
		return temp;			
	}
	
	string StsService::retrieveSensors()
	{
		string temp="";
		switch(device)
		{
			case StatsDevice::Juno:	
				mux1.lock();
				temp = junoStats.retrieveSensors();
				mux1.unlock();
				break;
			case StatsDevice::Xeon:
				break;
			case StatsDevice::VMachine:
				break;
		}	
		return temp;			
	}