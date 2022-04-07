#pragma once
#include<chrono>
#include<ctime>
#include<thread>

inline std::time_t Current_Time()
{
	using namespace std::chrono;
	return system_clock::to_time_t(system_clock::now());
}

inline std::string To_Complete_Date(const std::time_t seconds)
{
	std::tm t;
	char buff[64];
	localtime_s(&t, &seconds);
	std::strftime(buff, 64, "%a, %d-%b-%Y %H:%M:%S GMT" , &t);
	return buff;
}

class time_session
{
    std::shared_ptr<bool> Continue;
public:
    void clear()
    {
        if (Continue.use_count() > 0)
        {
            *Continue = false;
            Continue = nullptr;
        }
    }
    friend time_session set_timeout(int ms, std::function<void()> f);
    friend time_session set_interval(int ms, std::function<void()> f);
};

inline time_session set_timeout(int ms, std::function<void()> f)
{
    time_session session;
    session.Continue = std::make_shared<bool>(true);
    auto func = [](int time_out, std::shared_ptr<bool> s_ptr, auto func) {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_out));
        if (*s_ptr)
        {
            func();
        }
    };

    std::thread(func, ms, session.Continue, f).detach();

    return session;
}

inline time_session set_interval(int ms, std::function<void()> f)
{
    time_session session;
    session.Continue = std::make_shared<bool>(true);
    auto func = [](int time_gap, std::shared_ptr<bool> s_ptr, auto func)
    {
        bool Cont = false;
        while (*s_ptr)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(time_gap));
            func();
        }
    };
    std::thread(func, ms, session.Continue, f).detach();
    return session;
}
