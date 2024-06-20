#include "TcpListener.h"
#include <iostream>
#include <string>
#include <sstream>

TcpListener::TcpListener(const char* ip_address, int port): 
	_ip_address { ip_address }, 
	_port { port },
	_master { }
{ }

int TcpListener::init()
{
	WSADATA ws_data;
	WORD version= MAKEWORD(2, 2);

	int ws_ok = WSAStartup(version, &ws_data);
	if (ws_ok != 0)
	{
		return ws_ok;
	}

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
	{
		return WSAGetLastError();
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(_port);
	inet_pton(AF_INET, _ip_address, &hint.sin_addr);

	if (bind(_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	FD_ZERO(&_master);
	FD_SET(_socket, &_master);

	return 0;
}

int TcpListener::run()
{
	bool running = true;

	std::cout << "Web server listening on http://localhost:" << _port << "\n";

	while (running)
	{
		fd_set copy = _master;

		int socket_count = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socket_count; i++)
		{
			SOCKET socket = copy.fd_array[i];

			if (socket == _socket)
			{
				SOCKET client = accept(_socket, nullptr, nullptr);

				FD_SET(client, &_master);

				onClientConnected(client);
			}
			else
			{
				char buffer[PACKET_SIZE];
				ZeroMemory(buffer, PACKET_SIZE);

				int bytes_in = recv(socket, buffer, PACKET_SIZE, 0);
				if (bytes_in <= 0)
				{
					onClientDisconnected(socket);
					closesocket(socket);
					FD_CLR(socket, &_master);
				}
				else
				{
					onMessageReceived(socket, buffer, bytes_in);
				}
			}
		}
	}

	FD_CLR(_socket, &_master);
	closesocket(_socket);

	while (_master.fd_count > 0)
	{
		SOCKET socket = _master.fd_array[0];

		FD_CLR(socket, &_master);
		closesocket(socket);
	}

	WSACleanup();
	return 0;
}

void TcpListener::onClientConnected(int client_socket)
{

}

void TcpListener::onClientDisconnected(int client_socket)
{

}

void TcpListener::onMessageReceived(int client_socket, const char* message, int length)
{

}

void TcpListener::sendToClient(int client_socket, const char* message, int length)
{
	send(client_socket, message, length, 0);
}

void TcpListener::broadcastToClients(int sending_client, const char* message, int length)
{
	for (int i = 0; i < _master.fd_count; i++)
	{
		SOCKET out_socket = _master.fd_array[i];
		if (out_socket != _socket && out_socket != sending_client)
		{
			sendToClient(out_socket, message, length);
		}
	}
}
