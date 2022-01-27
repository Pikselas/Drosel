#pragma once
#include<thread>
#include<functional>
#include<memory>
#include"Handler.h"
#include"NetworkServer.h"
#include"KsXstr.hpp"
#include"Request.h"
#include"Parser.h"

template<class RequestT  = Request, class ResponseT = Response>
class DroselServer
{
private:
	std::unique_ptr<NetworkServer> server;
	std::unordered_map < std::string, std::function<void(RequestT&, ResponseT&)>> PATH_FUNCTIONS;
public:
	void OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function);
	void RunServer(const std::string& port);
};


template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function)
{
	PATH_FUNCTIONS[path] = path_function;
}

template<class RequestT, class ResponseT>
void DroselServer < RequestT, ResponseT>::RunServer(const std::string& port)
{
	server = std::make_unique<NetworkServer>(port);
	server->Listen();
	while (true)
	{
		server->AcceptConnection();
		auto rawData = server->Receive(1024);
		if (rawData)
		{
			Parser ps(rawData.value().first, rawData.value().second);
			auto path = ps.ParsePath();
			Request request(std::move(ps.ParseHeaders()));
			request.ClientIP = server->GetClientIP();
			request.METHOD = ps.ParseRequestMethod();
			request.GET = std::move(ps.ParsePathData());
			/*std::thread([this, request]() {
				Handler hnd (request, *server.get());
				hnd(callables[request.path]);
				}).detach();*/
			Handler<RequestT, ResponseT> hnd(request,*server);
			hnd(PATH_FUNCTIONS[path]);

		}
	}
}

template<class ...RequestTypes>
class FinalRequestType : public RequestTypes...
{};

template<class ...ResponseTypes>
class FinalResponseType : public ResponseTypes...
{};