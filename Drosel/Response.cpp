#include"Response.h"

std::unordered_map<int, std::string> Response::STATUS_CODE = { {200 , "OK"}};

void Response::SendString(const std::string& str)
{
	response = str;
}

std::string Response::Get() const
{
	return response;
}
