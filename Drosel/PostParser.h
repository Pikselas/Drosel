#pragma once
#include<vector>
#include<iostream>
#include<sstream>
#include<fstream>
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
			

		}
	}
};