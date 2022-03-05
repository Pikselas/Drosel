#pragma once
#include<functional>
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
	class MultipleResponseException : public DroselException
	{
	public:
		MultipleResponseException(std::source_location s = std::source_location::current()) : DroselException(s) {};
	};
public:
	void LockResponse(std::source_location s = std::source_location::current());
	void SendString(const std::string& str);
	virtual void Reset();
	std::string Get() const;
};