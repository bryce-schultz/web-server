#pragma once

#include <map>
#include <string>
#include <vector>

#include "Middleware.h"

#define AUTH if (!auth.authorize(req.getToken())) \
			 {									  \
			     res.status(401).send();          \
		     }

class SimpleAuth : public Middleware
{
private:
	class User
	{
	public:
		User();
		User(const std::string& name, const std::string& hash);

		const std::string name() const;
		const std::string get(const std::string& key);
		const void set(const std::string& key, const std::string& value);
		const bool isAuthenticated();
		const bool hashMatch(const std::string& hash) const;
	private:
		std::string _name;
		std::string _hash;
		std::map<std::string, std::string> _variables;
		bool _authenticated;
	};

public:
	SimpleAuth();

	bool registerUser(const std::string& username, const std::string& password_hash);
	std::string login(const std::string& username, const std::string& password_hash);

	User* authorize(const std::string& token);

	virtual void init() override;
	virtual void cleanup() override;
private:
	std::string generateToken(int length, std::vector<std::string> excluded = {});
private:
	std::map<std::string, User> _accounts;
	std::map<std::string, User*> _sessions;

	const int TOKEN_LENGTH { 32 };
};

