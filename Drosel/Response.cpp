#include"Response.h"

const std::unordered_map<int, std::string> Response::STATUS_CODES {
	{200 , "OK"},
	{404 , "NOT FOUND"},
	{301 , "Temporary Redirect"} 
};

void Response::LockResponse(std::source_location s)
{
	if (!FINAL_RESPONSE)
	{
		FINAL_RESPONSE = true;
	}
	else
	{
		throw MultipleResponseException(s);
	}
}

void Response::SendString(const std::string& str)
{
	LockResponse();
	headers.AddHeader("Content-Length", std::to_string(str.length()));
	response = str;
}

void Response::Reset()
{
	headers.Reset();
	STATUS_CODE = 200;
	FINAL_RESPONSE = false;
	response.clear();
}

std::string Response::Get() const
{
	return response;
}
