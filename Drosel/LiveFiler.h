#pragma once
#include<map>
#include<vector>
#include<filesystem>
#include<functional>
#include<iostream>
#include"Request.h"
class LiveFiler
{
private:
	const std::string path;
	public:
	class RequestT : virtual public Request
	{
		friend class LiveFiler;
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


	}
	LiveFiler(const std::string& path) : path(std::filesystem::is_directory(path) ? path : "")
	{}
};