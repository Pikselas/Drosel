#include"Response.h"
void Response::SendString(const std::string& str)
{
	response = str;
}

std::string Response::Get() const
{
	return response;
}
