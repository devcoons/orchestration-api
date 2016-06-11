#include "AppContainer.h"

AppContainer::AppContainer()
{
	processID = -1;
	selectedKernelManager = -1;
	processName[0] = '\0';
	tracker.setOffsetGoalMs(0);
	tracker.setInitialGoalMs(0);	
	tracker.setCurrentMs(0);
	tracker.setMinimumGoalMs(0);
	tracker.setMaximumGoalMs(0);
	tracker.setPassesCounter(0);
	tracker.updateCurrentError();
	tracker.updateAverageError();
	policy = IndividualPolicyType::Balanced;
	state = ApplicationState::RegisterApplication;
}

AppContainer::~AppContainer()
{
	
}

void AppContainer::execute(void * arg) 
{
	if(selectedKernelManager < 0)
		return;
	ImplementationManager* ptr = (ImplementationManager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	ptr[selectedKernelManager].function(arg);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	auto deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
	deltaTime = deltaTime / 1000;
	
	ptr[selectedKernelManager].increasePassesCounter();	
	ptr[selectedKernelManager].setCurrentMs(deltaTime);
	ptr[selectedKernelManager].setAverageMs(deltaTime);
		
	tracker.increasePassesCounter();
	tracker.setCurrentMs(deltaTime);
	tracker.setAverageMs(deltaTime);
	tracker.updateCurrentError();
	tracker.updateAverageError();
	
	shmdt(ptr);	
}

void AppContainer::setProfiling(int arg)
{
	ImplementationManager* ptr = (ImplementationManager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	for (int i = 0; i < this->kernelManagers.getSize(); i++)
		ptr[i].setProfiling(arg);
	
	shmdt(ptr);	
}

ShmList<ImplementationManager>* AppContainer::getManagers()
{
	return &kernelManagers;	
}

void AppContainer::registerImplementation(ImplementationKernel _function, ImplementationType type)
{
	this->kernelManagers.insertElement();
	ImplementationManager* ptr = (ImplementationManager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	ptr[this->kernelManagers.getSize() - 1] = ImplementationManager();
	ptr[this->kernelManagers.getSize() - 1].function = _function;
	ptr[this->kernelManagers.getSize() - 1].type = type;
	ptr[this->kernelManagers.getSize() - 1].setPassesCounter(0);
	ptr[this->kernelManagers.getSize() - 1].setProfiling(0);
	ptr[this->kernelManagers.getSize() - 1].setCurrentMs(0);
	ptr[this->kernelManagers.getSize() - 1].setAverageMs(0);
	
	shmdt(ptr);	
}

void AppContainer::deregisterImplementation(ImplementationKernel function, ImplementationType type)
{
	ImplementationManager* ptr = (ImplementationManager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);
	
	for (int i = 0; i < this->kernelManagers.getSize(); i++)
		if (ptr[this->kernelManagers.getSize() - 1].type == type)
			if (ptr[this->kernelManagers.getSize() - 1].function.target_type() == function.target_type())
			{
				this->kernelManagers.removeElement(i);
				break;
			}
			
	shmdt(ptr);				
}


bool AppContainer::getCurrentProfiling()
{
	if(selectedKernelManager <0)
		return false;
	ImplementationManager* ptr = (ImplementationManager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);	
	int profiling =  ptr[selectedKernelManager].getProfiling();	
	shmdt(ptr);	
	return profiling > 0 ? true : false;
}

bool AppContainer::getNextProfiling()
{
	if(selectedKernelManager <0)
		return false;
	
	if (selectedKernelManager + 1 >= this->kernelManagers.getSize())
		return false;
	
	ImplementationManager* ptr = (ImplementationManager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);	
	int profiling =  ptr[selectedKernelManager+1].getProfiling();	
	shmdt(ptr);		
	return profiling > 0 ? true : false;
}

void AppContainer::decreaseProfiling()
{
	ImplementationManager* ptr = (ImplementationManager*)shmat(this->kernelManagers.sharedMemoryID, 0, 0);	
	ptr[selectedKernelManager+1].decreaseProfiling();	
	shmdt(ptr);	
}