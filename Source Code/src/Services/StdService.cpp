#include "StdService.h"
#include "OrchestrationServer.h"
	
	StdService::StdService()
	{
		
	}

	StdService::~StdService()
	{
		
	}

	void StdService::Start(int _port, void * object)
	{
		port = _port;
		server.initialize(port);
		server.assignCallback(&StdService::callback, object);	
		server.execute();
	}
	
	vector<string> StdService::split(string str, char delimiter) 
	{
		vector<string> internal;
		stringstream ss(str);
		string tok;
  
		while(getline(ss, tok, delimiter))
			internal.push_back(tok);
		
		return internal;
	}
	
	void StdService::callback(Parameters * parameters, void * object)
	{
		std::cout<<"[ Watcher ] : Connection Opened"<<std::endl;
		size_t bytecount;
		char buffer[4055];
		int buffer_len = 4055;
		std::string temporary;
		Orchestration::Server * self = static_cast<Orchestration::Server*>(object);
		int n;
		while(1)
		{
			temporary.clear();
			memset(buffer, 0, buffer_len);
			bytecount = recv(parameters->clientSocket, buffer, buffer_len, 0);
			std::string temp = buffer;	
			if(temp=="terminate" || bytecount<=0 )
			{
				self->mux.lock();
					self->active = false;
				self->mux.unlock();
				break;
			}
			else if(temp=="appstats")
			{
				temporary="[";
				self->mux.lock();
					for (auto app1 = self->applications.begin(); app1 != self->applications.end();)
					{
						temporary += "{\"pid\":\"" + std::to_string((*app1)->sharedMemoryPtr->processID) +"\",\"name\":\"" + ((*app1)->sharedMemoryPtr->processName)+"\",\"initgoal\":\"" + std::to_string((*app1)->sharedMemoryPtr->tracker.getInitialGoalMs()) +"\",\"goal\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getInitialGoalMs()+(*app1)->sharedMemoryPtr->tracker.getOffsetGoalMs())+"\",\"min\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getMinimumGoalMs())+"\",\"max\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getMaximumGoalMs())+"\",\"averagems\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getAverageMs())+"\",\"currentms\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getCurrentMs())+"\",\"erroraverage\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getAverageError())+"\",\"errorcurrent\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getCurrentError())+"\",\"offset\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getOffsetGoalMs())+"\"},";	
						++app1;
					}
				self->mux.unlock();
				if(temporary.substr( temporary.length()-1,1)==",")
				temporary = temporary.substr(0, temporary.length()-1);
				temporary+="]";
				n = write(parameters->clientSocket,temporary.c_str(),temporary.length());			
			}
			else if(temp=="sysstats")
			{
				temporary = self->stsService.retrieveSensors();
				n = write(parameters->clientSocket,temporary.c_str(),temporary.length());
			}
			else if(temp=="cpuusage")
			{
				temporary = self->stsService.retrieveUsage();
				n = write(parameters->clientSocket,temporary.c_str(),temporary.length());		
			}
			else if(temp.find("setms")!= std::string::npos)
			{
				std::vector<std::string> vect;
				std::stringstream ss(temp);
				std::string ls;
				std::string word;
				while( std::getline(ss, word, ':') )
					vect.push_back(word);
				self->mux.lock();
					for (auto app1 = self->applications.begin(); app1 != self->applications.end();)
					{
						for(int ll=0;ll<vect.size();ll++)
						if(((*app1)->sharedMemoryPtr->processName) == vect.at(ll))
						{
							(*app1)->sharedMemoryPtr->tracker.setInitialGoalMs(std::stod(vect.at(ll+1)));
							(*app1)->sharedMemoryPtr->tracker.setMinimumGoalMs(std::stod(vect.at(ll+2)));
							(*app1)->sharedMemoryPtr->tracker.setMaximumGoalMs(std::stod(vect.at(ll+3)));
						}
						++app1;
					}
				self->mux.unlock();
			}
			else if(temp.find("setpriority")!= std::string::npos)
			{
				std::vector<std::string> vect;
				std::stringstream ss(temp);
				std::string ls;
				std::string word;
				while( std::getline(ss, word, ':') )
					vect.push_back(word);
				self->mux.lock();
					for (auto app1 = self->applications.begin(); app1 != self->applications.end();)
					{
						for(int ll=0;ll<vect.size();ll++)
						if(((*app1)->sharedMemoryPtr->processName) == vect.at(ll))
						{
							(*app1)->sharedMemoryPtr->priority = std::stod(vect.at(ll+1));
						}
						++app1;
					}
				self->mux.unlock();
			}
			else if(temp.find("setipolicy")!= std::string::npos)
			{
				std::vector<std::string> vect;
				std::stringstream ss(temp);
				std::string ls;
				std::string word;
				while( std::getline(ss, word, ':') )
					vect.push_back(word);
				self->mux.lock();
					for (auto app1 = self->applications.begin(); app1 != self->applications.end();)
					{
						for(int ll=0;ll<vect.size();ll++)
						if(((*app1)->sharedMemoryPtr->processName) == vect.at(ll))
						{
							if(vect.at(ll+1) == "Restricted")
								(*app1)->sharedMemoryPtr->policy = (IndividualPolicyType::Restricted);
							else
								(*app1)->sharedMemoryPtr->policy = (IndividualPolicyType::Balanced);
						}
						++app1;
					}
				self->mux.unlock();
			}	
			else if(temp=="getgpolicy")
			{
				temporary ="";
				self->mux.lock();
				switch(self->globalPolicy)
				{
					case GlobalPolicyType::Free:
						temporary = "{\"gpolicy\":\"free\",\"value\":\""+std::to_string(self->policyGoal)+"\"}";
						break;
					case GlobalPolicyType::PowerBalancing:
						temporary = "{\"gpolicy\":\"power\",\"value\":\""+std::to_string(self->policyGoal)+"\"}";
						break;
					case GlobalPolicyType::CurrentBalancing:
						temporary = "{\"gpolicy\":\"current\",\"value\":\""+std::to_string(self->policyGoal)+"\"}";
						break;
					case GlobalPolicyType::UtilizationBalancing:
						temporary = "{\"gpolicy\":\"util\",\"value\":\""+std::to_string(self->policyGoal)+"\"}";
						break;					
				}
				self->mux.unlock();
				temporary = self->stsService.retrieveUsage();
				n = write(parameters->clientSocket,temporary.c_str(),temporary.length());		
			}			
			else if(temp.find("setgpolicy")!= std::string::npos)
			{
				std::vector<std::string> vect;
				std::stringstream ss(temp);
				std::string ls;
				std::string word;
				while( std::getline(ss, word, ':') )
					vect.push_back(word);
				self->mux.lock();

				if(vect.at(1) == "Free")
				{
					self->globalPolicy = GlobalPolicyType::Free;
					self->policyGoal = 0;
				}
				if(vect.at(1) == "PowerBalancing")
				{
					self->globalPolicy = GlobalPolicyType::PowerBalancing;
					self->policyGoal = stod(vect.at(2));
				}
				if(vect.at(1) == "CurrentBalancing")
				{
					self->globalPolicy = GlobalPolicyType::CurrentBalancing;
					self->policyGoal = stod(vect.at(2));
				}
				if(vect.at(1) == "UtilizationBalancing")
				{
					self->globalPolicy = GlobalPolicyType::UtilizationBalancing;
					self->policyGoal = stod(vect.at(2));
				}
				
				self->mux.unlock();
			}
			else if(temp.find("stop")!= std::string::npos)
			{
				std::vector<std::string> vect;
				std::stringstream ss(temp);
				std::string ls;
				std::string word;
				while( std::getline(ss, word, ':') )
					vect.push_back(word);
			
				for (auto app1 = self->applications.begin(); app1 != self->applications.end();)
				{
					for(int ll=0;ll<vect.size();ll++)
					if(((*app1)->sharedMemoryPtr->processName) == vect.at(ll))
					{
						(*app1)->sharedMemoryPtr->state = ApplicationState::Stop;
					}
					++app1;
				}
			}
			else
			{
				n =write(parameters->clientSocket,"Unknown Command",15);
			}	
			usleep(1500);
		}
	}
