#pragma once

	#include <mutex>
	#include <thread>
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
		
	class StatsService
	{
		private:
			StatsDevice device;
			StatsJuno junoStats; 	
			
		public:	
			StatsService();
			~StatsService();

			void start(StatsDevice);
			string retrieveUsage();
			string retrieveSensors();
	};