#include"NetworkServer.h"
#include"KsXstr.hpp"
#include<fstream>
#include<iostream>
int main()
{
	try
	{
		NetworkServer ns("3000");
		ns.Listen();
		ns.AcceptConnection();
		NetworkBuilder nb;
		ns.MoveConnection(nb);
		auto x = std::string(nb.Receive().value());
		auto xk = ksTools::split_by_delms(x, "\n\r");
		for (auto d : xk)
		{
			std::cout << d << std::endl;
		}
		nb.Send("HTTP/1.1 200 OK\ncontent - type: text / html; charset = utf - 8");
		nb.Send("\r\n\r\n");
		nb.Send("<h1>HEllo world</h1>");
		nb.DisConnect();
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