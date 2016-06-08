#include "Client.h"


Client::Client(std::string _hostname, int _port)
{
	this->port = _port;
	this->host = _hostname;
}

int Client::send(std::string message)
{
	Socket socket = Socket(port);
	if (!socket.connect(host))
		return -1;
	size_t buffer_len = message.length();
	char buffer[buffer_len]; 
	memset(buffer, 0, buffer_len);
	strcpy(buffer, message.c_str());
	size_t bytecount;
	if (::send(socket.currSocket(), buffer, buffer_len, 0) == -1)
		return -1;
	return 0;
}

std::string Client::receive()
{
	char buffer[255]; 
	size_t bytecount;
	bytecount = ::recv(socket.currSocket(), buffer, 255, 0);
	if (bytecount == -1)
		return NULL;
	return buffer;
}

void Client::close()
{
	socket.disconnect();
}
