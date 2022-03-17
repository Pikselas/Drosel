#pragma once
#include<functional>
#include"Request.h"
#include"KsXstr.hpp"
class CookieParser
{
public:
	class RequestT : virtual public Request
	{
	public:
		std::unordered_map<std::string, std::string> COOKIES;
	};
public:
	void operator()(RequestT& request, std::vector<char>& raw, std::function<int(int)> fn)
	{
		if (auto cook = request.headers.GetHeader("Cookie"))
		{
			auto raw_cookies = std::move(ksTools::split_by_delms(cook.value(), ";"));
			for (auto& cookie : raw_cookies)
			{
				auto temp = std::move(ksTools::split_by_delms(cookie , "="));
				ksTools::trim(temp.front());
				ksTools::trim(temp.back());
				request.COOKIES[std::move(temp.front())] = std::move(temp.back());
			}
		}
	}
};