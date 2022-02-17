#include"PathFrog.h"

PathFrog::PathFrog(const std::string& frog)
{
	*this = frog;
}

PathFrog& PathFrog::operator=(const std::string& frog)
{
	PATH_COMPONENTS.clear();
	KEY_LIST.clear();
	for (auto& itm : ksTools::split_by_delms(frog, "/"))
	{
		if (itm.front() == '<' && itm.back() == '>')
		{
			KEY_LIST.emplace_back(itm.begin() + 1 , itm.end() - 1);
			PATH_COMPONENTS.emplace_back();
		}
		else
		{
			PATH_COMPONENTS.emplace_back(std::move(itm));
		}
	}
	return *this;
}

std::optional<std::unordered_map<std::string, std::string>> PathFrog::operator==(const std::vector<std::string> & pathcomp) const
{
	if (pathcomp.size() == PATH_COMPONENTS.size())
	{
		size_t key_pos = 0;
		std::unordered_map<std::string, std::string> m;
		for (auto i = 0; i < PATH_COMPONENTS.size(); i++)
		{
			if (PATH_COMPONENTS[i] != pathcomp[i])
			{
				if (PATH_COMPONENTS[i])
				{
					return {};
				}
				else
				{
					m[KEY_LIST[key_pos]] = pathcomp[i];
					key_pos++;
				}
			}
		}
		return m;
	}
	else
	{
		return{};
	}
}

std::optional<std::unordered_map<std::string, std::string>> PathFrog::operator==(const std::string& path) const
{
	auto vec = std::move(ksTools::split_by_delms(path, "/"));
	return *this == vec;
}
