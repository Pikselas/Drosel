#include"Globals.h"
#include"App.h"
#include<iostream>

int main()
{
	auto hDll = LoadLibrary("D:/PathFunctions.dll");
	auto func = (int(*)())GetProcAddress(hDll, "Function");

	std::cout << func();

	GLOBAL_DROSEL.Use(CookieParser{});
	GLOBAL_DROSEL.Use(Flyler{ "./" });
	GLOBAL_DROSEL.Use(FilePro{});
	GLOBAL_DROSEL.RunServer("3456");
	return 0;
}