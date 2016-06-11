#pragma once
	
	#include <string>
	#include <vector>
	#include <fcntl.h>
	#include <sstream>
	#include <iostream>
	#include <sys/mman.h>
	#include <semaphore.h>
	
	using namespace std;
	
	class StatsJuno
	{
		private:
			long double cpu_idle_0[2];
			long double cpu_idle_1[2];
			long double cpu_all_0[2];
			long double cpu_all_1[2];
			long double cpu_all_previous[2];
			long double cpu_idle_next[2];
			long double cpu_all_next[2];
			long double cpc_big;
			long double cpc_little;
			long double cpc_board;
			long double cpc_gpu;
			long double apc_big;
			long double apc_little;
			long double apc_board;
			long double apc_gpu;
			char buffer[2000];
			string temporary;
		public:
			StatsJuno();
			~StatsJuno();
			void update();
			string retrieveUsage();		
			string retrieveSensors();
			vector<string> split(string , char);		
	};