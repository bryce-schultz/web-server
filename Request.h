#pragma once
#include <string>
#include <map>

class Request
{
public:
	friend class WebServer;
	Request(const std::string& content);

	std::string get(const std::string& key);
private:
	std::string _content;
	std::map<std::string, std::string> _variables;
	std::string _type;
	std::string _path;
	std::string _prot_ver;

	void parseContent();
};