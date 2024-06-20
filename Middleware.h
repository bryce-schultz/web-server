#pragma once

#include <string>
#include "Request.h"
#include "Response.h"

class Middleware
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void request(Request& req, Response& res) = 0;
};

