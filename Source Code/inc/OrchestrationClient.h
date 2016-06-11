#pragma once

	#include <vector>	
	#include <fcntl.h>
	#include <stdio.h>
	#include <sstream>
	#include <stdlib.h>
	#include <iostream>
	#include <sys/mman.h>
	#include <semaphore.h>
	#include <sys/resource.h>
	
	#include "Client.h"
	#include "AppContainer.h"
	

	using namespace std;
		
	namespace Orchestration 
	{
		class Client
		{
			private:
				int port;
				sem_t *semaphore;
				string hostname;
				shared_ptr<ShmObject<AppContainer>> container;
				
			public:
				~Client();
				Client(pid_t);
				void execute(void*);
				void disconnect(void);
				void setPriority(int);
				void setProfiling(int);
				void setAppName(char *);
				void connect(string, int);
				double getCurrentMs(void);
				double getAverageMs(void);
				IndividualPolicyType getPolicy(void);
				void setPolicy(IndividualPolicyType);
				void setGoalMs(double, double, double);
				void registerImplementation(ImplementationKernel, ImplementationType);
				void deregisterImplementation(ImplementationKernel, ImplementationType);
		};		
	}