#pragma once
#include<string>
#include<optional>
#include<unordered_map>
class Header
{
private:
	std::unordered_map<std::string, std::string> headers;
public:
	Header() = default;
	Header(Header&&) noexcept = default;
	Header(const Header&) = default;
public:
	Header& operator= (Header&&) noexcept = default;
	Header& operator= (const Header&) = default;
public: 
	void AddHeader(const std::string& key , const std::string& value);
	std::optional<std::string> GetHeader(const std::string& key) const;
	std::string CounstructRaw() const;
};