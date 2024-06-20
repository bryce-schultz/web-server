#include "SimpleAuth.h"

#include <cstdlib>  
#include <ctime> 
#include <iostream>

SimpleAuth::User::User():
    _name {},
    _hash {},
    _variables {},
    _authenticated {false}
{
}

SimpleAuth::User::User(const std::string& name, const std::string& hash):
    _name {name},
    _hash {hash},
    _variables {},
    _authenticated {true}
{
}

const std::string SimpleAuth::User::name() const
{
    return _name;
}

const std::string SimpleAuth::User::get(const std::string& key)
{
    if (_variables.find(key) == _variables.end())
    {
        return "";
    }

    return _variables[key];
}

const void SimpleAuth::User::set(const std::string& key, const std::string& value)
{
    _variables[key] = value;
}

const bool SimpleAuth::User::isAuthenticated()
{
    return false;
}

const bool SimpleAuth::User::hashMatch(const std::string& hash) const
{
    return (_hash == hash);
}

SimpleAuth::SimpleAuth():
    _accounts {},
    _sessions {}
{
    srand(time(0));
}

bool SimpleAuth::registerUser(const std::string& username, const std::string& password_hash)
{
    if (_accounts.find(username) != _accounts.end())
    {
        return false;
    }

    _accounts[username] = { username, password_hash };
    std::cout << "[SimpleAuth] user " << username << " created.\n";
    return true;
}

std::string SimpleAuth::login(const std::string& username, const std::string& password_hash)
{
    auto it = _accounts.find(username);
    if (it == _accounts.end())
    {
        return "";
    }

    User* user = &(it->second);

    if (!user->hashMatch(password_hash))
    {
        return "";
    }

    std::vector<std::string> used_tokens;
    for (auto& session : _sessions)
    {
        used_tokens.push_back(session.first);
    }

    std::string token = generateToken(TOKEN_LENGTH, used_tokens);

    _sessions.insert({ token, user });

    return token;
}

SimpleAuth::User* SimpleAuth::authorize(const std::string& token)
{
    if (_sessions.find(token) == _sessions.end())
    {
        return nullptr;
    }

    return _sessions[token];
}

void SimpleAuth::init()
{
    std::cout << "[SimpleAuth] initialized.\n";
}

void SimpleAuth::cleanup()
{
    std::cout << "[SimpleAuth] shutdown.\n";
}

std::string SimpleAuth::generateToken(int length, std::vector<std::string> excluded)
{
    std::string token;
    const static char *possible_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    bool collision = true;

    while (collision)
    {
        token.clear();
        for (int i = 0; i < length; ++i)
        {
            token += possible_chars[rand() % strlen(possible_chars)];
        }

        for (int i = 0; i < excluded.size(); ++i)
        {
            if (token == excluded[i]) continue;
        }

        collision = false;
    }

    return token;
}
