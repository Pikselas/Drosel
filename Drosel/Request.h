#pragma once
#include"Header.h"
class Request
{
public:
	using u_map = std::unordered_map<std::string, std::string>;
	u_map GET;
	u_map POST;
	Header header;
	std::string ClientIP;
	std::string METHOD;
public:
	Request() = default;
	Request(Header hdr) : header(std::move(hdr)){}
	Request(Request&&) noexcept = default;
	Request(const Request&) = default;
};