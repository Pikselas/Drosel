#pragma once
#include"Header.h"
class Response
{
private:
	std::string response;
public:
	Header headers;
	int status_code = 200;
public:
	static std::unordered_map<int, std::string> STATUS_CODE;
public:
	void SendString(const std::string& str);
	std::string Get() const;
};