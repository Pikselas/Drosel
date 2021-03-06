#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<optional>
#include<string_view>
#include<exception>
#include<memory>
#include<vector>
#pragma comment (lib, "Ws2_32.lib")

class NetworkBuilder
{
public:
	class Exception : public std::exception
	{
	private:
		int line;
		std::string file;
		std::string Message;
	public:
		Exception(int line, const char* file, const int ErrorCode);
		const char* what() const noexcept override;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
	};
private:
	class Starter
	{
	private:
		Starter();
		~Starter();
		static Starter Stuart;
	};
private:
	constexpr static int MAX_HOST_LENGTH = 80;
protected:
	int RECEIVE_BUFF_SIZE = 4096;
	std::unique_ptr<char[]> RECV_BUFF;
 	bool HasConnection = false;
	SOCKET CONNECTION_SOCKET = INVALID_SOCKET;
protected:
	NetworkBuilder();
public:
	NetworkBuilder(const NetworkBuilder&) = delete;
	NetworkBuilder(NetworkBuilder&& nb) noexcept;
	~NetworkBuilder();
public:
	static std::vector<std::string> GetDeviceIPs();
public:
	bool IsConnected() const noexcept;
	void ResizeReceiveBuffer(const int size) noexcept;
    int Send(const std::string& data);
	int Send(const char* DataBuffer, const int DataLen);
	std::optional<std::string_view> Receive();
	std::optional<std::pair<const char*,int>> Receive(int size);
	void DisConnect() noexcept;
};

#define ThrowException(code) throw NetworkBuilder::Exception(__LINE__,__FILE__,code)