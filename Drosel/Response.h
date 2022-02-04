#pragma once
#include"Header.h"
class Response
{
private:
	std::string response;
public:
	Header headers;
	int STATUS_CODE = 200;
public:
	const static std::unordered_map<int, std::string> STATUS_CODES;
public:
	void AddString(const std::string& str);
	std::string Get() const;
};