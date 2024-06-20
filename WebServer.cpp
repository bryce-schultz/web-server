#include "WebServer.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>

#include "HTMLGenerator.h"
#include "StringUtils.h"
#include "ContentResolver.h"

WebServer::WebServer(const char* ip_address, int port):
	TcpListener(ip_address, port)
{ }

void WebServer::get(const std::string& path, const WebCallback& callback)
{
	if (!str::contains(":", path))
	{
		_listeners.emplace(path, callback);
		return;
	}
	
	std::string base_path = str::clip(str::find(":", path), path);
	std::string dynamic_path = str::trim(str::find(":", path) + 1, path);

	_dynamicListeners.emplace(base_path, callback);
	_variables.emplace(base_path, dynamic_path);
}

void WebServer::post(const std::string& path, const WebCallback& callback)
{
	if (!str::contains(":", path))
	{
		_listeners.emplace(path, callback);
		return;
	}

	std::string base_path = str::clip(str::find(":", path), path);
	std::string dynamic_path = str::trim(str::find(":", path) + 1, path);

	_dynamicListeners.emplace(base_path, callback);
	_variables.emplace(base_path, dynamic_path);
}

void WebServer::serve(const std::string& path)
{
	serveDirectory(path, path);
}

void WebServer::use(Middleware& middleware)
{
	middleware.init();
	_middleware.push_back(&middleware);
}

void WebServer::onClientConnected(int client_socket)
{
}

void WebServer::onClientDisconnected(int client_socket)
{
}

void WebServer::onMessageReceived(int client_socket, const char* message, int length)
{
	// convert message to a string.
	std::string content = std::string(message, length);

	// TODO: parseRequestedURI should return a Request with a uri inside instead, 
	// it can then also do the parameter variables as well.
	// rename it parseRequest
	std::string requested_uri = parseRequestedURI(message);

	Request req(content);
	Response res(*this, client_socket);

	removeAndAddQueryParameterVariables(requested_uri, req);

	for (Middleware* middleware : _middleware)
	{
		middleware->request(req, res);
	}

	auto listener = _listeners.find(requested_uri);
	if (listener != _listeners.end())
	{
		(*listener).second(req, res);
	}

	for (auto& dyn_listener : _dynamicListeners)
	{
		if (str::contains(dyn_listener.first, requested_uri))
		{
			std::pair<std::string, std::string> var = (*_variables.find(dyn_listener.first));

			std::string value = str::removeFirst(dyn_listener.first, requested_uri);
			req._variables.emplace(var.second, value);
			dyn_listener.second(req, res);
		}
	}

	if (res.getStatus() == 404)
	{
		std::string error_message = HTMLGenerator::generateTag("h1", "404 Page Not Found!");
		std::string message = Response::generateResponseHeader(404, error_message.length(), "text/html");
		message.append(error_message);
		sendToClient(client_socket, message.c_str(), message.size());
	}
}

std::string WebServer::parseRequestedURI(const std::string& request)
{
	auto parts = str::split(request);
	return parts[1];
}

Request WebServer::parseRequest(const std::string& message, const int length)
{
	// TODO: implement this.
	return Request(message);
}

void WebServer::removeAndAddQueryParameterVariables(std::string& uri, Request& request)
{
	// strip the params off the uri.
	std::string params_string = parseParams(uri);

	// there were no params so return.
	if (params_string.empty())
	{
		return;
	}

	// split the params.
	auto params = str::split(params_string, "&");

	// loop over the params and add them to the variables.
	for (auto& param : params)
	{
		auto key_value_pair = str::splitKeyValuePair(param, "=");

		// if we don't have a valid key value pair, skip to next pair.
		if (key_value_pair.first == "") continue;

		// add the key value pair to the request variables.
		request.addVariable(key_value_pair.first, key_value_pair.second);
	}
}

std::string WebServer::parseParams(std::string& uri)
{
	// check for parameters using find.
	auto params_start = str::find("?", uri);

	// if we don't find the parameter character (?), there aren't params.
	if (params_start == str::NPOS)
	{
		return "";
	}

	// we might have params, remove the ? and return the params string.
	std::string params_string = str::subString(params_start + 1, uri.length(), uri);
	uri = str::subString(str::START, params_start, uri);

	return params_string;
}

void WebServer::serveDirectory(const fs::path& path, const std::string& base_dir)
{
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			serveDirectory(entry.path(), base_dir);
		}
		else if (entry.is_regular_file())
		{
			serveFile(entry.path(), base_dir);
		}
	}
}

void WebServer::serveFile(const fs::path& path, const std::string& base_dir)
{
	std::string uri = "/" + str::removeFirst(base_dir, path.generic_string());
	
	get(uri, [path](Req req, Res res)
		{
			std::ifstream file(path, std::ios::in | std::ios::binary);
			std::string extention = path.extension().generic_string();

			if (!file.is_open() || !file.good()) return;

			std::stringstream buffer;
			buffer << file.rdbuf();

			res.send(buffer.str(), ContentResolver::resolveType(extention));

			file.close();
		}
	);
}
