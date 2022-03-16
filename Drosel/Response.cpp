#include"Response.h"

const std::unordered_map<int, std::string> Response::STATUS_CODES {
	{200 , "OK"},
	{404 , "NOT FOUND"},
	{301 , "Temporary Redirect"} 
};

void Response::LockResponse(std::source_location s)
{
	if (!FINAL_RESPONSE)
	{
		FINAL_RESPONSE = true;
	}
	else
	{
		throw MultipleResponseException(s);
	}
}

void Response::SendString(const std::string& str)
{
	LockResponse();
	headers.AddHeader("Content-Length", std::to_string(str.length()));
	response = str;
}

void Response::SetCookie(const std::string& name, const std::string& value, std::optional<std::string> path, std::optional<std::string> domain, bool httponly)
{
	std::string cookie_string = name + '=' + value + ';';
	if (path)
	{
		cookie_string += "path=" + path.value() + ';';
	}
	if (domain)
	{
		cookie_string += "domain=" + domain.value() + ';';
	}
	if (httponly)
	{
		cookie_string += "HttpOnly";
	}
	cookies.emplace_back(std::move(cookie_string));
}

std::string Response::ConstructRawCookies() const
{
	std::string raw;
	for (auto& cookie : cookies)
	{
		raw += "Set-Cookie:" + cookie + "\r\n";
	}
	return raw;
}

void Response::Reset()
{
	headers.Reset();
	STATUS_CODE = 200;
	FINAL_RESPONSE = false;
	response.clear();
	cookies.clear();
}

std::string Response::Get() const
{
	return response;
}
