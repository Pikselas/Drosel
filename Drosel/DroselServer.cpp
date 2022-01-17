#include "DroselServer.h"

void DroselServer::RunServer(const std::string& port)
{
	server = std::make_unique<NetworkServer>(port);
	server->Listen();
	while (true)
	{
		server->AcceptConnection();
		auto rawData = server->Receive();
		if (rawData)
		{
			auto splitted = ksTools::split_by_delms(std::string(rawData.value()), "\n");
			auto path = ksTools::split_by_delms(splitted.front(), " ")[1];
			splitted.erase(splitted.begin());
			Request request;
			request.path = path;
			for (auto& dt : splitted)
			{
				auto vec = ksTools::split_by_delms(dt, ":");
				ksTools::trim(vec[0]);
				ksTools::trim(vec[1]);
				request.header.AddHeader(vec[0], vec[1]);
			}
			Handler(request, *server.get());
		}
	}
}
