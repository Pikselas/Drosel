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
	std::vector<std::function<void(RequestT& , std::vector<char>& , NetworkBuilder& nb)>> FORWARD_ENGINES;
	std::vector<std::function<void(ResponseT&, std::vector<char>&)>> BACKWARD_ENGINES;
public:
	void OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function);
	void UseEngine(std::function<void(RequestT&, std::vector<char>& , NetworkBuilder& nb)> engine);
	void UseEngine(std::function<void(ResponseT&, std::vector<char>&)> engine);
	void RunServer(const std::string& port);
};


template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function)
{
	PATH_FUNCTIONS[path] = path_function;
}

template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::UseEngine(std::function<void(RequestT&, std::vector<char>& , NetworkBuilder& nb)> engine)
{
	FORWARD_ENGINES.emplace_back(engine);
}

template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::UseEngine(std::function<void(ResponseT&, std::vector<char>&)> engine)
{
	BACKWARD_ENGINES.emplace_back(engine);
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
			RequestT request;
			request.header = std::move(ps.ParseHeaders());
			request.ClientIP = server->GetClientIP();
			request.METHOD = ps.ParseRequestMethod();
			request.GET = std::move(ps.ParsePathData());
			/*std::thread([this, request]() {
				Handler hnd (request, *server.get());
				hnd(callables[request.path]);
				}).detach();*/
			Handler<RequestT, ResponseT> hnd(request , std::move(ps.GetLeftOverData()), FORWARD_ENGINES, BACKWARD_ENGINES, *server);
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