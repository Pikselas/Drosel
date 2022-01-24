#include "DroselServer.h"

void DroselServer::OnPath(const std::string& path, std::function<void(Request&, Response&)> callable)
{
	callables[path] = callable;
}

void DroselServer::RunServer(const std::string& port)
{
	server = std::make_unique<NetworkServer>(port);
	server->Listen();
	while (true)
	{
		server->AcceptConnection();
		auto rawData = server->Receive(1024);
		if (rawData)
		{
			Parser ps(rawData.value().first , rawData.value().second);
			auto path = ps.ParsePath();
			Request request(std::move(ps.ParseHeaders()));
			request.ClientIP = server->GetClientIP();
			request.METHOD = ps.ParseRequestMethod();
			request.GET = std::move(ps.ParsePathData());
			/*std::thread([this, request]() {
				Handler hnd (request, *server.get());
				hnd(callables[request.path]);
				}).detach();*/
			Handler hnd(request, *server);
			hnd(callables[path]);
		}
	}
}
