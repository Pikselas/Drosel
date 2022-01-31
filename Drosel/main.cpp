#include<iostream>
#include"DroselServer.h"
#include"PostParser.h"
int main()
{
	try
	{
		DroselServer ds;
		ds.OnPath("/", [](auto& req, auto& res)
			{
				res.AddString("<h1>Hello</h1>");
			});
		ds.RunServer("3456");
	}
	catch (const NetworkBuilder::Exception e)
	{
		std::cout<<e.what();
	}
	catch (const std::exception&)
	{
		std::cerr << "SOME EXCEPTION";
	}
	return 0;
}