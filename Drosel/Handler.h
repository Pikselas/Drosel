#pragma once
#include"Request.h"
#include"Response.h"
#include"NetworkServer.h"
#include<functional>
#include<sstream>
class Handler
{
private:
	Request request;
	Response response;
private:
	NetworkBuilder connection;
public:
	Handler(Request request, NetworkBuilder& server);
	~Handler();
public:
	void operator()(std::function<void(Request&, Response&)> callable = nullptr);
};