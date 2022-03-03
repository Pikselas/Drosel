#pragma once
#include<thread>
#include<functional>
#include<memory>
#include"Handler.h"
#include"NetworkServer.h"
#include"KsXstr.hpp"
#include"Request.h"
#include"Parser.h"
#include"PathFrog.h"

template<class RequestT  = Request, class ResponseT = Response>
class DroselServer
{
private:
	std::unique_ptr<NetworkServer> server;
	std::unordered_map < std::string, std::function<void(RequestT&, ResponseT&)>> PATH_FUNCTIONS;
	std::vector < std::pair<PathFrog, std::function<void(RequestT&, ResponseT&)>>> FROGGED_FUNCTIONS;
	std::vector<typename Handler<RequestT, ResponseT>::FWD_ENGINE_TYPE> FWD_ENGINES;
	std::vector<typename Handler<RequestT, ResponseT>::BCKWD_ENGINE_TYPE> BCKWD_ENGINES;
public:
	void OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function);
	void OnPath(const PathFrog& pathfrog, std::function<void(RequestT&, ResponseT&)> frog_function);
	void Use(Handler<RequestT,ResponseT>::FWD_ENGINE_TYPE engine);
	void Use(Handler<RequestT, ResponseT>::BCKWD_ENGINE_TYPE engine);
	void RunServer(const std::string& port);
};


template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::OnPath(const std::string& path, std::function<void(RequestT&, ResponseT&)> path_function)
{
	PATH_FUNCTIONS[path] = path_function;
}

template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::OnPath(const PathFrog& pathfrog, std::function<void(RequestT&, ResponseT&)> frog_function)
{
	FROGGED_FUNCTIONS.emplace_back(pathfrog, frog_function);
}

template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::Use(Handler<RequestT, ResponseT>::FWD_ENGINE_TYPE engine)
{
	FWD_ENGINES.emplace_back(engine);
}

template<class RequestT, class ResponseT>
void DroselServer<RequestT, ResponseT>::Use(Handler<RequestT, ResponseT>::BCKWD_ENGINE_TYPE engine)
{
	BCKWD_ENGINES.emplace_back(engine);
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

			if ((LastFindingPos = (int)ksTools::FindSubStr(
				HeadData.data() + std::clamp(LastFindingPos - 4, 0, (int)HeadData.size()),
				HeadData.size(), "\r\n\r\n", 4
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

		std::function<void(RequestT&, ResponseT&)> func = nullptr;

		const auto& path = request.PATH = HeadParser.ParsePath();

		auto itr = PATH_FUNCTIONS.find(path);
		if (itr != PATH_FUNCTIONS.end())
		{
			func = itr->second;
		}
		else
		{
			for (auto& pr : FROGGED_FUNCTIONS)
			{
				if (auto pdt = std::move((pr.first == path)))
				{
					func = pr.second;
					request.PATH_DATA = std::move(pdt.value());
					break;
				}
			}
		}

		Handler<RequestT, ResponseT>{
										std::move(request),
										{ 
											std::clamp(HeadData.begin() + LastFindingPos + 4 , HeadData.begin() , HeadData.end()),
											HeadData.end()
										},
											FWD_ENGINES,
											BCKWD_ENGINES,
											* server
									}(func);

	}
}

template<class ...Types>
class FinalType : public Types...
{};