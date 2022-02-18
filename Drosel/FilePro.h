#pragma once
#include<unordered_map>
#include<fstream>
#include<filesystem>
#include<optional>
#include"Response.h"
class FilePro
{
private:
	const static std::unordered_map<std::string, std::string> CONTENT_TYPES;
public:
	class ResponseT : public virtual Response
	{
	friend class FilePro;
	private:
		std::ifstream File;
	private:
		static std::optional<std::string> GetFileType(const std::filesystem::path& file)
		{
			auto fnd = CONTENT_TYPES.find(file.extension().string());
			if (fnd == CONTENT_TYPES.end())
			{
				return {};
			}
			else
			{
				return fnd->second;
			}
		}
	public:
		void SendFile(const std::string& file)
		{
			if (std::filesystem::exists(file))
			{
				File.open(file, std::ios_base::binary);
				headers.AddHeader("Content-Length", std::to_string(std::filesystem::file_size(file)));
				if (auto c_type = GetFileType(file))
				{
					headers.AddHeader("Content-Type", c_type.value());
				}
			}
		}
	};
	void operator()(ResponseT& response , std::vector<char>& raw)
	{
		if (response.File.is_open())
		{
			std::copy(std::istreambuf_iterator<char>(response.File) , std::istreambuf_iterator<char>{}, std::back_inserter(raw));
		}
	}
};
const std::unordered_map<std::string, std::string> FilePro::CONTENT_TYPES = {
	{".png" , "image/png"} , {".jpg" , "image/jpeg"} ,
	{".ico" , "image/x-icon"},{".jpeg","image/jpeg"} ,
	{".mp4" , "video/mp4"} , {".txt" , "text/plain"}};