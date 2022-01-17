#pragma once
#include<string>
#include<optional>
#include<unordered_map>
class Header
{
private:
	std::unordered_map<std::string, std::string> headers;
public:
	void AddHeader(const std::string& key , const std::string& value);
	std::optional<std::string> GetHeader(const std::string& key) const;
	std::string CounstructRaw() const;
};