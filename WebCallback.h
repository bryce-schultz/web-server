#pragma once
#include "Request.h"
#include "Response.h"
#include "Callback.h"

typedef Response& Res;
typedef Request& Req;

typedef Callback<Req, Res> WebCallback;