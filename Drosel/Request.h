#pragma once
#include"Header.h"
class Request
{
public:
	using u_map = std::unordered_map<std::string, std::string>;
	u_map GET;
	Header headers;
	std::string ClientIP;
	std::string METHOD;
	size_t BODY_SIZE = 0;
public:
	Request() = default;
	Request(Header hdr) : headers(std::move(hdr)){}
	Request(Request&&) noexcept = default;
	Request(const Request&) = default;
};