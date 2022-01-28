#include"Response.h"

std::unordered_map<int, std::string> Response::STATUS_CODE = {
	{200 , "OK"},
	{404 , "NOT FOUND"} };

void Response::AddString(const std::string& str)
{
	response += str;
}

std::string Response::Get() const
{
	return response;
}
