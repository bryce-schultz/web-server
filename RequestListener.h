#pragma once

#include <string>

#include "WebCallback.h"

enum class RequestType { GET, POST };

struct RequestListener
{
	std::string path;
	WebCallback callback;
	RequestType type;
};