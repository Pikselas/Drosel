#pragma once
#include"Globals.h"


std::initializer_list<PATH_FUNC_T> REGISTERED_FUNCS
{
	{"/" , 
		[](auto& req , auto& res) 
		{
			res.SendString("HELLO WORLD");
			res.SendString("OK");
		}
	},

	{"/home/<name>"_PF , 
		[](ReqT& req , ResT& res)
		{
			res.SendString("Hello " + req.PATH_DATA["name"]);
		}
	}
};