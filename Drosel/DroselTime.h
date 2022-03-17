#pragma once
#include<chrono>
#include<ctime>

inline std::time_t Current_Time()
{
	using namespace std::chrono;
	return system_clock::to_time_t(system_clock::now());
}

inline std::string To_Complete_Date(const std::time_t seconds)
{
	std::tm t;
	char buff[64];
	localtime_s(&t, &seconds);
	std::strftime(buff, 64, "%a, %d-%b-%Y %H:%M:%S GMT" , &t);
	return buff;
}