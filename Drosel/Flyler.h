#pragma once
#include<map>
#include<vector>
#include<filesystem>
#include<functional>
#include<iostream>
#include<unordered_map>
#include<fstream>
#include"Request.h"
#include"kgenerals.h"
class Flyler
{
private:
	std::string path = "./";
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
	enum class WRITING_TYPE
	{
		FILE_DATA , POST_DATA , NONE
	};

	std::unordered_map<std::string, std::string> GetItemDetails(std::string_view view)
	{
		std::unordered_map<std::string, std::string> mp;
		auto splitted_by_newline = std::move(ksTools::split_by_word(view,"\r\n"));
		for (auto& itm : ksTools::split_by_word(splitted_by_newline.front(), ";"))
		{
			auto splt = std::move(ksTools::split_by_delms(std::string(itm), ":=\""));
			ksTools::trim(splt.front());
			ksTools::trim(splt.back());
			mp[std::move(splt.front())] = std::move(splt.back());
		}
		if (splitted_by_newline.size() > 1)
		{
			auto splt = std::move(ksTools::split_by_delms(std::string(splitted_by_newline.back()) , ":"));
			
			if (splt.size() > 1)
			{
				ksTools::trim(splt.front());
				ksTools::trim(splt.back());
				mp[std::move(splt.front())] = std::move(splt.back());
			}
		}
		return mp;
	}

	void operator()(RequestT& request, std::vector<char>& raw, std::function<int(int)> fn)
	{
		request.storagePath = path;
		if (auto type = request.headers.GetHeader("Content-Type"))
		{
			const auto& type_val = type.value();
			if (type_val.find("multipart/form-data") != std::string::npos)
			{
				auto boundary_start = type_val.find("boundary");
				if (boundary_start != std::string::npos)
				{
					std::string boundary(type_val.begin() + boundary_start + 8, type_val.end());
					std::erase_if(boundary, [](char c) {return c == '='; });
					ksTools::trim(boundary);

					WRITING_TYPE current_writing_type = WRITING_TYPE::NONE;
					std::string current_writing_name;
					std::ofstream outfile;

					std::string end_of_section_str("\r\n\r\n");

					auto write_data = [&](auto start_itr , auto end_itr) 
					{

						if (current_writing_type == WRITING_TYPE::FILE_DATA)
						{
							request.FILES[current_writing_name].back().SIZE += std::distance(start_itr, end_itr);
							std::copy(start_itr, end_itr, std::ostreambuf_iterator(outfile));
						}
						else
						{
							std::copy(start_itr, end_itr, std::back_inserter(request.POST[current_writing_name]));
						}
					};

					do
					{
						while (true)
						{
							auto boundary_pos = std::search(raw.begin(), raw.end(), boundary.begin(), boundary.end());
							if (boundary_pos != raw.end())
							{
								if (*(boundary_pos + boundary.size()) != '-' && *(boundary_pos + boundary.size() + 1) != '-')
								{
									auto pos_from_search = ksTools::seek_itr_forward(boundary_pos, raw.end(), boundary.size() + 2);
									auto Indx = std::distance(raw.begin(), boundary_pos);
									while (true)
									{
										auto section_end_pos = std::search(pos_from_search, raw.end(),end_of_section_str.begin(), end_of_section_str.end());
										if (section_end_pos != raw.end())
										{
											write_data(raw.begin(), ksTools::seek_itr_backward(raw.begin(), boundary_pos, 2));
											
											if (current_writing_type == WRITING_TYPE::FILE_DATA)
											{
												outfile.close();
											}

											auto itemdtls = std::move(GetItemDetails({ boundary_pos + boundary.size() + 2 , section_end_pos }));
											current_writing_name = itemdtls["name"];
											auto is_file = itemdtls.find("filename");

											if (is_file != itemdtls.end())
											{
												current_writing_type = WRITING_TYPE::FILE_DATA;
												outfile.open( path + is_file->second, std::ios_base::binary);
												request.FILES[current_writing_name].emplace_back(std::move(is_file->second), 0, itemdtls["type"], "" );
											}
											else
											{
												current_writing_type = WRITING_TYPE::POST_DATA;
												request.POST[current_writing_name];
											}

											raw.erase(raw.begin(), section_end_pos + 4);
											break;
										}
										else
										{
											auto len = raw.size() - 5;
											if (!fn(1024))
											{
												return;
											}
											// getting new iterator as if vector grows old one gets invalidated
											boundary_pos = raw.begin() + Indx;
											pos_from_search = raw.begin() + len;

										}
									}
								}
								else
								{
									write_data(raw.begin(), ksTools::seek_itr_backward(raw.begin(), boundary_pos, 2));
									return;
								}
							}
							else
							{
								auto end_itr = ksTools::seek_itr_backward(raw.begin(), raw.end(), boundary.size());
								write_data(raw.begin() , end_itr);
								raw.erase(raw.begin(), end_itr);
								break;
							}
						}
					} 
					while (fn(1024));
				}
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
	Flyler() = default;
	Flyler(const std::optional<std::string>& path) : path(path ? (std::filesystem::is_directory(path.value()) ? path.value() + '/' : "./") : "./")
	{}
};