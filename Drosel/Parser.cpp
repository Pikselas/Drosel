#include"Parser.h"
#include<iostream>
Parser::Parser(const char* RawData, size_t size)
{
	// dividing Header parts and data parts
	auto RAW_DATA_AND_HEADER = std::move(ksTools::split_by_word(RawData , size , "\r\n\r\n" , 4));
	std::string FirstLine(RAW_DATA_AND_HEADER.front().begin(),RAW_DATA_AND_HEADER.front().end());
	
	// sperating all the headers by line
	RAW_HEADER = std::move(ksTools::split_by_word(FirstLine, "\r\n"));
	
	// removing first line as it is not a header
	FIRST_LINE_SPLITTED = std::move(ksTools::split_by_delms(RAW_HEADER.front(), " "));
	RAW_HEADER.erase(RAW_HEADER.begin());
	
	// storing Raw data

	RAW_DATA = std::move(RAW_DATA_AND_HEADER.back());
	auto EndItr = FIRST_LINE_SPLITTED[1].end();
	for (auto i = FIRST_LINE_SPLITTED[1].begin(); i < EndItr; i++)
	{
		if (*i == '?')
		{
			RAW_GET_DATA = std::move(std::string(i + 1, EndItr));
			FIRST_LINE_SPLITTED[1].erase(i, EndItr);
			break;
		}
	}
}

Header Parser::ParseHeaders() const
{
	Header header;
	for (auto& dt : RAW_HEADER)
	{
		auto vec = ksTools::split_by_delms(dt, ":");
		if (vec.size() > 1)
		{
			ksTools::trim(vec[0]);
			if (vec.size() > 2)
			{
				std::string tmp;
				for (auto i = vec.begin() + 1; i < vec.end(); i++)
				{
					if (i != vec.end() - 1)
					{
						tmp += *i + ":";
					}
				}
				header.AddHeader(vec[0], tmp);
			}
			else
			{
				header.AddHeader(vec[0], vec[1]);
			}
		}
	}
	return header;
}

const std::string& Parser::ParsePath() const
{
	return FIRST_LINE_SPLITTED[1];
}

const std::string& Parser::ParseRequestMethod()
{
	return FIRST_LINE_SPLITTED[0];
}
