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
	typedef std::function<void(ResponseT& , std::vector<char>&)> BCKWD_ENGINE_TYPE;
private:
	RequestT request;
	ResponseT response;
	std::vector<char> RAW_RESPONSE_DATA;
	std::vector<char> RAW_REQUEST_DATA;
private:
	const std::vector<BCKWD_ENGINE_TYPE>& BCKWD_ENGINES;
private:
	NetworkBuilder connection;
private:
	size_t TOTAL_BODY = 0;
	size_t BODY_RECEIVED;
	constexpr static int TRANSFERR_PER_CALL = 1024 * 1024 * 40;
public:
	int ReceiveData(int size);
public:
	Handler(RequestT request , const std::vector<char>& body ,const std::vector<FWD_ENGINE_TYPE>& fwd_engines , const std::vector<BCKWD_ENGINE_TYPE>& bckwd_engines , NetworkBuilder& conn);
	Handler(Handler&& handler) noexcept;
	~Handler();
public:
	void operator()(std::function<void(RequestT&, ResponseT&)> callable = nullptr);
};

template<class RequestT, class ResponseT>
int Handler<RequestT, ResponseT>::ReceiveData(int size)
{
	if (BODY_RECEIVED < TOTAL_BODY)
	{
		if (auto dt = connection.Receive(size))
		{
			std::copy_n(dt.value().first, dt.value().second, std::back_inserter(RAW_REQUEST_DATA));
			BODY_RECEIVED += dt.value().second;
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
Handler<RequestT , ResponseT>::Handler(RequestT request , const std::vector<char>& body , const std::vector<FWD_ENGINE_TYPE>& fwd_engines , const std::vector<BCKWD_ENGINE_TYPE>& bckwd_engines, NetworkBuilder& conn)
	: 
 request(std::move(request)), RAW_REQUEST_DATA(std::move(body)) , BCKWD_ENGINES(bckwd_engines),connection(std::move(conn))
{
	BODY_RECEIVED = RAW_REQUEST_DATA.size();
	if (auto Has_Body = this->request.headers.GetHeader("Content-Length"))
	{
		std::stringstream ss;
		ss << Has_Body.value();
		ss >> TOTAL_BODY;
	}
	std::function<int(int)> f = std::bind(&Handler::ReceiveData,this, std::placeholders::_1);
	for (auto& engine : fwd_engines)
	{
		engine(this->request, RAW_REQUEST_DATA, f);
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
		while (BODY_RECEIVED < TOTAL_BODY)
		{
			if (auto ob = connection.Receive(TRANSFERR_PER_CALL))
			{
				BODY_RECEIVED += ob.value().second;
			}
		}
		std::ostringstream ostr;
		ostr << "HTTP/1.1 " << response.STATUS_CODE << " " << Response::STATUS_CODES.at(response.STATUS_CODE) << "\r\n";
		ostr << response.headers.CounstructRaw();
		ostr << "\r\n\r\n";
		for (auto& engine : BCKWD_ENGINES)
		{
			engine(response, RAW_RESPONSE_DATA);
		}
		connection.Send(ostr.str());
		size_t response_size = RAW_RESPONSE_DATA.size();
		while (response_size > TRANSFERR_PER_CALL)
		{
			connection.Send(RAW_RESPONSE_DATA.data() + (RAW_RESPONSE_DATA.size() - response_size), TRANSFERR_PER_CALL);
			response_size -= TRANSFERR_PER_CALL;
		}
		connection.Send(RAW_RESPONSE_DATA.data() + (RAW_RESPONSE_DATA.size() - response_size), response_size);
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
