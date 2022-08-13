#include <iostream>

#include "WebServer.h"
#include "HTMLGenerator.h"


int main()
{
	WebServer app("0.0.0.0", 8080);

	if (app.init() != 0)
	{
		return 1;
	}

	app.get("/", [&](Req req, Res res)
		{
			res.redirect("index.html");
		}
	);

	app.get("/agent", [&](Req req, Res res)
		{
			res.send(req.get("User-Agent"));
		}
	);

	app.get("/users/:id", [&](Req req, Res res)
		{
			res.send("Hello, " + req.get("id"));
		}
	);

	app.serve("public/");

	app.run();

	return 0;
}