#include"DroselException.h"

DroselException::DroselException(std::source_location s)
{
	LINE = std::to_string(s.line());
	FILE = s.file_name();
}

const char* DroselException::what() const
{
	return "DROSEL EXCEPTION";
}

const std::string& DroselException::get_line() const
{
	return LINE;
}

const std::string& DroselException::get_file() const
{
	return FILE;
}

std::string DroselException::get_details() const
{
	std::stringstream ss;
	ss  << "<h1>[DROSEL EXCEPTION]</h1>" << "<br/>"
		<< "<h2>[LINE NO]</h2>" << LINE << "<br/>"
		<< "<h2>[FILE NAME]</h2>" << FILE << "<br/>";
	return ss.str();
}

