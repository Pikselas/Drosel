#include"Parser.h"
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
		auto Splitted = std::move(ksTools::split_by_delms( dt, ":"));
		if (Splitted.size() > 1)
		{
			if (Splitted.size() > 2)
			{
				Splitted[1] = ksTools::merge_by_delm(Splitted.begin() + 1, Splitted.end(), ":");
			}
			ksTools::trim(Splitted[0]);
			ksTools::trim(Splitted[1]);
			header.AddHeader(Splitted[0], Splitted[1]);
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

std::unordered_map<std::string, std::string> Parser::ParsePathData()
{
	std::unordered_map<std::string, std::string> GET_DATA;
	auto splittedAr = std::move(ksTools::split_by_delms(RAW_GET_DATA, "&"));
	for (auto& evrElm : splittedAr)
	{
		auto Obj = std::move(ksTools::split_by_delms(evrElm, "="));
		ksTools::trim(Obj.front());
		ksTools::trim(Obj.back());
		GET_DATA[Obj.front()] = Obj.back();
	}
	return GET_DATA;
}

std::vector<char>& Parser::GetLeftOverData()
{
	return RAW_DATA;
}
