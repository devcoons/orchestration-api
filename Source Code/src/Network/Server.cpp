#include "Server.h"

	Server::Server()
	{
		
	}

	void Server::initialize(int port) 
	{
		this->port = port;
	}

	void Server::assignCallback(void(*function)(Parameters *, void*), void* self)
	{
		this->callbackFunction = function;
		this->callbackObject = self;
	}

	void Server::execute()
	{
		std::thread applicationServerThread(&Server::execution, this);
		applicationServerThread.detach();
	}

	void Server::execution()
	{
		Socket socket(port);

		if (!socket.isOpen()) 	{	socket.disconnect(); exit(1); }
		if (!socket.bind()) 	{	socket.disconnect(); exit(1); } 
		if (!socket.listen()) 	{	socket.disconnect(); exit(1); }
		
		while (true) 
		{      
			Parameters * params = socket.accept(); 
			if (params->clientSocket != -1) 
			{   
				struct in_addr inaddr = params->clientAddr.sin_addr;
				char buf[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &inaddr, buf, sizeof(buf));
				std::thread t(&Server::callback, this, params);
				t.detach();					
			}
			else 
			{
				free(params);
			}
		}
	}

	void Server::callback(Parameters * p)
	{
		if (Server::callbackFunction != NULL)
			Server::callbackFunction(p, this->callbackObject);
			
		free(p);
	}