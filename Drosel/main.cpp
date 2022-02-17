#include<iostream>
#include<fstream>
#include<vector>
#include"DroselServer.h"
#include"PostParser.h"
#include"FilePro.h"
#include"LiveFiler.h"
#include"PathFrog.h"
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

		ds.OnPath(PathFrog{ "/himawari/<img_name>" }, [](auto& req, auto& res) {

			res.SendFile("D:/USELESS/New_folder/Henta Data/himawar yrs/" + req.PATH_DATA["img_name"]);
			
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