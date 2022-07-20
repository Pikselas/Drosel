#include<iostream>
#include"Globals.h"
#include"App.h"

int main()
{
	try
	{
		GLOBAL_DROSEL.Use(CookieParser{});
		GLOBAL_DROSEL.Use(Flyler{ "./" });
		GLOBAL_DROSEL.Use(FilePro{});
		GLOBAL_DROSEL.RunServer("3456");
	}
	catch (std::exception e)
	{
		std::cerr << e.what();
	}
	return 0;
}