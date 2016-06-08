#include "Container.h"
#include <iostream>
Container::Container()
{
	policy = PolicyType::Balanced;
	minThroughput = -1;
	maxThroughput = -1;
	goalThroughput = -1;
	currThroughput = 0;
	selectedManager = -1;
	counter = 0;
	state = ApplicationState::RegisterApplication;
	appName[0]='\0';
	lastThroughput=0;
	newgoal=0;
	avg_error=0;
	cur_error=0;
}


double Container::getCNAThroughput(void)
{
	return lastThroughput;
}

double Container::getNewGoal(void)
{
	return newgoal;
}

double Container::getAverageError(void)
{
	return avg_error;
}

double Container::getCurrentError(void)
{
	return cur_error;
}




Container::~Container()
{
	
}

	void Container::setAppName(std::string arg)
	{
		int i=0;
		for(i=0;i<strlen(arg.c_str());i++)
		{
			this->appName[i]=arg.c_str()[i];
		}
		this->appName[i]='\0';
	}
	
	std::string Container::getAppName()
	{
		std::string retvalue;
		retvalue = appName;
		return retvalue;
	}


void Container::execute(void * arg) 
{
	Manager* ptr = (Manager*)shmat(this->managers.sharedMemoryID, 0, 0);
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	ptr[selectedManager].function(arg);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	ptr[selectedManager].counter++;
	auto deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
	deltaTime = deltaTime / 1000;
	if (ptr[selectedManager].counter == 1)
		ptr[selectedManager].averageThroughput = deltaTime;
	else
		ptr[selectedManager].averageThroughput = ptr[selectedManager].averageThroughput / ptr[selectedManager].counter*(ptr[selectedManager].counter - 1) + deltaTime / ptr[selectedManager].counter;
	counter++;
	if (counter == 1)
	{
		currThroughput = deltaTime;
	  avg_error = std::abs(goalThroughput - lastThroughput);
	}
	else
	{		
		currThroughput = currThroughput / counter * (counter - 1) + deltaTime / counter;	
		 avg_error =  avg_error/counter *( counter-1) + std::abs(goalThroughput - currThroughput)/ counter;
	}
	lastThroughput=deltaTime;
	cur_error = std::abs(goalThroughput - lastThroughput);
	
		shmdt(ptr);	
}

void Container::setCounter(int arg)
{
	counter = arg;	
}

int Container::getCounter(void)
{
	return counter;
}

void Container::setPId(pid_t arg)
{
	pid = arg;
}

pid_t Container::getPId(void)
{
	return pid;
}

void Container::setPolicy(PolicyType arg)
{
	policy = arg;
}

PolicyType Container::getPolicy(void)
{
	return policy;
}

double Container::getMaxThroughput(void)
{
	return maxThroughput;
}

double Container::getMinThroughput(void)
{
	return minThroughput;
}

void Container::setMaxThroughput(double arg)
{
	maxThroughput = arg;
}

void Container::setMinThroughput(double arg)
{
	minThroughput = arg;
}	

double Container::getCurrThroughput(void)
{
	return currThroughput;
}

double Container::getGoalThroughput(void)
{
	return goalThroughput;
}	

void Container::setCurrThroughput(double arg)
{
	currThroughput = arg;
}

void Container::setGoalThroughput(double arg)
{
	newgoal =arg;
	goalThroughput = arg;
}

ApplicationState Container::getState(void)
{
	return state;
}

void Container::setState(ApplicationState arg)
{
	state = arg;
}

void Container::setProfiling(int arg)
{
	Manager* ptr = (Manager*)shmat(this->managers.sharedMemoryID, 0, 0);
	for (int i = 0; i < this->managers.getSize(); i++)
		ptr[i].profiling = arg;
	shmdt(ptr);	
}

ShmList<Manager>* Container::getManagers()
{
	return &managers;	
}


void Container::registerImplementation(ImplementationKernel function0, ImplementationType type)
{
	this->managers.insertElement();
	Manager* ptr = (Manager*)shmat(this->managers.sharedMemoryID, 0, 0);
	ptr[this->managers.getSize() - 1] = Manager();
	ptr[this->managers.getSize() - 1].function = function0;
	ptr[this->managers.getSize() - 1].type = type;
	ptr[this->managers.getSize() - 1].counter=0;
	ptr[this->managers.getSize() - 1].profiling=0;
	ptr[this->managers.getSize() - 1].currentThroughput=0;
	ptr[this->managers.getSize() - 1].averageThroughput=0;
	shmdt(ptr);	
}

void Container::deregisterImplementation(ImplementationKernel function, ImplementationType type)
{
	Manager* ptr = (Manager*)shmat(this->managers.sharedMemoryID, 0, 0);
	for (int i = 0; i < this->managers.getSize(); i++)
		if (ptr[this->managers.getSize() - 1].type == type)
			if (ptr[this->managers.getSize() - 1].function.target_type() == function.target_type())
			{
				this->managers.removeElement(i);
				break;
			}	
shmdt(ptr);				
}

volatile int Container::getSelectedManagerIndex(void)
{
	return selectedManager.load(std::memory_order_acquire);
}
void Container::setSelectedManagerIndex(volatile int arg)
{
	selectedManager.store(arg, std::memory_order_release);
}

bool Container::getCurrentProfiling()
{
	Manager* ptr = (Manager*)shmat(this->managers.sharedMemoryID, 0, 0);
	int profiling =  ptr[getSelectedManagerIndex()].profiling;
	shmdt(ptr);	
	if (profiling > 0)
		return true;
	return false;
}

bool Container::getNextProfiling()
{
	if (getSelectedManagerIndex() + 1 >= this->managers.getSize())
		return false;
		Manager* ptr = (Manager*)shmat(this->managers.sharedMemoryID, 0, 0);
	int profiling =  ptr[getSelectedManagerIndex()+1].profiling;
	shmdt(ptr);	
	if (profiling > 0)
		return true;
	return false;
}

void Container::decreaseProfiling()
{
	Manager* ptr = (Manager*)shmat(this->managers.sharedMemoryID, 0, 0);
	ptr[getSelectedManagerIndex()+1].profiling--;;
	shmdt(ptr);	
}