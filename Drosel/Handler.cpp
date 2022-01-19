#include"Handler.h"
Handler::Handler( Request request, NetworkBuilder& conn) : request(request) , connection(std::move(conn))
{
		
}

Handler::~Handler()
{
	std::ostringstream ostr;
	ostr << "HTTP/1.1 " << response.status_code << " " << Response::STATUS_CODE[response.status_code] << "\r\n";
	ostr << response.headers.CounstructRaw();
	ostr << "\r\n\r\n";
	ostr << response.Get();
	connection.Send(ostr.str());
}

void Handler::operator()(std::function<void(Request&, Response&)> callable)
{
	if (callable != nullptr)
	{
		callable(request, response);
	}
}
