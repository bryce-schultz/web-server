#pragma once
#include <string>

class HTMLGenerator
{
public:
	static std::string generateTag
	(
		const std::string& tag = "div",
		const std::string& content = "",
		const std::string& cssId = "",
		const std::string& cssClass = ""
	);
};

