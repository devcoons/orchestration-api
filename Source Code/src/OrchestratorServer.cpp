#include "OrchestratorServer.h"

namespace Orchestrator
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
		cpu_idle_0[0]=0;
		cpu_all_0[0]=0;
		cpu_idle_0[1]=0;
		cpu_all_0[1]=0;
		cpu_idle_1[0]=0;
		cpu_all_1[0]=0;
		cpu_idle_1[1]=0;
		cpu_all_1[1]=0;
		applications = std::vector<std::unique_ptr<ShmObject<Container>>>();
		semaphores = std::vector<sem_t*>();
		mux.unlock();
	}

	void Server::beginServer(int port,int portStatistics)
	{
		serverStatistics.initialize(portStatistics);
		serverStatistics.assignCallback(Statistics::Callback,this);
		server.initialize(port);
		server.assignCallback(Orchestrator::Server::callback, this);	
		serverStatistics.execute();
		server.execute();
	}

	void Server::execute()
	{
		int i;
		int newIndex;
		int index;
		double newDeltaThroughput;
		double newDeltaThroughput1;
		active = true;
		while (active)
		{
			i = 0;
			mux.lock();
			for (auto app = applications.begin(); app != applications.end();)
			{
			
				if ((*app)->sharedMemoryPtr->getState() == ApplicationState::DeregisterApplication)
				{
					std::cout << "Application :" << (*app)->sharedMemoryPtr->pid << " Deregistration Requested. [Removing shared pointer]" << std::endl;
					(*app)->Delete();
					
					applications.erase(applications.begin() + i);
					semaphores.erase(semaphores.begin() + i);
				}
				else if ((*app)->numberOfProcesses() <2)
				{
					std::cout << "Application :" << (*app)->sharedMemoryPtr->pid << " Unexpected Closed. [Removing shared pointer]" << std::endl;
					(*app)->Delete();
					applications.erase(applications.begin() + i);
					semaphores.erase(semaphores.begin() + i);
				}
				else if ((*app)->sharedMemoryPtr->getState() == ApplicationState::Pause)
				{
					if ((*app)->sharedMemoryPtr->getCurrentProfiling() == true)
					{
						(*app)->sharedMemoryPtr->decreaseProfiling();
					}
					else if ((*app)->sharedMemoryPtr->getNextProfiling() == true)
					{
						index = (*app)->sharedMemoryPtr->getSelectedManagerIndex();
						(*app)->sharedMemoryPtr->setSelectedManagerIndex(index + 1);
						(*app)->sharedMemoryPtr->decreaseProfiling();
					}
					else
					{	
						Manager* ptr = (Manager*)shmat((*app)->sharedMemoryPtr->managers.sharedMemoryID, 0, 0);
						newIndex = (*app)->sharedMemoryPtr->getSelectedManagerIndex();
						newDeltaThroughput = fabs((*app)->sharedMemoryPtr->getGoalThroughput() - ((*app)->sharedMemoryPtr->getCurrThroughput() / (*app)->sharedMemoryPtr->getCounter() *((*app)->sharedMemoryPtr->getCounter() - 1) + ptr[newIndex].averageThroughput / ((*app)->sharedMemoryPtr->getCounter())));
						newDeltaThroughput1=0;
						switch ((*app)->sharedMemoryPtr->getPolicy())
						{
						case PolicyType::Balanced:		
						
							for (int j = 0; j < (*app)->sharedMemoryPtr->getManagers()->getSize(); j++)
							{
								if (ptr[j].averageThroughput == 0)
								{
									newIndex = j;
									break;
								}
								newDeltaThroughput1 =  fabs((*app)->sharedMemoryPtr->getGoalThroughput() - ((*app)->sharedMemoryPtr->getCurrThroughput() / (*app)->sharedMemoryPtr->getCounter() *((*app)->sharedMemoryPtr->getCounter() - 1) + ptr[j].averageThroughput / ((*app)->sharedMemoryPtr->getCounter())));
								if (newDeltaThroughput1 < newDeltaThroughput)
								{
									newIndex = j;	
									newDeltaThroughput = newDeltaThroughput1;						
								}
							}
							
							(*app)->sharedMemoryPtr->setSelectedManagerIndex(newIndex);
					
							break;
						case PolicyType::Performance:
							
						
						
						
							break;
						case PolicyType::Powersaving:
							break;
						}
						shmdt(ptr);	
					}
						
					
					
					std::cout << "App :: [";
					std::cout.width(6);
					std::cout << (*app)->sharedMemoryPtr->getPId();
					std::cout << "]  Selected Manager :: [";
					std::cout.width(3); 
					std::cout << (*app)->sharedMemoryPtr->selectedManager + 1;
					std::cout << "]  Profiling :: [";
					std::cout.width(4);
					std::cout << (*app)->sharedMemoryPtr->getCurrentProfiling();
					std::cout << "]" <<	std::endl;	
					(*app)->sharedMemoryPtr->setState(ApplicationState::Execute);
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

	void Server::callback(Parameters * parameters, void * object)
	{
		size_t bytecount;
		char buffer[1024];
		int buffer_len = 1024;
		Orchestrator::Server * self = static_cast<Orchestrator::Server*>(object);	
		memset(buffer, 0, buffer_len);
		bytecount = recv(parameters->clientSocket, buffer, buffer_len, 0);
		std::string temp = buffer;	
		self->mux.lock();
		self->applications.push_back(std::unique_ptr<ShmObject<Container>>(new ShmObject<Container>(std::stoi(temp))));	
		self->semaphores.push_back(sem_open((std::to_string((self->applications.back())->sharedMemoryPtr->getPId())).c_str(), 0));

		std::cout << "Application :" << (self->applications.back())->sharedMemoryPtr->getPId() << " Registered. [Adding shared pointer]" << std::endl;
		self->mux.unlock();
	}


}