
#include"DroselServer.h"
#include"FilePro.h"
#include"Flyler.h"

int main()
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

	return 0;
}