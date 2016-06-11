#include "OrchestrationServer.h"

namespace Orchestration
{
	Server::Server()
	{	
	}

	Server::~Server()
	{
	}

	void Server::initialize()
	{
		mux.lock();

		applications = std::vector<std::unique_ptr<ShmObject<AppContainer>>>();
		semaphores = std::vector<sem_t*>();
		mux.unlock();
	}

	void Server::beginServer(int port,int portStatistics)
	{
		long refreshRate = 25000;
		appService.Start(port,this);
		stdService.Start(portStatistics,this);
		
	}

	void Server::execute()
	{
		int i;
		active = true;

		while (active)
		{
			i = 0;
			mux.lock();
			for (auto app = applications.begin(); app != applications.end();)
			{		
				if ((*app)->sharedMemoryPtr->state == ApplicationState::DeregisterApplication)
				{
					std::cout << "Application :" << (*app)->sharedMemoryPtr->processID << " Deregistration Requested. [Removing shared pointer]" << std::endl;
					(*app)->Delete();
					
					applications.erase(applications.begin() + i);
					semaphores.erase(semaphores.begin() + i);
				}
				else if ((*app)->numberOfProcesses() <2)
				{
					std::cout << "Application :" << (*app)->sharedMemoryPtr->processID << " Unexpected Closed. [Removing shared pointer]" << std::endl;
					(*app)->Delete();
					applications.erase(applications.begin() + i);
					semaphores.erase(semaphores.begin() + i);
				}
				else if ((*app)->sharedMemoryPtr->state == ApplicationState::Pause)
				{
					ImplementationManager *ptr = (ImplementationManager*)shmat((*app)->sharedMemoryPtr->kernelManagers.sharedMemoryID, 0, 0);
					
					if ((*app)->sharedMemoryPtr->getCurrentProfiling() == true)
					{
						ptr[(*app)->sharedMemoryPtr->selectedKernelManager].decreaseProfiling();
					}
					else if ((*app)->sharedMemoryPtr->getNextProfiling() == true)
					{
						(*app)->sharedMemoryPtr->selectedKernelManager+=1;
						 ptr[(*app)->sharedMemoryPtr->selectedKernelManager].decreaseProfiling();
					}
					else
					{	
						switch((*app)->sharedMemoryPtr->policy)
						{
							case IndividualPolicyType::Balanced:
								IndividualPolicies::Balanced((*app)->sharedMemoryPtr,ptr);
								break;
							case IndividualPolicyType::Performance:
								IndividualPolicies::Performance((*app)->sharedMemoryPtr,ptr);
								break;
							case IndividualPolicyType::Restricted:
								IndividualPolicies::Restricted((*app)->sharedMemoryPtr,ptr);								
								break;
						}						
					}
					shmdt(ptr);	
					(*app)->sharedMemoryPtr->state = ApplicationState::Execute;
					sem_post(semaphores.at(i));
					++i;
					++app;
				}
				else
				{
					++i;
					++app;							
				}
			}
			mux.unlock();	
			usleep(25000);
		}
	}
}