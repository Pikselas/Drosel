#include"DroselServer.h"
#include"Flyler.h"
#include"FilePro.h"

int main()
{
	using ReqT = Flyler::RequestT;
	using ResT = FilePro::ResponseT;
	DroselServer<ReqT , ResT> ds;
	
	ds.Use(Flyler{"D:/data"});
	ds.Use(FilePro{});
	ds.OnPath("/posthere", [](auto& req, auto& res) {

		std::stringstream ss;
		ss << "<h1>UPLOADED DATA</h1>";
		ss << "<h2>";
		for (auto& [k, v] : req.POST)
		{
			ss << k << "->" << v;
		}
		ss << "</h2>";
		ss << "<h1>UPLOADED FILES</h1>";
		for (auto& [k, v] : req.GetUploadedFiles())
		{
			for (auto& file : v)
			{
				ss << file.NAME << "->" << file.SIZE;
			}
		}

		res.SendString(ss.str());

		});

	ds.OnPath("/", [](auto& req, auto& res) {

		res.SendFile("C:/Users/Aritra Maji/Videos/Drosel TEST.mp4");

		});
	
	ds.RunServer("3456");

	return 0;
}