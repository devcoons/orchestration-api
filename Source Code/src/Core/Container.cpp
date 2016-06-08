#include "Container.h"

Container::Container()
{
	processID = -1;
	selectedKernelManager = -1;
	processName = std::string();
	statistics.setOffsetGoalMs(0);
	statistics.setInitialGoalMs(0);	
	statistics.setCurrentMs(0);
	statistics.setAverageMs(0);
	statistics.setMinimumGoalMs(0);
	statistics.setMaximumGoalMs(0);
	statistics.setCurrentError(0);
	statistics.setAverageError(0);
	statistics.setPassesCounter(0);
	policy = IndividualPolicyType::Balanced;
	state = ApplicationState::RegisterApplication;
}

Container::~Container()
{
	
}

void Container::execute(void * arg) 
{
	Manager* ptr = (Manager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	ptr[selectedKernelManager].function(arg);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	auto deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
	deltaTime = deltaTime / 1000;
	
	ptr[selectedKernelManager].increasePassesCounter();	
	ptr[selectedKernelManager].setCurrentMs(deltaTime);
	ptr[selectedKernelManager].setAverageMs(deltaTime);
		
	statistics.increasePassesCounter();
	statistics.setCurrentMs(deltaTime);
	statistics.setAverageMs(deltaTime);
	statistics.setCurrentError(deltaTime);
	statistics.setAverageError(deltaTime);
	
	shmdt(ptr);	
}

void Container::setProfiling(int arg)
{
	Manager* ptr = (Manager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	for (int i = 0; i < this->kernelManagers.getSize(); i++)
		ptr[i].setProfiling(arg);
	
	shmdt(ptr);	
}

ShmList<Manager>* Container::getManagers()
{
	return &kernelManagers;	
}

void Container::registerImplementation(ImplementationKernel _function, ImplementationType type)
{
	this->kernelManagers.insertElement();
	Manager* ptr = (Manager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	ptr[this->kernelManagers.getSize() - 1] = Manager();
	ptr[this->kernelManagers.getSize() - 1].function = _function;
	ptr[this->kernelManagers.getSize() - 1].type = type;
	ptr[this->kernelManagers.getSize() - 1].setPassesCounter(0);
	ptr[this->kernelManagers.getSize() - 1].setProfiling(0);
	ptr[this->kernelManagers.getSize() - 1].setCurrentMs(0);
	ptr[this->kernelManagers.getSize() - 1].setAverageMs(0);
	
	shmdt(ptr);	
}

void Container::deregisterImplementation(ImplementationKernel function, ImplementationType type)
{
	Manager* ptr = (Manager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	for (int i = 0; i < this->kernelManagers.getSize(); i++)
		if (ptr[this->kernelManagers.getSize() - 1].type == type)
			if (ptr[this->kernelManagers.getSize() - 1].function.target_type() == function.target_type())
			{
				this->kernelManagers.removeElement(i);
				break;
			}
			
	shmdt(ptr);				
}


bool Container::getCurrentProfiling()
{
	Manager* ptr = (Manager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);	
	int profiling =  ptr[selectedKernelManager].getProfiling();	
	shmdt(ptr);	
	return profiling > 0 ? true : false;
}

bool Container::getNextProfiling()
{
	if (selectedKernelManager + 1 >= this->kernelManagers.getSize())
		return false;
	
	Manager* ptr = (Manager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);	
	int profiling =  ptr[selectedKernelManager+1].getProfiling();	
	shmdt(ptr);		
	return profiling > 0 ? true : false;
}

void Container::decreaseProfiling()
{
	Manager* ptr = (Manager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);	
	ptr[selectedKernelManager+1].decreaseProfiling();	
	shmdt(ptr);	
}