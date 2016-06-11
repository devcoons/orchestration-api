#include "OrchestrationServer.h"

namespace Orchestration
{
	Server::Server()
	{	
		active = true;
		setpriority(PRIO_PROCESS, getpid(), -20);
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

	void Server::beginServer(int port,int portStsService)
	{
		long refreshRate = 25000;
		appService.Start(port,this);
		stdService.Start(portStsService,this);	
		stsService.start(StatsDevice::Juno);
	}

	void Server::execute()
	{
		int i;
		double offsetDecreasing=0;
		std::chrono::steady_clock::time_point begin;
		std::chrono::steady_clock::time_point end;
		auto deltaTime = std::chrono::milliseconds(50);
		
		while (true)
		{
			i = 0;
			
			mux.lock();			
			if(active == false)
				break;
			begin = std::chrono::steady_clock::now();
			for (auto app = applications.begin(); app != applications.end();)
			{		
				if ((*app)->sharedMemoryPtr->state == ApplicationState::DeregisterApplication)
				{
					std::cout << "Application :" << (*app)->sharedMemoryPtr->processID << " Deregistration Requested. [Removing shared pointer]" << std::endl;
					(*app)->Delete();		
					applications.erase(applications.begin() + i);
					semaphores.erase(semaphores.begin() + i);
				}
				else if ((*app)->sharedMemoryPtr->state == ApplicationState::Stop)
				{
					std::cout << "Application :" << (*app)->sharedMemoryPtr->processID << " Stop Requested. [Removing shared pointer]" << std::endl;
					kill((*app)->sharedMemoryPtr->processID, SIGKILL);
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
						if((*app)->sharedMemoryPtr->tracker.getOffsetGoalMs()!=0)
						{
							offsetDecreasing = (*app)->sharedMemoryPtr->tracker.getOffsetGoalMs()/(priority+25)<5;
							if((*app)->sharedMemoryPtr->tracker.getOffsetGoalMs()>0)
							{
								offsetDecreasing = offsetDecreasing < 5 ? 5 : offsetDecreasing;
								offsetDecreasing = (*app)->sharedMemoryPtr->tracker.getOffsetGoalMs() - offsetDecreasing < 0 ? 0 : (*app)->sharedMemoryPtr->tracker.getOffsetGoalMs() - offsetDecreasing; 						
							}
							else
							{
								offsetDecreasing = offsetDecreasing > -5 ? -5 : offsetDecreasing;
								offsetDecreasing = (*app)->sharedMemoryPtr->tracker.getOffsetGoalMs() - offsetDecreasing > 0 ? 0 : (*app)->sharedMemoryPtr->tracker.getOffsetGoalMs() - offsetDecreasing; 									
							}
							(*app)->sharedMemoryPtr->tracker.setOffsetGoalMs(offsetDecreasing);
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
			end = std::chrono::steady_clock::now();	
			mux.unlock();
			deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
			deltaTime = std::chrono::milliseconds(50) - deltaTime > std::chrono::milliseconds(10) ? std::chrono::milliseconds(50) - deltaTime : std::chrono::milliseconds(10);		
			std::this_thread::sleep_for(deltaTime);			
		}
	}
}