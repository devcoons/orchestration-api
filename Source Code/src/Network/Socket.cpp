#include "Socket.h"

Socket::Socket()
{
	
}

Socket::Socket(int hostPort)
{
	m_hostPort = hostPort;
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == -1) 
	{
		disconnect();
		return;
	}
	bool option = 1;
}

Socket::~Socket() 
{
	disconnect();
}

bool Socket::isOpen(void) 
{
	return m_sock != -1;
}

int Socket::currSocket(void) 
{
	return m_sock;
}

bool Socket::bind(void) 
{
	if (isOpen() == false)	
		return false;
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(m_hostPort);
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	if (::bind(m_sock, (sockaddr*)&my_addr, sizeof(my_addr)) == -1)
		return false;
	return true;
}

bool Socket::listen(void) 
{
	if (isOpen() == false)	return false;
	if (::listen(m_sock, 10) == -1) 
		return false;
	return true;
}

Parameters* Socket::accept(void)
{
	socklen_t addr_size = 0;
	addr_size = sizeof(sockaddr_in);
	sockaddr_in clientAddr;    
	int result =:: accept(m_sock, (sockaddr*)&clientAddr, &addr_size);
	Parameters * params = (Parameters*)malloc(sizeof(Parameters));
	params->clientSocket = result;
	params->clientAddr = clientAddr;
	return params;
}

void Socket::disconnect()
{
	if (m_sock == -1)return;
	close(m_sock);
	m_sock = -1;
}

bool Socket::connect(std::string host_name) 
{    
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(m_hostPort);
	memset(&(my_addr.sin_zero), 0, sizeof(my_addr.sin_zero));
	inet_pton(AF_INET, host_name.c_str(), &my_addr.sin_addr);
	if (::connect(currSocket(), (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) 
		return false; 
	return true;
}