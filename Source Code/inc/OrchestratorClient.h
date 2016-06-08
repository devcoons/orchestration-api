#pragma once
	#include <vector>
	#include <fcntl.h>
	#include <iostream>
	#include <semaphore.h>
	#include <sys/mman.h>
	#include <sstream>
	
	#include "Container.h"
	#include "Client.h"


namespace Orchestrator 
{
	class Client
	{
		private:
			int port;
			std::string hostname;
			
			sem_t *semaphore;
			std::shared_ptr<ShmObject<Container>> container;
			
		public:
			Client(pid_t);
			~Client();
			void execute(void*);
			void disconnect(void);
			void setProfiling(int);
			PolicyType getPolicy(void);
			void setPolicy(PolicyType);
			double getThroughput(void);
			void connect(std::string, int);
			void setThroughput(double, double, double);
			void registerImplementation(ImplementationKernel, ImplementationType);
			void deregisterImplementation(ImplementationKernel, ImplementationType);
			void setAppName(std::string);
	};		
}