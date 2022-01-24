#include"Handler.h"
Handler::Handler( Request request, NetworkBuilder& conn) : request(request) , connection(std::move(conn))
{
		
}

Handler::Handler(Handler&& handler) noexcept
	: 
request(handler.request), connection(std::move(handler.connection)) , RAW_RESPONSE_DATA(std::move(handler.RAW_RESPONSE_DATA))
{
	moved = true;
}

Handler::~Handler()
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
		connection.Send(RAW_RESPONSE_DATA.data() , RAW_RESPONSE_DATA.size());
	}
}

void Handler::operator()(std::function<void(Request&, Response&)> callable)
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
