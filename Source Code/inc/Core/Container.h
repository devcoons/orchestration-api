#pragma once
#include "Policy.h"
#include "ShmList.h"
#include "Manager.h"
#include "ShmObject.h"
#include "Semaphore.h"
#include <thread>
#include <algorithm>
#include <mutex>
#include <atomic>

typedef enum
{
	RegisterApplication,
	RegisterImplementation,
	DeregisterApplication,
	DeregisterImplementation,
	Execute,
	Pause
}   
ApplicationState;


class Container
{
	private:

		
	protected:
		
	public:
		pid_t pid;
		std::mutex smux;
		char appName[100];
		Semaphore semaphore;
		volatile double counter;
		ShmList<Manager> managers;
		volatile PolicyType policy;
		volatile double maxThroughput;
		volatile double minThroughput;
		volatile double currThroughput;
		volatile double goalThroughput;
		volatile double lastThroughput;
		volatile double newgoal;
		volatile double avg_error;
		volatile double cur_error;
		volatile ApplicationState state;
		std::atomic<int> selectedManager;

		Container();
		~Container();	
		double getCNAThroughput(void);
		double getNewGoal(void);
		double getCurrentError(void);
		double getAverageError(void);
		pid_t getPId(void);
		void setPId(pid_t);
		void execute(void*);
		void setCounter(int);
		int getCounter(void);
		void setProfiling(int);	
		bool getNextProfiling();
		std::string getAppName();
		void decreaseProfiling();
		PolicyType getPolicy(void);
		void setPolicy(PolicyType);
		bool getCurrentProfiling();
		void setAppName(std::string);
		void setMaxThroughput(double);
		double getMaxThroughput(void);
		double getMinThroughput(void);
		void setMinThroughput(double);	
		void setCurrThroughput(double);
		void setGoalThroughput(double);
		double getCurrThroughput(void);
		double getGoalThroughput(void);	
		ApplicationState getState(void);
		void setState(ApplicationState);
		ShmList<Manager>* getManagers();
		volatile int getSelectedManagerIndex(void);
		void setSelectedManagerIndex(volatile int);
		void registerImplementation(ImplementationKernel, ImplementationType);
		void deregisterImplementation(ImplementationKernel, ImplementationType);
};

