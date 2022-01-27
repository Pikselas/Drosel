#include"Header.h"

void Header::AddHeader(const std::string& key, const std::string& value)
{
	headers[key] = value;
}

std::optional<std::string> Header::GetHeader(const std::string& key) const
{
	auto itr = headers.find(key);
	if (itr != headers.end())
	{
		return itr->second;
	}
	return {};
}

std::string Header::CounstructRaw() const
{
	std::string tmp;
	for (auto& [key, val] : headers)
	{
		tmp += key + ":" + val + "\n";
	}
	if (tmp.length() > 0)
	{
		tmp.erase(tmp.end() - 1);
	}
	return tmp;
}
