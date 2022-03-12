#pragma once
#include"DroselServer.h"
#include"Flyler.h"
#include"FilePro.h"
#include<any>

using ReqT = Flyler::RequestT;
using ResT = FilePro::ResponseT;

DroselServer<ReqT, ResT> GLOBAL_DROSEL;

using PAIR_T = std::pair<std::any, std::function<void(ReqT&, ResT&)>>;

void UsePath(const PAIR_T& pr)
{
	const auto& type = pr.first.type();
	if (type == typeid(const char *))
	{
		GLOBAL_DROSEL.OnPath(std::any_cast<const char *>(pr.first), pr.second);
	}
	else if (type == typeid(std::string))
	{
		GLOBAL_DROSEL.OnPath(std::any_cast<std::string>(pr.first), pr.second);
	}
	else if (type == typeid(PathFrog))
	{
		GLOBAL_DROSEL.OnPath(std::any_cast<PathFrog>(pr.first), pr.second);
	}
}

struct DROSEL
{
	DROSEL() = default;
	DROSEL(const std::initializer_list<const PAIR_T>& prs)
	{
		for (const auto& pr : prs)
		{
			UsePath(pr);
		}
	}
};
