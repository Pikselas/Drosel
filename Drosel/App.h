#pragma once
#include"Globals.h"


std::initializer_list<PATH_FUNC_T> REGISTERED_FUNCS
{
	{"/" , 
		[](auto& req , auto& res) 
		{
			res.SendFile("C:/Users/Aritra Maji/Downloads/k.avi");
		}
	},

	{"/home/<name>"_PF , 
		[](ReqT& req , ResT& res)
		{
			res.SendFile("D:/" + req.PATH_DATA["name"]);
		}
	}
};