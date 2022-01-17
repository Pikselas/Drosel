#pragma once
#include"Header.h"
class Response
{
private:
	std::string response;
public:
	void SendString(const std::string& str);
	std::string Get() const;
};