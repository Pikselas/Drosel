#pragma once
#include"Request.h"
#include"Response.h"
#include"NetworkServer.h"
#include<functional>
#include<sstream>
#include<vector>
class Handler
{
private:
	bool moved = false;
private:
	Request request;
	Response response;
	std::vector<char> RAW_RESPONSE_DATA;
private:
	NetworkBuilder connection;
public:
	Handler(Request request, NetworkBuilder& server);
	Handler(Handler&& handler) noexcept;
	~Handler();
public:
	void operator()(std::function<void(Request&, Response&)> callable = nullptr);
};