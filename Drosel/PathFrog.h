#pragma once
#include<unordered_map>
#include<vector>
#include<optional>
#include<unordered_set>
#include"KsXstr.hpp"
#include"kgenerals.h"
class PathFrog
{
private:
	std::unordered_set<size_t> ACCEPTSALL_SET;
private:
	std::vector<std::optional<std::string>> PATH_COMPONENTS;
	std::vector<std::string> KEY_LIST;
public:
	PathFrog() = default;
	PathFrog(const std::string& frog);
	PathFrog(const PathFrog& pathfrog) = default;
	PathFrog(PathFrog&&) noexcept  = default;
public:
	PathFrog& operator=(const PathFrog& pathfrog) = default;
	PathFrog& operator=(PathFrog&&) noexcept = default;
	PathFrog& operator=(const std::string& frog);
	std::optional<std::unordered_map<std::string, std::string>> operator==(const std::vector<std::string>& pathcomp) const ;
	std::optional<std::unordered_map<std::string, std::string>> operator==(const std::string& path) const ;
};

inline PathFrog operator ""_PF(const char* frog , size_t length)
{
	return PathFrog(frog);
}