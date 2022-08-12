#pragma once
#include <string>
#include <sstream>

#include "WebServer.h"

class WebServer;

class Response
{
public:
	const enum class RedirectType { TEMPORARY, PERMINANT };

	Response(WebServer& server, int client_socket);

	Response& send(const std::string& message, const std::string& content_type = "text/plain");
	Response& status(int status);
	Response& redirect(const std::string& uri);

	const int getStatus() const;

	static std::string generateResponseHeader(int status = 404, int content_size = 0, const std::string& content_type = "text/plain");
	static std::string generateResponse(const std::string& content, const std::string& content_type = "text/plain");
	static std::string generateRedirect(RedirectType type, const std::string& location);
	static const std::string& statusToString(int status);

private:
	WebServer& _server;
	std::ostringstream _stream;
	int _status;
	int _client_socket;

	void buildResponse(const std::string& message, const std::string& content_type = "text/plain");
};