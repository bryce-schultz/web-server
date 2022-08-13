#pragma once
#include "TcpListener.h"
#include "Callback.h"
#include "Request.h"
#include "Response.h"

#include <filesystem>
#include <map>
#include <string>

namespace fs = std::filesystem;

class Response;
class Request;

typedef Response& Res;
typedef Request& Req;

typedef Callback<Req, Res> WebCallback;

class WebServer : public TcpListener
{
	friend class Response;
public:

	WebServer(const char* ip_address, int port);

	void get(const std::string& path, const WebCallback& callback);

	void serve(const std::string& path);

protected:

	virtual void onClientConnected(int client_socket);

	virtual void onClientDisconnected(int client_socket);

	virtual void onMessageReceived(int client_socket, const char* message, int length);

private:

	std::map<std::string, WebCallback> _listeners;
	std::map<std::string, WebCallback> _dynamicListeners;
	std::map<std::string, std::string> _variables;

	std::string parseRequestedURI(const std::string& request);

	void serveDirectory(const fs::path& path, const std::string& base_dir);
	void serveFile(const fs::path& path, const std::string& base_dir);
};

