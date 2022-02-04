#include"Response.h"

const std::unordered_map<int, std::string> Response::STATUS_CODES {
	{200 , "OK"},
	{404 , "NOT FOUND"},
	{301 , "Temporary Redirect"} 
};

void Response::AddString(const std::string& str)
{
	response += str;
}

std::string Response::Get() const
{
	return response;
}
