#pragma once

	#include <mutex>
	#include <thread>
	#include <atomic>
	#include <iostream>
	#include <algorithm>
	
	#include "ShmList.h"	
	#include "AppTracker.h"
	#include "ShmObject.h"
	#include "Semaphore.h"
	#include "IndividualPolicies.h"
	#include "ImplementationManager.h"

	using namespace std;
	
	typedef enum
	{
		RegisterApplication,
		RegisterImplementation,
		DeregisterApplication,
		DeregisterImplementation,
		Execute,
		Restart,
		Stop,
		Pause
	}   
	ApplicationState;

	class AppContainer
	{
		private:

		protected:
			
		public:
			int priority;
			pid_t processID;
			AppTracker tracker;
			Semaphore semaphore;
			char processName[100];
			mutex atomicityMutex;
			ApplicationState state;
			int selectedKernelManager;
			IndividualPolicyType policy;
			ShmList<ImplementationManager> kernelManagers;
			
			AppContainer();
			~AppContainer();
			
			void execute(void * );
			void setProfiling(int );
			bool getNextProfiling();
			void decreaseProfiling();
			bool getCurrentProfiling();
			
			ShmList<ImplementationManager>* getManagers();
			
			void registerImplementation(ImplementationKernel, ImplementationType);
			void deregisterImplementation(ImplementationKernel, ImplementationType);
	};