#include "ContentResolver.h"

std::string ContentResolver::resolveType(const std::string& ext)
{
	if (ext == ".pdf")
	{
		return "application/pdf";
	}
	else if (ext == ".html")
	{
		return "text/html";
	}
	else if (ext == ".png")
	{
		return "image/png";
	}
	else if (ext == ".css")
	{
		return "text/css";
	}
	else if (ext == ".js")
	{
		return "text/javascript";
	}
	else if (ext == ".xml")
	{
		return "text/xml";
	}
	else
	{
		return "text/plain";
	}
}
