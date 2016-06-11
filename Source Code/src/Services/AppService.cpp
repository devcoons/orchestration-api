#include "AppService.h"
#include "OrchestrationServer.h"

	AppService::AppService()
	{
		
	}

	AppService::~AppService()
	{
		
	}

	void AppService::Start(int _port, void * object)
	{
		port = _port;
		server.initialize(port);
		server.assignCallback(AppService::callback, object);	
		server.execute();
	}

	void AppService::callback(Parameters * parameters, void * object)
	{
		size_t bytecount;
		char buffer[1024];
		int buffer_len = 1024;
		Orchestration::Server * self = static_cast<Orchestration::Server*>(object);	
		memset(buffer, 0, buffer_len);
		bytecount = recv(parameters->clientSocket, buffer, buffer_len, 0);
		std::string temp = buffer;	
		self->mux.lock();
		self->applications.push_back(std::unique_ptr<ShmObject<AppContainer>>(new ShmObject<AppContainer>(std::stoi(temp))));	
		self->semaphores.push_back(sem_open((std::to_string((self->applications.back())->sharedMemoryPtr->processID)).c_str(), 0));
		std::cout << "Application :" << (self->applications.back())->sharedMemoryPtr->processID << " Registered. [Adding shared pointer]" << std::endl;
		self->mux.unlock();
	}
