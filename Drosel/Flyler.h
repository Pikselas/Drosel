#pragma once
#include<map>
#include<vector>
#include<filesystem>
#include<functional>
#include<iostream>
#include<unordered_map>
#include"Request.h"
class Flyler
{
private:
	std::string path;
	public:
	class RequestT : virtual public Request
	{
		friend class Flyler;
	private:
		std::string storagePath;
	public:
		struct FILE_TYPE
		{
			std::string NAME;
			size_t SIZE;
			std::string TYPE;
			std::string TEMP_NAME;
		};
	private:
		using FILE_MAP_T = std::map<std::string, std::vector<FILE_TYPE>>;
		 FILE_MAP_T FILES;
	public:
		std::unordered_map<std::string, std::string> POST;
		const FILE_MAP_T& GetUploadedFiles() const
		{
			return FILES;
		}
		~RequestT()
		{
			namespace fs  = std::filesystem;
			for (const auto& [key , file_arr] : FILES)
			{
				for (const auto& file : file_arr)
				{
					auto path = storagePath + file.TEMP_NAME;
					if (fs::is_regular_file(path))
					{
						std::filesystem::remove(storagePath + file.TEMP_NAME);
					}
				}
			}
		}
	};

	void operator()(RequestT& request, std::vector<char>& raw, std::function<int(int)> fn)
	{
		request.storagePath = path + '/';
		if (auto type = request.headers.GetHeader("Content-Type"))
		{
			if (type.value() != "application/x-www-form-urlencoded")
			{
				
			}
			else
			{
				while (fn(100));
				std::string key;
				std::string val;
				bool GotKey = false;
				for (auto c : raw)
				{
					if (c == '&')
					{
						GotKey = false;
						request.POST[key] = std::move(val);
						key.clear();
					}
					else if (c == '=')
					{
						GotKey = true;
						request.POST[key];
					}
					else if(c != '\n' && c != '\r')
					{
						if (GotKey)
						{
							val += c;
						}
						else
						{
							key += c;
						}
					}
				}
				request.POST[key] = val;
				raw.clear();
			}
		}
	}
	Flyler(const std::string& path = "") : path(std::filesystem::is_directory(path) ? path + '/' : "")
	{}
};