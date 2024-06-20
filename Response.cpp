#include "Response.h"
#include "WebServer.h"
#include "StringUtils.h"

Response::Response(WebServer& server, int client_socket): 
	_server { server },
	_stream { },
	_status { 404 },
	_client_socket { client_socket }
{ }

Response& Response::send()
{
	buildResponse("", "text/plain");

	_server.sendToClient(_client_socket, _stream.str().c_str(), _stream.str().size());

	return *this;
}

Response& Response::send(const std::string& message, const std::string& content_type)
{
	if (_status == 404)
	{
		_status = 200;
	}

	buildResponse(message, content_type);

	_server.sendToClient(_client_socket, _stream.str().c_str(), _stream.str().size());

	return *this;
}

Response& Response::status(int status)
{
	_status = status;

	return *this;
}

Response& Response::redirect(const std::string& uri)
{
	std::string response = generateRedirect(RedirectType::PERMINANT, uri);

	_status = 300;

	_server.sendToClient(_client_socket, response.c_str(), response.size());

	return *this;
}

const int Response::getStatus() const
{
	return _status;
}

void Response::buildResponse(const std::string& message, const std::string& content_type)
{
	_stream << generateResponseHeader(_status, message.length(), content_type);
	
	_stream << message;
}

std::string Response::generateResponseHeader(int status, int content_size, const std::string& content_type)
{
	std::ostringstream responce;
	responce << "HTTP/1.1 " << status << " " << statusToString(status) << "\r\n";
	responce << "Cache-Control: no-cache, private\r\n";
	responce << "Content-Type: " << content_type << "\r\n";
	responce << "Content-Length: " << content_size << "\r\n";
	responce << "\r\n";

	return responce.str();
}

std::string Response::generateResponse(const std::string& content, const std::string& content_type)
{
	std::string responce = generateResponseHeader(200, content.length(), content_type);
	responce.append(content);
	return responce;
}

std::string Response::generateRedirect(RedirectType type, const std::string& location)
{
	switch (type)
	{
	case RedirectType::TEMPORARY:
		return ("HTTP/1.1 302: Found\r\nLocation: " + location + "\r\n\r\n");
	case RedirectType::PERMINANT:
		return ("HTTP/1.1 301: Moved Permanently\r\nLocation: " + location + "\r\n\r\n");
	}

	return "";
}

const std::string& Response::statusToString(int status)
{
	const static std::string _200 = "OK";
	const static std::string _404 = "Not Found";
	const static std::string _500 = "Server Error";

	switch (status)
	{
		case 200:
			return _200;
		case 404:
			return _404;
		default:
			return _500;
	}
}
