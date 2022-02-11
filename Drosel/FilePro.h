#pragma once
#include<vector>
#include<fstream>
#include"Response.h"
class FilePro
{
public:
	class ResponseT : virtual public Response
	{
	friend class FilePro;
	private:
		struct FILE
		{
			enum struct TYPE
			{
				JPG,
				PNG,
				UNKNOWN
			};
			std::ifstream file;
			size_t size = 0;
			std::string name;
			TYPE type = TYPE::UNKNOWN;
		};
		FILE File;
	private:
		static FILE::TYPE GetFileType(const std::string& file)
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
			if (ext == "jpg")
			{
				return FILE::TYPE::JPG;
			}
			else if (ext == "png")
			{
				return FILE::TYPE::PNG;
			}
			else
			{
				return FILE::TYPE::UNKNOWN;
			}
		}
	public:
		void SendFile(const std::string& file)
		{
			
			File.file.open(file, std::ios_base::binary);
			if (File.file.good())
			{
				File.name = file;
				File.file.seekg(0, std::ios_base::end);
				File.size = File.file.tellg();
				File.file.seekg(0, std::ios_base::beg);
			}
		}
	};
	void operator()(ResponseT& response , std::vector<char>& raw)
	{
		if (response.File.size > 0)
		{
			std::stringstream ss;
			ss << response.File.size;
			response.headers.AddHeader("Content-Length" , ss.str());
			using type = ResponseT::FILE::TYPE;
			switch (ResponseT::GetFileType(response.File.name))
			{
			case type::JPG:
				response.headers.AddHeader("Content-Type", "image/jpg");
				break;
			case type::PNG:
				response.headers.AddHeader("Content-Type", "image/png");
				break;
			case type::UNKNOWN:
				break;
			}
			std::copy_n(std::istreambuf_iterator<char>(response.File.file), response.File.size, std::back_inserter(raw));
		}
	}
};