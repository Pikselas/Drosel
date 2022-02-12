#pragma once
#include<unordered_map>
#include<fstream>
#include<optional>
#include"Response.h"
class FilePro
{
private:
	const static std::unordered_map<std::string, std::string> CONTENT_TYPES;
public:
	class ResponseT : virtual public Response
	{
	friend class FilePro;
	private:
		struct FILE
		{
			std::ifstream file;
			size_t size = 0;
		};
		FILE File;
	private:
		static std::optional<std::string> GetFileType(const std::string& file)
		{
			std::string ext;
			for (auto i = file.rbegin(); i < file.rend(); i++)
			{
				if (*i != '.')
				{
					ext = *i + ext;
					continue;
				}
				break;
			}
			if (CONTENT_TYPES.find(ext) == CONTENT_TYPES.end())
			{
				return {};
			}
			else
			{
				return CONTENT_TYPES.at(ext);
			}
		}
	public:
		void SendFile(const std::string& file)
		{
			
			File.file.open(file, std::ios_base::binary);
			if (File.file.good())
			{
				File.file.seekg(0, std::ios_base::end);
				File.size = File.file.tellg();
				File.file.seekg(0, std::ios_base::beg);
				std::stringstream ss;
				ss << File.size;
				headers.AddHeader("Content-Length", ss.str());
				if (auto c_type = GetFileType(file))
				{
					headers.AddHeader("Content-Type", c_type.value());
				}
			}
		}
	};
	void operator()(ResponseT& response , std::vector<char>& raw)
	{
		if (response.File.size > 0)
		{
			std::copy_n(std::istreambuf_iterator<char>(response.File.file), response.File.size, std::back_inserter(raw));
		}
	}
};
const std::unordered_map<std::string, std::string> FilePro::CONTENT_TYPES = {
	{"png" , "image/png"} , {"jpg" , "image/jpeg"} ,
	{"ico" , "image/x-icon"},{"jpeg","image/jpeg"} };