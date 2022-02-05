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
public:
	typedef std::function<void(RequestT&, std::vector<char>&, std::function<int(int)>)> FWD_ENGINE_TYPE;
private:
	RequestT request;
	ResponseT response;
	std::vector<char> RAW_RESPONSE_DATA;
	std::vector<char> RAW_REQUEST_DATA;
private:
	NetworkBuilder connection;
private:
	size_t TOTAL_BODY = 0;
	size_t BODY_RECEIVED;
public:
	int ReceiveData(int size);
public:
	Handler(RequestT request , const std::vector<char>& body ,const std::vector<FWD_ENGINE_TYPE>& engines, NetworkBuilder& server);
	Handler(Handler&& handler) noexcept;
	~Handler();
public:
	void operator()(std::function<void(RequestT&, ResponseT&)> callable = nullptr);
};

template<class RequestT, class ResponseT>
inline int Handler<RequestT, ResponseT>::ReceiveData(int size)
{
	if (BODY_RECEIVED < TOTAL_BODY)
	{
		if (auto dt = connection.Receive(size))
		{
			std::copy_n(dt.value().first, dt.value().second, std::back_inserter(RAW_REQUEST_DATA));
			return dt.value().second;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

template<class RequestT, class ResponseT>
Handler<RequestT , ResponseT>::Handler(RequestT request , const std::vector<char>& body , const std::vector<FWD_ENGINE_TYPE>& engines , NetworkBuilder & conn)
	: 
 request(std::move(request)), RAW_REQUEST_DATA(std::move(body)),connection(std::move(conn))
{
	BODY_RECEIVED = RAW_REQUEST_DATA.size();
	for (auto& engine : engines)
	{
		engine(this->request, RAW_REQUEST_DATA, [](int x) {return x; });
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
		ostr << "HTTP/1.1 " << response.STATUS_CODE << " " << Response::STATUS_CODES.at(response.STATUS_CODE) << "\r\n";
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
		response.STATUS_CODE = 404;
	}
}
