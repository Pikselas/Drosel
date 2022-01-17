#pragma once
#include"Request.h"
#include"Response.h"
#include"NetworkBuilder.h"
#include<functional>
class Handler
{
private:
	Request request;
	Response response;
private:
	NetworkBuilder connection;
public:
	Handler(Request request, NetworkBuilder& connection);
};