#pragma once
#include"Globals.h"

std::initializer_list<PATH_FUNC_T> REGISTERED_FUNCS
{
	{"/" , 
		[](auto& req , auto& res) 
		{
			res.SendString("HELLO WORLD");
		}
	},

	{"/home/<name>"_PF , 
		[](ReqT& req , ResT& res)
		{
			res.SendString(req.COOKIES["hi"]);
		}
	}
};