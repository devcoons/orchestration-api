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
			pid_t processID;
			char processName[100];
			ApplicationState state;
			Semaphore semaphore;
			AppTracker tracker;
			mutex atomicityMutex;
			
			IndividualPolicyType policy;
			ShmList<ImplementationManager> kernelManagers;
			int selectedKernelManager;
			
			AppContainer();
			~AppContainer();
	
			bool getCurrentProfiling();
			bool getNextProfiling();
			void decreaseProfiling();
			void execute(void * );
			void setProfiling(int );
			ShmList<ImplementationManager>* getManagers();
			
			void registerImplementation(ImplementationKernel, ImplementationType);
			void deregisterImplementation(ImplementationKernel, ImplementationType);
			
	};