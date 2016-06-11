#pragma once
	
	#include <string>
	#include <vector>
	#include <fcntl.h>
	#include <sstream>
	#include <iostream>
	#include <sys/mman.h>
	#include <semaphore.h>
	#include <algorithm> 
	
	using namespace std;
	
	class StatsJuno
	{
		private:
			long double cpuBig[4];
			long double cpuLITTLE[4];
			long double cpuBigCurrent;
			long double cpuLITTLECurrent;
			long double cpuBigAverage;
			long double cpuLITTLEAverage;
			long long passesCounter;
			
			char buffer[2000];
			string temporary;
		public:
			StatsJuno();
			~StatsJuno();
			void update();
			string retrieveUsage();		
			string retrieveSensors();
			void updateUsage();
			void updateSensors();
			vector<string> split(string , char);		
	};