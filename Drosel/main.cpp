#include<iostream>
#include<fstream>
#include<vector>
#include"DroselServer.h"
#include"PostParser.h"
#include"FilePro.h"
int main()
{
	try
	{
		DroselServer
			<
			Request,
			FilePro::ResponseT
			>
			ds;

		ds.Use(FilePro{});

		ds.OnPath("/yokawaii.jpg", [](auto& req, auto& res) {
			res.SendFile("C:/Users/Aritra Maji/Downloads/yokawaii.jpg");
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