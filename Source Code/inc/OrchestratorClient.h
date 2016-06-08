#pragma once
	#include <vector>
	#include <fcntl.h>
	#include <iostream>
	#include <semaphore.h>
	#include <sys/mman.h>
	#include <sstream>
	
	#include "Container.h"
	#include "Client.h"

	using namespace std;
		
	namespace Orchestrator 
	{
		class Client
		{
			private:
				int port;
				sem_t *semaphore;
				string hostname;
				shared_ptr<ShmObject<Container>> container;
				
			public:
				~Client();
				Client(pid_t);
				void execute(void*);
				void disconnect(void);
				void setPriority(int);
				void setProfiling(int);
				void setAppName(string);
				void connect(string, int);
				double getCurrentMs(void);
				IndividualPolicyType getPolicy(void);
				void setPolicy(IndividualPolicyType);
				void setGoalMs(double, double, double);
				void registerImplementation(ImplementationKernel, ImplementationType);
				void deregisterImplementation(ImplementationKernel, ImplementationType);
		};		
	}