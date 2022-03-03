#pragma once
#include"Header.h"
#include"DroselException.h"
class Response
{
private:
	bool FINAL_RESPONSE = false;
private:
	std::string response;
public:
	Header headers;
	int STATUS_CODE = 200;
public:
	const static std::unordered_map<int, std::string> STATUS_CODES;
public:
	class ResponseException : public DroselException
	{

	};
	class MultipleResponseException : ResponseException
	{
		
	};
public:
	void LockResponse()
	{
		if (!FINAL_RESPONSE)
		{
			FINAL_RESPONSE = true;
		}
		else
		{

		}
	}
	void AddString(const std::string& str);
	std::string Get() const;
};