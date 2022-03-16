#pragma once
#include"Globals.h"

DROSEL D
{
	{
		"/" , [](ReqT& req , ResT& res)
		{
			res.SetCookie("aritra","maji");
			res.SetCookie("japan", "hello", "/", "localhost");
		}
	}
};
