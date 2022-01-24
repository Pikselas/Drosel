#include<iostream>
#include"DroselServer.h"
int main()
{
	try
	{
		DroselServer ds;
		ds.OnPath("/", [](auto& req, auto& res) {
			std::ostringstream ss;
			ss << "<table>";
			for (auto& x : req.GET)
			{
				ss << "<tr>"
					<< "<td>" << x.first << "</td>"
					<< "<td>" << x.second << "</td>"
					<< "</tr>";
 			}
			ss << "</table>";
			res.SendString(ss.str());
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