#pragma once
#include<functional>
#include<list>
#include"Header.h"
#include"DroselException.h"
class Response
{
private:
	bool FINAL_RESPONSE = false;
private:
	std::string response;
	std::list<std::string> cookies;
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
protected:
	void LockResponse(std::source_location s = std::source_location::current());
public:
	void SendString(const std::string& str);
	void SetCookie(const std::string& name, const std::string& value, std::optional<std::string> path = {}, std::optional<std::string> domain = {} , bool httponly = false);
	virtual void Reset();
	std::string Get() const;
	std::string ConstructRawCookies() const;
};