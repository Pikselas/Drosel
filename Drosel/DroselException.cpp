#include"DroselException.h"

DroselException::DroselException(source s)
{
	LINE = std::to_string(s.line());
	FILE = std::filesystem::path(s.file_name()).filename().string();
}

const char* DroselException::what() const
{
	return WHAT_BUFFER.c_str();
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
#ifdef _DEBUG
	ss << "<h1>DROSEL EXCEPTION</h1><hr/>"
		<< "<h2>[FILE NAME]</h2><h3>" << FILE << "</h3>"
		<< "<h2>[LINE NO]</h2><h3>" << LINE << "</h3>";
#elif
    ss << "<h1>EXCEPTION HAPPENED</h1>"
#endif
	return ss.str();
}

