#include"DroselServer.h"
#include"FilePro.h"
#include"Flyler.h"
#include"PathFrog.h"
#include"DroselException.h"
int main()
{
	try
	{
		using ReqT = FinalType<	 
								Flyler::RequestT
							  >;
		using ResT = FilePro::ResponseT;
		
		DroselServer<ReqT , ResT> ds;

		ds.Use(Flyler{});
		ds.Use(FilePro{});


		ds.OnPath("/img", [](auto& req , auto& res ) 
			{
			
			 res.SendString("Hello World");

			});


		ds.OnPath("/posthere", [](auto& req , auto& res) {
			
			std::stringstream ss;
			for (auto& [k, v] : req.POST)
			{
				ss << "<h2>" << k << "</h2>"
					<< v << "<br/>";
			}
			res.SendString(ss.str());
			
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