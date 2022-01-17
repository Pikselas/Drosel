#pragma once
#include<thread>
#include<functional>
#include<memory>
#include"Handler.h"
#include"NetworkServer.h"
#include"KsXstr.hpp"
#include"Request.h"
class DroselServer
{
private:
	std::unique_ptr<NetworkServer> server;
public:
	void RunServer(const std::string& port);
};