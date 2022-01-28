#include<iostream>
#include"DroselServer.h"
#include"PostParser.h"
int main()
{
	try
	{
		DroselServer<
			FinalRequestType<PostParser::RequestT> 
		> ds;

		ds.UseEngine(PostParser());

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
			res.AddString(ss.str());
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