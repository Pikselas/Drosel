#pragma once
#include"DroselServer.h"
#include"Flyler.h"
#include"FilePro.h"
#include"CookieParser.h"

using ReqT = FinalType<Flyler::RequestT , CookieParser::RequestT>; 
using ResT = FilePro::ResponseT;

DroselServer<ReqT, ResT> GLOBAL_DROSEL;

struct PATH_FUNC_T
{
	PATH_FUNC_T(const std::string& path , const std::function<void(ReqT&, ResT&)>& func)
	{
		GLOBAL_DROSEL.OnPath(path, func);
	}
	PATH_FUNC_T(const PathFrog& path , const std::function<void(ReqT&, ResT&)>& func)
	{
		GLOBAL_DROSEL.OnPath(path, func);
	}
};