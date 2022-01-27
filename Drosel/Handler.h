#pragma once
#include"Request.h"
#include"Response.h"
#include"NetworkServer.h"
#include<functional>
#include<sstream>
#include<vector>
template<class RequestT , class ResponseT>
class Handler
{
private:
	bool moved = false;
private:
	RequestT request;
	ResponseT response;
	std::vector<char> RAW_RESPONSE_DATA;
private:
	NetworkBuilder connection;
public:
	Handler(RequestT request, NetworkBuilder& server);
	Handler(Handler&& handler) noexcept;
	~Handler();
public:
	void operator()(std::function<void(RequestT&, ResponseT&)> callable = nullptr);
};

template<class RequestT, class ResponseT>
Handler<RequestT , ResponseT>::Handler(RequestT request, NetworkBuilder & conn) : request(request), connection(std::move(conn))
{

}

template<class RequestT, class ResponseT>
Handler<RequestT , ResponseT>::Handler(Handler&& handler) noexcept
	:
	request(handler.request), connection(std::move(handler.connection)), RAW_RESPONSE_DATA(std::move(handler.RAW_RESPONSE_DATA))
{
	moved = true;
}

template<class RequestT, class ResponseT>
Handler<RequestT,ResponseT>::~Handler()
{
	if (!moved)
	{
		std::ostringstream ostr;
		ostr << "HTTP/1.1 " << response.status_code << " " << Response::STATUS_CODE[response.status_code] << "\r\n";
		ostr << response.headers.CounstructRaw();
		ostr << "\r\n\r\n";
		ostr << response.Get();
		const auto& stremData = ostr.str();
		std::copy(stremData.begin(), stremData.end(), std::back_inserter(RAW_RESPONSE_DATA));
		connection.Send(RAW_RESPONSE_DATA.data(), RAW_RESPONSE_DATA.size());
	}
}

template<class RequestT, class ResponseT>
void Handler<RequestT , ResponseT>::operator()(std::function<void(RequestT&, ResponseT&)> callable)
{
	if (callable != nullptr)
	{
		callable(request, response);
	}
	else
	{
		response.status_code = 404;
	}
}
