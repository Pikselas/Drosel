#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string_view>
#include<string>
#include<vector>
#include<optional>
#include<memory>
#include<exception>
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
	constexpr static int RECEIVE_SIZE = 4096;
	std::string RECV_BUFF;
 	mutable bool HasConnection = false;
	mutable SOCKET CONNECTION_SOCKET = INVALID_SOCKET;
public:
	NetworkBuilder();
	~NetworkBuilder();
public:
	static std::vector<std::string> GetDeviceIPs();
public:
	void MoveConnection(NetworkBuilder& nb) const;
	bool IsConnected() const noexcept;
	void ResizeReceiveBuffer(const int size) noexcept;
    void Send(const std::string& data);
	void Send(const char* DataBuffer, const size_t DataLen);
	std::optional<std::string_view> Receive();
	std::optional<std::pair<const char*,size_t>> Receive(size_t size);
	void DisConnect() noexcept;
};

#define ThrowException(code) throw NetworkBuilder::Exception(__LINE__,__FILE__,code)