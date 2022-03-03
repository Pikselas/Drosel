#include"DroselServer.h"
#include"PostParser.h"
#include"FilePro.h"
#include"LiveFiler.h"
#include"PathFrog.h"
#include"DroselException.h"
int main()
{
	try
	{
		using ReqT = FinalType<	
								PostParser::RequestT, 
								LiveFiler::RequestT
							  >;
		using ResT = FilePro::ResponseT;
		
		DroselServer<ReqT , ResT> ds;

		ds.Use(PostParser{});
		ds.Use(LiveFiler{"D:/data"});
		ds.Use(FilePro{});


		ds.OnPath("/img.jpg", [](auto& req , auto& res ) {
			
			res.SendFile("D:/retro.jpg");

			});
		
		
		ds.RunServer("3456");
	}
	catch (const NetworkBuilder::Exception& e)
	{
		std::cerr<<e.what();
	}
	catch (const DroselException& e)
	{
		std::cout << e.get_details();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
	return 0;
}