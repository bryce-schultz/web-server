#include "MultiClientListener.h"
#include <string>

MultiClientListener::MultiClientListener(const char* ip_address, int port) :
	TcpListener(ip_address, port) 
{ }

void MultiClientListener::onClientConnected(int client_socket)
{
	std::string welcome_message = "Welcome to the Chat Server!\r\n";
	sendToClient(client_socket, welcome_message.c_str(), welcome_message.size() + 1);
}

void MultiClientListener::onClientDisconnected(int client_socket)
{

}

void MultiClientListener::onMessageReceived(int client_socket, const char* message, int length)
{
	broadcastToClients(client_socket, message, length);
}
