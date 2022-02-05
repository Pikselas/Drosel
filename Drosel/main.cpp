#include<iostream>
#include"DroselServer.h"
#include"PostParser.h"
int main()
{
	try
	{
		DroselServer<PostParser::RequestT> ds;

		ds.Use(PostParser{});

		ds.OnPath("/", [](auto& req, auto& res) {

			res.AddString("<h1>Hello World</h2>");

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