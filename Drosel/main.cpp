#include<iostream>
#include"DroselServer.h"
int main()
{
	try
	{
		DroselServer sv;
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