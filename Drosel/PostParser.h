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
	void operator()(RequestT& request, std::vector<char>& raw, NetworkBuilder& nb)
	{
		if (request.METHOD == "POST")
		{
			if (auto x = request.header.GetHeader("Content-Type"))
			{
				if (x.value() == "application/x-www-form-urlencoded")
				{
					for (auto r : raw)
					{
						std::cout << r;
					}
				}
			}
		}
	}
};