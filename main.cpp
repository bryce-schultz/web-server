#include <iostream>

#include "WebServer.h"
#include "HTMLGenerator.h"
#include "SimpleAuth.h"
#include "StringUtils.h"


int main()
{
	WebServer app("0.0.0.0", 8080);
	SimpleAuth auth;

	if (app.init() != 0)
	{
		return 1;
	}

	app.get("/", [&](Req req, Res res)
		{
			res.redirect("index.html");
		}
	);

	app.get("/profile", [&](Req req, Res res)
		{
			res.redirect("profile.html");
		}
	);

	app.get("/signup", [&](Req req, Res res)
		{
			res.redirect("signup.html");
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

	app.get("/variables", [&](Req req, Res res)
		{
			AUTH;

			std::string response;

			for (auto& current_request : req.variables())
			{
				response.append(current_request.first + ": " + current_request.second + "\n");
			}

			res.send(response);
		}
	);

	app.post("/register", [&](Req req, Res res)
		{
			std::string response;
			
			auto form_data = str::split(req.getBody(), "&");

			if (form_data.size() != 2)
			{
				res.status(400).send("Invalid request.");
			}

			auto username_pair = str::splitKeyValuePair(form_data[0], "=");

			if (username_pair.first != "username")
			{
				res.status(400).send("Invalid request.");
			}

			auto password_pair = str::splitKeyValuePair(form_data[1], "=");

			if (password_pair.first != "password")
			{
				res.status(400).send("Invalid request.");
			}

			if (auth.registerUser(username_pair.second, password_pair.second))
			{
				res.redirect("/");
			}

			res.redirect("/register");
		}
	);

	app.post("/login", [&](Req req, Res res)
		{
			std::string response;

			auto form_data = str::split(req.getBody(), "&");

			if (form_data.size() != 2)
			{
				res.status(400).send("Invalid request.");
			}

			auto username_pair = str::splitKeyValuePair(form_data[0], "=");

			if (username_pair.first != "username")
			{
				res.status(400).send("Invalid request.");
			}

			auto password_pair = str::splitKeyValuePair(form_data[1], "=");

			if (password_pair.first != "password")
			{
				res.status(400).send("Invalid request.");
			}

			std::string token = auth.login(username_pair.second, password_pair.second);

			if (token.empty())
			{
				res.redirect("/");
			}

			std::string script = 
				"function setCookie(cname, cvalue, exdays)										\
				 {																				\
					const d = new Date();														\
					d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));					\
					let expires = \"expires=\" + d.toUTCString();								\
					document.cookie = cname + \"=\" + cvalue + \";\" + expires + \";path=/\";	\
				 }																			    \
				 setCookie(\"token\", \"" + token + "\", 1);									\
				 window.location = \"/profile\";											    \
				";

			res.send(HTMLGenerator::generateTag("script", script), "text/html");
		}
	);

	app.use(auth);

	app.serve("public/");

	app.run();

	return 0;
}