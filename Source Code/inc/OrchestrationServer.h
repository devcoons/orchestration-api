#pragma once

	#include <vector>
	#include <chrono>	
	#include <fcntl.h>
	#include <sstream>
	#include <signal.h>
	#include <unistd.h>
	#include <iostream>
	#include <sys/mman.h>
	#include <semaphore.h>
	#include <sys/resource.h>
	#include "AppService.h"
	#include "StdService.h"
	#include "StsService.h"
	#include "AppContainer.h"
	#include "GlobalPolicies.h"

	using namespace std;
	
	namespace Orchestration
	{
		class Server
		{ 
			private:
				AppService appService;
				StdService stdService;
				
			public:
				double policyGoal;
				GlobalPolicyType globalPolicy;
				mutex mux;
				StsService stsService;
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