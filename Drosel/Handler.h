#pragma once
#include"Request.h"
#include"Response.h"
#include"NetworkServer.h"
#include<functional>
#include<sstream>
#include<vector>
#include<iostream>

template<class Type>
concept DerivedFromRequest = std::is_base_of_v<Request, Type>;

template<class Type>
concept DerivedFromResponse = std::is_base_of_v<Response, Type>;

template<DerivedFromRequest RequestT, DerivedFromResponse ResponseT>
class Handler
{
public:
	typedef std::function<void(RequestT&, std::vector<char>&, std::function<int(int)>)> FWD_ENGINE_TYPE;
	typedef std::function<void(ResponseT& , std::vector<char>& , std::function<int(const char* , int)>)> BCKWD_ENGINE_TYPE;
private:
	RequestT request;
	ResponseT response;
	std::vector<char> RAW_RESPONSE_DATA;
	std::vector<char> RAW_REQUEST_DATA;
private:
	const std::vector<FWD_ENGINE_TYPE>& FWD_ENGINES;
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
public:
	void operator()(std::function<void(RequestT&, ResponseT&)> callable = nullptr);
};

template<DerivedFromRequest RequestT, DerivedFromResponse ResponseT>
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

template<DerivedFromRequest RequestT, DerivedFromResponse ResponseT>
Handler<RequestT , ResponseT>::Handler(RequestT request , const std::vector<char>& body , const std::vector<FWD_ENGINE_TYPE>& fwd_engines , const std::vector<BCKWD_ENGINE_TYPE>& bckwd_engines, NetworkBuilder& conn)
	: 
 request(std::move(request)), RAW_REQUEST_DATA(std::move(body)) , FWD_ENGINES(fwd_engines), BCKWD_ENGINES(bckwd_engines), connection(std::move(conn))
{
	BODY_RECEIVED = RAW_REQUEST_DATA.size();
	if (auto Has_Body = this->request.headers.GetHeader("Content-Length"))
	{
		std::stringstream ss;
		ss << Has_Body.value();
		ss >> TOTAL_BODY;
		request.BODY_SIZE = TOTAL_BODY;
	}
}


template<DerivedFromRequest RequestT, DerivedFromResponse ResponseT>
void Handler<RequestT , ResponseT>::operator()(std::function<void(RequestT&, ResponseT&)> callable)
{

	std::function<int(int)> f = std::bind(&Handler::ReceiveData, this, std::placeholders::_1);
	try
	{
		for (auto& engine : FWD_ENGINES)
		{
			engine(this->request, RAW_REQUEST_DATA, f);
		}
		if (callable)
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
	catch (NetworkBuilder::Exception e)
	{
		std::cerr << e.what();
	}
}
