#pragma once

#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

#define PACKET_SIZE 4096

class TcpListener
{
public:

	TcpListener(const char* ip_address, int port);

	int init();

	int run();

protected:

	virtual void onClientConnected(int client_socket);

	virtual void onClientDisconnected(int client_socket);

	virtual void onMessageReceived(int client_socket, const char* message, int length);

	void sendToClient(int client_socket, const char* message, int length);

	void broadcastToClients(int sending_client, const char* message, int length);

private:

	const char*		_ip_address;
	int				_port;
	int				_socket;
	fd_set			_master;
};

