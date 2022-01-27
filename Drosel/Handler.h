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
	std::vector<char> RAW_REQUEST_DATA;
private:
	NetworkBuilder connection;
public:
	typedef std::vector<std::function<void(RequestT&, std::vector<char>& , NetworkBuilder& nb)>> FWD_E;
	typedef	std::vector<std::function<void(ResponseT&, std::vector<char>&)>> BCKWD_E;
private:
	BCKWD_E& bckwd_engines;
public:
	Handler(RequestT request , std::vector<char> RAW_DATA , FWD_E& f_engines , BCKWD_E& b_engines , NetworkBuilder& server);
	Handler(Handler&& handler) noexcept;
	~Handler();
public:
	void operator()(std::function<void(RequestT&, ResponseT&)> callable = nullptr);
};

template<class RequestT, class ResponseT>
Handler<RequestT , ResponseT>::Handler(RequestT request , std::vector<char> RAW_DATA , FWD_E& f_engines , BCKWD_E& b_engines,NetworkBuilder & conn)
	: 
RAW_REQUEST_DATA(std::move(RAW_DATA)),
request(request), connection(std::move(conn)) , bckwd_engines(b_engines) 
{
	for (auto& engine : f_engines)
	{
		engine(request, RAW_REQUEST_DATA, connection);
	}
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
		// call backward_engines here
		//..
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
