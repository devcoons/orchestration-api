#pragma once
	
	#include <string>
	#include <vector>
	#include <fcntl.h>
	#include <sstream>
	#include <fstream>
	#include <iostream>
	#include <algorithm>
	#include <sys/mman.h>
	#include <semaphore.h> 
	#include "FstKalman.h"
	
	using namespace std;
	
	class StatsJuno
	{
		private:
			long double cpuBig[4];
			long double cpuLITTLE[4];
			long double cpuBigCurrent;
			long double cpuLITTLECurrent;
			long double cpuAllCurrent;
			long long passesCounter;
			FstKalman cpuBigKalman;
			FstKalman cpuLittleKalman;
			FstKalman cpuAllKalman;
			char buffer[2000];
			string temporary;
			string sysData[8][2];
			ifstream  fileStream1,fileStream2;
			std::stringstream sstream1;
			std::stringstream sstream2;
			FstKalman sysAllKalman[8];

		public:
			StatsJuno();
			~StatsJuno();
			void update();
			
			void updateUsage();
			void updateSensors();
			string retrieveUsage();		
			string retrieveSensors();	
			double getFilteredPowerAvg();
			double getFilteredCurrentAvg();
			double getFilterUtilizationAvg();
			vector<string> split(string , char);
	};