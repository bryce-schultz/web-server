#pragma once
#include <string>
#include <map>

class Request
{
public:
	friend class WebServer;
	Request(const std::string& content);

	std::string get(const std::string& key);
	const std::map<std::string, std::string> variables() const;
	const std::string type() const;
	const std::string getToken() const;
	const std::string getBody() const;
private:
	std::string _content;
	std::map<std::string, std::string> _variables;
	std::string _type;
	std::string _path;
	std::string _prot_ver;
	std::string _token;
	std::string _body;

	void parseContent();
	void addVariable(const std::string& key, const std::string& value);
};