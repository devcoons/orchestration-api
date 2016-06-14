#pragma once

	#include <mutex>
	#include <thread>
	#include <vector>
	#include <chrono>
	#include <unistd.h>
	#include <iostream>
	
	#include <algorithm>
	
	#include "StatsJuno.h"
	#include "StatsXeon.h"
	#include "StatsVMachine.h"
	#include "ImplementationManager.h"
	
	using namespace std;
	
	typedef enum
	{
		Juno,
		Xeon,
		VMachine
	} 
	StatsDevice;
		
	class StsService
	{
		private:
			bool active;
			long refreshRate;
			mutex privateMutex;
			StatsDevice device;
			StatsJuno junoStats;
			
		public:
			mutex mux1;
			mutex mux2;
			
			StsService();
			~StsService();
			void execution();			
			void start(StatsDevice);
			string retrieveUsage();
			string retrieveSensors();
			static void update(void *);
			double getFilteredPowerAvg();
			double getFilteredCurrentAvg();
			double getFilterUtilizationAvg();
	};