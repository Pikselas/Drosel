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
	std::vector<typename Handler<RequestT, ResponseT>::FWD_ENGINE_TYPE> FWD_ENGINES;
public:
	void OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function);
	void Use(Handler<RequestT,ResponseT>::FWD_ENGINE_TYPE engine);
	void RunServer(const std::string& port);
};


template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function)
{
	PATH_FUNCTIONS[path] = path_function;
}

template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::Use(Handler<RequestT, ResponseT>::FWD_ENGINE_TYPE engine)
{
	FWD_ENGINES.emplace_back(engine);
}

template<class RequestT, class ResponseT>
void DroselServer < RequestT, ResponseT>::RunServer(const std::string& port)
{
	server = std::make_unique<NetworkServer>(port);
	server->Listen();
	while (true)
	{
		server->AcceptConnection();
		std::vector<char> HeadData;
		HeadData.reserve(1024);
		int LastFindingPos = 0;
		while (auto HasData = server->Receive(1024))
		{
			auto RawData = HasData.value();
			std::copy_n(RawData.first, RawData.second, std::back_inserter(HeadData));

			if ((LastFindingPos = (int) ksTools::FindSubStr( 
								HeadData.data() + std::clamp(LastFindingPos - 4 , 0 , (int)HeadData.size()),
								HeadData.size() , "\r\n\r\n" , 4
						)) != HeadData.size()
				)
			{
				break;
			}
		}

		Parser HeadParser(HeadData.cbegin(), HeadData.cbegin() + LastFindingPos - 1);

		RequestT request;
		request.METHOD = HeadParser.ParseRequestMethod();
		request.headers = std::move(HeadParser.ParseHeaders());
		request.ClientIP = server->GetClientIP();
		request.GET = std::move(HeadParser.ParsePathData());

		Handler<RequestT, ResponseT> hnd(
										std::move(request), 
										{HeadData.begin() + LastFindingPos + 4 , HeadData.end()} , 
											FWD_ENGINES,
											*server
										);
		hnd(PATH_FUNCTIONS[HeadParser.ParsePath()]);
	}
}

template<class ...RequestTypes>
class FinalRequestType : public RequestTypes...
{};

template<class ...ResponseTypes>
class FinalResponseType : public ResponseTypes...
{};