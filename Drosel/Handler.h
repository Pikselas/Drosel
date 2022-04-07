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
	bool call_functions = true;
public:
	typedef std::function<void(RequestT&, std::vector<char>&, std::function<int(int)>)> FWD_ENGINE_TYPE;
	typedef std::function<void(ResponseT& , std::vector<char>& , std::function<int(const char* , int)>)> BCKWD_ENGINE_TYPE;
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
		request.BODY_SIZE = TOTAL_BODY;
	}
	std::function<int(int)> f = std::bind(&Handler::ReceiveData,this, std::placeholders::_1);
	try
	{
		for (auto& engine : fwd_engines)
		{
			engine(this->request, RAW_REQUEST_DATA, f);
		}
	}
	catch (const DroselException& de)
	{
		call_functions = false;
	}
	catch (const NetworkBuilder::Exception& e)
	{
		call_functions = false;
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
		try
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
			ostr << response.ConstructRawCookies();
			ostr << "\r\n\r\n" << response.Get();
			connection.Send(ostr.str());


			using overloaded = int (NetworkBuilder::*)(const char*, int);
			auto f = std::bind(static_cast<overloaded>(&NetworkBuilder::Send), std::ref(connection), std::placeholders::_1, std::placeholders::_2);

			for (auto& engine : BCKWD_ENGINES)
			{
				engine(response, RAW_RESPONSE_DATA, f);
			}

			size_t response_size = RAW_RESPONSE_DATA.size();
			while (response_size > TRANSFERR_PER_CALL)
			{
				connection.Send(RAW_RESPONSE_DATA.data() + (RAW_RESPONSE_DATA.size() - response_size), TRANSFERR_PER_CALL);
				response_size -= TRANSFERR_PER_CALL;
			}
			connection.Send(RAW_RESPONSE_DATA.data() + (RAW_RESPONSE_DATA.size() - response_size), response_size);
		}
		catch(const NetworkBuilder::Exception& e)
		{

		}
	}
}

template<class RequestT, class ResponseT>
void Handler<RequestT , ResponseT>::operator()(std::function<void(RequestT&, ResponseT&)> callable)
{
	if (callable != nullptr && call_functions)
	{
		try
		{
			callable(request, response);
		}
		catch (const DroselException& de)
		{
			response.Reset();
			response.SendString(de.get_details() + "<hr><h2>" + de.what() + "</h2>");
		}
	}
	else
	{
		response.STATUS_CODE = 404;
	}
}
