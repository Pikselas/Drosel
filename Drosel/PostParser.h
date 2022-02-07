#pragma once
#include<vector>
#include<iostream>
#include<sstream>
#include"Request.h"
#include"NetworkBuilder.h"
class PostParser
{
public:
	class RequestT : public virtual Request
	{
	public:
		std::unordered_map<std::string, std::string> POST;
 	};
	void operator()(RequestT& request , std::vector<char>& raw,std::function<int(int)> fn)
	{
		if (request.METHOD == "POST")
		{
			size_t len = 0;
			std::stringstream ss;
			ss << request.headers.GetHeader("Content-Length").value();
			ss >> len;
			len -= raw.size();
			while (len)
			{
				len -= fn(len);
			}
			for (auto x : raw)
			{
				std::cout << x;
			}
		}
	}
};