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

void Request::parseContent()
{
	std::vector<std::string> header = str::split(_content, "\r\n");

	std::vector<std::string> head = str::split(header[0]);

	_type = head[0];
	_path = head[1];
	_prot_ver = head[2];

	header.erase(header.begin());

	for (auto var : header)
	{
		std::pair<std::string, std::string> req_vars = str::splitKeyValuePair(var);
		if (req_vars.first != "") _variables.emplace(req_vars);
	}
}
