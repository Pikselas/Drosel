#pragma once
#include<random>
#include<string>
class UnID
{
private:
	size_t LENGTH;
	std::mt19937 random_engine;
	std::uniform_int_distribution<int> element_picker;
	static const std::string str_list;
public:
	UnID(size_t len = 10) : LENGTH(len), random_engine(std::random_device{}()) , element_picker( 0 , str_list.size() - 1) {}
	std::string operator()()
	{
		std::string ID;
		ID.reserve(LENGTH);
		for (auto i = 0; i < LENGTH; ++i)
		{
			ID.push_back(str_list[element_picker(random_engine)]);
		}
		return ID;
	}
};

const std::string UnID::str_list = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";