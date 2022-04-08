#pragma once
#include<string>
#include<sstream>
#include<filesystem>
#include<exception>
#include<source_location>

using source = std::source_location;

class DroselException : public std::exception
{
private:
	std::string LINE;
	std::string FILE;
public:
	DroselException(source s = source::current());
public:
	virtual const char* what() const override;
	const std::string& get_line() const;
	const std::string& get_file() const;
	virtual std::string get_details() const;
};