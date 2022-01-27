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
	std::vector<char> RAW_DATA;
	std::vector<std::string> RAW_HEADER;
private:
	int PathDataBeginIndex;
public:
	Parser(const char* RawData , size_t size);
	Header ParseHeaders() const;
	const std::string& ParsePath() const;
	const std::string& ParseRequestMethod();
	std::unordered_map<std::string, std::string> ParsePathData();
	std::vector<char>& GetLeftOverData();
};