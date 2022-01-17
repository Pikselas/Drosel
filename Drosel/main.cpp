#include<iostream>
#include"DroselServer.h"
int main()
{
	try
	{
		DroselServer sv;
		
		sv.OnPath("/", [](auto& req, auto& res) {

			res.SendString("<h1>Hello World<h1/>");
			});
		
		sv.OnPath("/hentai" , [](auto& req, auto& res) {

			res.SendString("<h1>Hentai<h1/>");
			});

		sv.RunServer("3456");
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