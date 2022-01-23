#pragma once
#include<thread>
#include<functional>
#include<memory>
#include"Handler.h"
#include"NetworkServer.h"
#include"KsXstr.hpp"
#include"Request.h"
#include"Parser.h"
class DroselServer
{
private:
	std::unique_ptr<NetworkServer> server;
	std::unordered_map < std::string, std::function<void(Request&, Response&)>> callables;
public:
	void OnPath(const std::string& path, std::function<void(Request&, Response&)> callable);
	void RunServer(const std::string& port);
};