#include "OrchestrationClient.h"
#include <stdio.h>
#include <stdlib.h>
namespace Orchestration
{

	Client::Client(pid_t arg)
	{
		container = std::shared_ptr<ShmObject<AppContainer>> (new ShmObject<AppContainer>());
		container->sharedMemoryPtr->processID = arg;
		semaphore = sem_open((std::to_string(arg)).c_str(), O_CREAT, 0644, 1);
	}

	Client::~Client()
	{
		
	}

	void Client::execute(void* arg)
	{
		while (container->sharedMemoryPtr->state != ApplicationState::Execute)
			sem_wait(semaphore);

		if (container->sharedMemoryPtr->selectedKernelManager != -1)
			container->sharedMemoryPtr->execute(arg);
		
		container->sharedMemoryPtr->state = ApplicationState::Pause;
	}

	void Client::setAppName(char * _value)
	{
		strcpy(container->sharedMemoryPtr->processName,_value);
	}

	void Client::setPolicy(IndividualPolicyType _value)
	{
		container->sharedMemoryPtr->policy = _value;
	}

	void Client::setGoalMs(double _goalMs, double _minMs, double _maxMs)
	{
		container->sharedMemoryPtr->tracker.setInitialGoalMs(_goalMs);
		container->sharedMemoryPtr->tracker.setMinimumGoalMs(_minMs);
		container->sharedMemoryPtr->tracker.setMaximumGoalMs(_maxMs);
	}

	void Client::connect(std::string host, int port)
	{
		this->hostname = host;
		this->port = port;
		::Client client(host, port);
		container->sharedMemoryPtr->state = ApplicationState::RegisterApplication;
		client.send(std::to_string(container->sharedMemoryID));
		client.close();
	}
	
	void Client::disconnect()
	{
		container->sharedMemoryPtr->state = ApplicationState::DeregisterApplication;
	}
	
	void Client::registerImplementation(ImplementationKernel function,ImplementationType type)
	{
		container->sharedMemoryPtr->state = ApplicationState::RegisterApplication;
		container->sharedMemoryPtr->registerImplementation(function,type);
	}
	
	void Client::deregisterImplementation(ImplementationKernel function, ImplementationType type)
	{
		container->sharedMemoryPtr->state = ApplicationState::DeregisterApplication;
		container->sharedMemoryPtr->deregisterImplementation(function, type);
	}
	
	double  Client::getCurrentMs()
	{
		return container->sharedMemoryPtr->tracker.getCurrentMs();
	}
	
	double  Client::getAverageMs()
	{
		return container->sharedMemoryPtr->tracker.getAverageMs();
	}
	
	void Client::setProfiling(int arg)
	{
		container->sharedMemoryPtr->state = ApplicationState::Pause;
		container->sharedMemoryPtr->setProfiling(arg);
	}
}