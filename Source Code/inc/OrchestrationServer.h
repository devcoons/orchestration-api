#pragma once

	#include <vector>	
	#include <fcntl.h>
	#include <sstream>
	#include <iostream>
	#include <sys/mman.h>
	#include <semaphore.h>
	
	#include "AppService.h"
	#include "StdService.h"
	#include "AppContainer.h"


	using namespace std;
	
	namespace Orchestration
	{
		class Server
		{ 
			private:
				AppService appService;
				StdService stdService;

			public:	
				mutex mux;

				long long counter;
				uint32_t loopRate;
				bool active;
				Server();
				~Server();
				void execute(void);
				void initialize(void);
				void beginServer(int,int);
				
				vector<unique_ptr<ShmObject<AppContainer>>> applications;	
				vector<sem_t*> semaphores;
		};
	}