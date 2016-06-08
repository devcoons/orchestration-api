#pragma once
	#include <vector>
	#include <fcntl.h>
	#include <iostream>
	#include <semaphore.h>
	#include <sys/mman.h>
	#include <sstream>
	
	#include "Container.h"
	#include "Server.h"
	#include "Statistics.h"

	using namespace std;
	
	namespace Orchestrator
	{
		class Server
		{ 
			private:
				mutex mux;
				static void callback(Parameters *, void *);
				static void statisticscallback(Parameters *, void *);
				::Server server;
				::Server serverStatistics;

			public:	
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
				long long counter;
				uint32_t loopRate;
				bool active;
				Server();
				~Server();
				void execute(void);
				void initialize(void);
				void beginServer(int,int);
				vector<unique_ptr<ShmObject<Container>>> applications;	
				vector<sem_t*> semaphores;
		};
	}