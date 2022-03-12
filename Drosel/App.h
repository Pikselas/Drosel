#pragma once
#include"Globals.h"

DROSEL D
{
	{
		"/" , [](ReqT& req, ResT& res)
		{
			res.SendString("Hello World");
		}
	},
	{
		PathFrog{"home/<name...>"} , [](ReqT& req, ResT& res)
		{
			res.SendString(req.PATH_DATA["name"]);
		}
	}
};