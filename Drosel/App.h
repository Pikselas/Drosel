#pragma once
#include"Globals.h"

DROSEL D
{
	{
		"/" , [](ReqT& req , ResT& res)
		{
			res.SetCookie("aritra","maji" , Current_Time() + 60 * 60 * 24);
		}
	}
};
