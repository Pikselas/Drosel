#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include"KsXstr.hpp"
#include"Header.h"
class Parser
{
private:
	std::vector<std::string> FIRST_LINE_SPLITTED;
	std::string RAW_GET_DATA;
private:
	std::vector<std::vector<char>> RAW_DATA;
	std::vector<std::string> RAW_HEADER;
public:
	Parser(std::vector<char>::const_iterator vItrStart, std::vector<char>::const_iterator vItrEnd);
	Header ParseHeaders() const;
	const std::string& ParsePath() const;
	const std::string& ParseRequestMethod();
	std::unordered_map<std::string, std::string> ParsePathData();
};