#pragma once
#include "TcpListener.h"

class MultiClientListener : public TcpListener
{
public:

	MultiClientListener(const char* ip_address, int port);

protected:

	virtual void onClientConnected(int client_socket);

	virtual void onClientDisconnected(int client_socket);

	virtual void onMessageReceived(int client_socket, const char* message, int length);
};

