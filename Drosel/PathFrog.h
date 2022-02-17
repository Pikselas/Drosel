#pragma once
#include<unordered_map>
#include<vector>
#include<optional>
#include"KsXstr.hpp"
class PathFrog
{
private:
	std::vector<std::optional<std::string>> PATH_COMPONENTS;
	std::vector<std::string> KEY_LIST;
public:
	PathFrog() = default;
	PathFrog(const std::string& frog);
	PathFrog(const PathFrog& pathfrog) = default;
public:
	PathFrog& operator=(const PathFrog& pathfrog) = default;
	PathFrog& operator=(const std::string& frog);
	std::optional<std::unordered_map<std::string, std::string>> operator==(const std::vector<std::string>& pathcomp) const ;
	std::optional<std::unordered_map<std::string, std::string>> operator==(const std::string& path) const ;
};