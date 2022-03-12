#include"PathFrog.h"

PathFrog::PathFrog(const std::string& frog)
{
	*this = frog;
}

PathFrog& PathFrog::operator=(const std::string& frog)
{
	PATH_COMPONENTS.clear();
	KEY_LIST.clear();
	size_t ACCEPT_V = 0;
	for (auto& itm : ksTools::split_by_delms(frog, "/"))
	{
		if (itm.front() == '<' && itm.back() == '>')
		{
			PATH_COMPONENTS.emplace_back();
			if (
				std::string{ ksTools::seek_itr_backward(itm.begin() , itm.end() , 4) , itm.end() -1 } == "..."
				)
			{
				KEY_LIST.emplace_back(itm.begin() + 1, itm.end() - 4);
				ACCEPTSALL_SET.insert(ACCEPT_V);
			}
			else
			{
				KEY_LIST.emplace_back(itm.begin() + 1, itm.end() - 1);
			}
		}
		else
		{
			PATH_COMPONENTS.emplace_back(std::move(itm));
		}
		ACCEPT_V++;
	}
	return *this;
}

std::optional<std::unordered_map<std::string, std::string>> PathFrog::operator==(const std::vector<std::string> & pathcomp) const
{
	size_t key_pos = 0;
	std::unordered_map<std::string, std::string> m;
	bool PrevAcceptsAll = false;
	for (size_t i = 0 , j = 0; i < pathcomp.size(); i++)
	{
		if (PATH_COMPONENTS[j] == pathcomp[i])
		{
			if (PrevAcceptsAll)
			{
				key_pos++;
				PrevAcceptsAll = false;
			}
			j++;
		}
		else
		{
			if (!PATH_COMPONENTS[j])
			{
				if (ACCEPTSALL_SET.find(j) != ACCEPTSALL_SET.end())
				{
					m[KEY_LIST[key_pos]] +=  '/' + pathcomp[i];
					PrevAcceptsAll = true;
				}
				else
				{
					PrevAcceptsAll = false;
					if (key_pos < KEY_LIST.size())
					{
						m[KEY_LIST[key_pos]] = pathcomp[i];
						key_pos++;
					}
					else
					{
						return {};
					}
				}
				j = std::clamp(j + 1, (size_t)0, PATH_COMPONENTS.size() - 1);
			}
			else if (PrevAcceptsAll)
			{
				m[KEY_LIST[key_pos]] += '/' + pathcomp[i];
			}
			else
			{
				return {};
			}
		}
	}
	if (m.size() == KEY_LIST.size())
	{
		return m;
	}
	else
	{
		return {};
	}
}

std::optional<std::unordered_map<std::string, std::string>> PathFrog::operator==(const std::string& path) const
{
	auto vec = std::move(ksTools::split_by_delms(path, "/"));
	return *this == vec;
}
