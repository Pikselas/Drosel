#pragma once
#include<vector>
#include<iostream>
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
	void operator()(RequestT& request, std::vector <std::vector<char>>& raw, NetworkBuilder& nb)
	{
		if (request.METHOD == "POST")
		{
			if (auto x = request.header.GetHeader("Content-Type"))
			{
				for (auto& x : raw)
				{
					std::copy(x.begin(), x.end(), std::ostream_iterator<char>(std::cout));
				}
			}
		}
	}
};