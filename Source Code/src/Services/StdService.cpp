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
						temporary += "{\"pid\":\"" + std::to_string((*app1)->sharedMemoryPtr->processID) +"\",\"name\":\"" + ((*app1)->sharedMemoryPtr->processName) +"\",\"goal\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getInitialGoalMs())+"\",\"min\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getMinimumGoalMs())+"\",\"max\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getMaximumGoalMs())+"\",\"averagems\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getAverageMs())+"\",\"currentms\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getCurrentMs())+"\",\"erroraverage\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getAverageError())+"\",\"errorcurrent\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getCurrentError())+"\",\"offset\":\""+std::to_string((*app1)->sharedMemoryPtr->tracker.getOffsetGoalMs())+"\"},";	
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
			else if(temp.find("goal")!= std::string::npos)
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
						}
						++app1;
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