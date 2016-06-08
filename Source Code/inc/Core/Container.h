#pragma once
	#include <mutex>
	#include <thread>
	#include <atomic>
	#include <iostream>
	#include "ShmList.h"
	#include "Manager.h"
	#include <algorithm>
	#include "AppStats.h"
	#include "ShmObject.h"
	#include "Semaphore.h"
	#include "IndividualPolicies.h"

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

	class Container
	{
		private:

		protected:
			
		public:
			pid_t processID;
			string processName;
			ApplicationState state;
			Semaphore semaphore;
			AppStats statistics;
			mutex atomicityMutex;
			
			IndividualPolicyType policy;
			ShmList<Manager> kernelManagers;
			int selectedKernelManager;
			
			Container();
			~Container();
	
			bool getCurrentProfiling();
			bool getNextProfiling();
			void decreaseProfiling();
			void execute(void * );
			void setProfiling(int );
			ShmList<Manager>* getManagers();
			
			void registerImplementation(ImplementationKernel, ImplementationType);
			void deregisterImplementation(ImplementationKernel, ImplementationType);
			
	};