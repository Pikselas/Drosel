#pragma once
#include"Header.h"
struct Request
{
	using u_map = std::unordered_map<std::string, std::string>;
	std::string path;
	u_map GET;
	Header header;
};