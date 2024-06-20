#include "Request.h"
#include "StringUtils.h"

#include <iostream>
#include <vector>

Request::Request(const std::string& content):
	_content { content }
{
	parseContent();
}

std::string Request::get(const std::string& key)
{
	auto value = _variables.find(key);

	if (value != _variables.end())
	{
		return (*value).second;
	}

	return "";
}

const std::map<std::string, std::string> Request::variables() const
{
	return _variables;
}

const std::string Request::type() const
{
	return _type;
}

const std::string Request::getToken() const
{
	return _token;
}

const std::string Request::getBody() const
{
	return _body;
}

void Request::parseContent()
{
	std::vector<std::string> header_body = str::split(_content, "\r\n\r\n");
	std::vector<std::string> header = str::split(header_body[0], "\r\n");

	if (header_body.size() > 1)
	{
		_body = header_body[1];
	}

	std::vector<std::string> head = str::split(header[0]);

	_type = head[0];
	_path = head[1];
	_prot_ver = head[2];

	header.erase(header.begin());

	for (auto& var : header)
	{
		std::pair<std::string, std::string> req_vars = str::splitKeyValuePair(var);

		if (req_vars.first == "Authorization")
		{
			_token = req_vars.second;
		}

		if (req_vars.first != "") _variables.emplace(req_vars);
	}
}

void Request::addVariable(const std::string& key, const std::string& value)
{
	_variables.emplace(key, value);
}
