#pragma once
#include<functional>
#include<list>
#include"Header.h"
#include"DroselTime.h"
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
	   static const std::string what_happend;
	public:
		MultipleResponseException(source s = source::current()) : DroselException(s) {};
		const char* what() const override;
	};
protected:
	void LockResponse(source s = source::current());
public:
	void SendString(const std::string& str , source s = source::current());
	void SetCookie(const std::string& name, const std::string& value, std::optional<std::time_t> expires = {}, std::optional<std::string> path = {}, std::optional<std::string> domain = {}, bool httponly = false);
	virtual void Reset();
	std::string Get() const;
	std::string ConstructRawCookies() const;
};
