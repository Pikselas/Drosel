#include"Globals.h"
#include"App.h"

int main()
{
	GLOBAL_DROSEL.Use(Flyler{ "D:/data" });
	GLOBAL_DROSEL.Use(FilePro{});
	GLOBAL_DROSEL.RunServer("3456");
	return 0;
}