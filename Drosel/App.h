#pragma once
#include"Globals.h"


std::initializer_list<PATH_FUNC_T> REGISTERED_FUNCS
{
	{"/" , 
		[](auto& req , auto& res) 
		{
			res.SendFile("E:/movies/Man.Of.Steel.(2013).720p.Dual.Audio.(Hin-Eng).[MoviesFlix.in].mkv");
		}
	},

	{"/home/<name>"_PF , 
		[](ReqT& req , ResT& res)
		{
			res.SendFile("D:/" + req.PATH_DATA["name"]);
		}
	},

	{
		"/upload_any",
		[](ReqT& req , ResT&)
		{
			auto &upFile = req.GetUploadedFiles().at("fl").at(0);
			upFile.SaveFile("D:/data");
		}
	}
};