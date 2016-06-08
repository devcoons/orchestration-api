#include "OrchestratorClient.h"

namespace Orchestrator
{
	/*
	
	*/
	Client::Client(pid_t arg)
	{
		container = std::shared_ptr<ShmObject<Container>> (new ShmObject<Container>());
		container->sharedMemoryPtr->setPId(arg);
		semaphore = sem_open((std::to_string(arg)).c_str(), O_CREAT, 0644, 1);
	}

	/*
	
	*/	
	Client::~Client()
	{
		
	}

	/*
	
	*/
	void Client::execute(void* arg)
	{
		while (container->sharedMemoryPtr->getState() != ApplicationState::Execute)
			sem_wait(semaphore);

		if (container->sharedMemoryPtr->getSelectedManagerIndex() != -1)
			container->sharedMemoryPtr->execute(arg);
		
		container->sharedMemoryPtr->setState(ApplicationState::Pause);
	}

	/*
	
	*/
	void Client::setAppName(std::string arg)
	{
		container->sharedMemoryPtr->setAppName(arg);
	}

	/*
	
	*/
	void Client::setPolicy(PolicyType policy)
	{
		container->sharedMemoryPtr->setPolicy(policy);
	}

	/*
	
	*/
	void Client::setThroughput(double goal, double min, double max)
	{
		container->sharedMemoryPtr->setMinThroughput(min);
		container->sharedMemoryPtr->setMaxThroughput(max);
		container->sharedMemoryPtr->setGoalThroughput(goal);
	}

	/*
	
	*/
	void Client::connect(std::string host, int port)
	{
		this->hostname = host;
		this->port = port;
		::Client client(host, port);
		container->sharedMemoryPtr->setState(ApplicationState::RegisterApplication);
		client.send(std::to_string(container->sharedMemoryID));
		client.close();
	}
	
	/*
	
	*/
	void Client::disconnect()
	{
		container->sharedMemoryPtr->setState(ApplicationState::DeregisterApplication);
	}
	
	/*
	
	*/
	void Client::registerImplementation(ImplementationKernel function,ImplementationType type)
	{
		container->sharedMemoryPtr->setState(ApplicationState::RegisterApplication);
		container->sharedMemoryPtr->registerImplementation(function,type);
	}
	
	/*
	
	*/
	void Client::deregisterImplementation(ImplementationKernel function, ImplementationType type)
	{
		container->sharedMemoryPtr->setState(ApplicationState::DeregisterApplication);
		container->sharedMemoryPtr->deregisterImplementation(function, type);
	}
	
	/*
	
	*/
	double  Client::getThroughput()
	{
		return container->sharedMemoryPtr->getCurrThroughput();
	}
	
	/*
	
	*/
	void Client::setProfiling(int arg)
	{
		container->sharedMemoryPtr->setState(ApplicationState::Pause);
		container->sharedMemoryPtr->setProfiling(arg);
	}
}