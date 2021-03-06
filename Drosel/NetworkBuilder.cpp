#include "NetworkBuilder.h"
NetworkBuilder::Starter NetworkBuilder::Starter::Stuart;
NetworkBuilder::Starter::Starter()
{
	try
	{
		WSADATA wdt;
		auto Res = WSAStartup(MAKEWORD(2, 2), &wdt);
		if (Res != 0)
		{
			ThrowException(Res);
		}
	}
	catch (Exception e)
	{
		//.....
	}
}
NetworkBuilder::Starter::~Starter()
{
	WSACleanup();
}
NetworkBuilder::NetworkBuilder()
{
	ResizeReceiveBuffer(RECEIVE_BUFF_SIZE);
}
NetworkBuilder::NetworkBuilder(NetworkBuilder&& nb) noexcept
{
	CONNECTION_SOCKET = nb.CONNECTION_SOCKET;
	HasConnection = nb.HasConnection;
	ResizeReceiveBuffer(nb.RECEIVE_BUFF_SIZE);
	nb.CONNECTION_SOCKET = INVALID_SOCKET;
	nb.HasConnection = false;
}
std::vector<std::string> NetworkBuilder::GetDeviceIPs()
{
	using namespace std;
	vector<string> IPS;
	auto NameBuffer = std::make_unique<char[]>(MAX_HOST_LENGTH);
	if (gethostname(NameBuffer.get(), MAX_HOST_LENGTH) != SOCKET_ERROR)
	{
		hostent* hosts = gethostbyname(NameBuffer.get());
		if (hosts != nullptr)
		{
			for (int i = 0; hosts->h_addr_list[i] != nullptr; i++)
			{
				in_addr adrs;
				memcpy(&adrs, hosts->h_addr_list[i], sizeof(in_addr));
				IPS.emplace_back(inet_ntoa(adrs));
			}
		}
	}
	return IPS;
}

NetworkBuilder::~NetworkBuilder()
{
	DisConnect();
}

bool NetworkBuilder::IsConnected() const noexcept
{
	return HasConnection;
}

void NetworkBuilder::ResizeReceiveBuffer(const int size) noexcept
{
	RECV_BUFF = std::make_unique<char[]>(size);
}

int NetworkBuilder::Send(const std::string& data)
{
	return Send(data.c_str(), (int) data.length());
}

int NetworkBuilder::Send(const char* DataBuffer, const int DataLen)
{
	int send_stat = send(CONNECTION_SOCKET, DataBuffer, DataLen, 0);
	if (send_stat == SOCKET_ERROR)
	{
		ThrowException(WSAGetLastError());
	}
	return send_stat;
}

std::optional<std::string_view> NetworkBuilder::Receive()
{
	auto Res = recv(CONNECTION_SOCKET, RECV_BUFF.get(), RECEIVE_BUFF_SIZE, 0);
	if (Res > 0)
	{
		return std::string_view(RECV_BUFF.get(), Res);
	}
	else
	{
		HasConnection = false;
		ThrowException(WSAGetLastError());
	}
	return {};
}

std::optional<std::pair<const char*,int>> NetworkBuilder::Receive(int size)
{
	if (size > RECEIVE_BUFF_SIZE)
	{
		ResizeReceiveBuffer(size);
	}
	auto Res = recv(CONNECTION_SOCKET, RECV_BUFF.get(), size, 0);
	if (Res > 0)
	{
		return std::make_pair(RECV_BUFF.get(), Res);
	}
	else
	{
		HasConnection = false;
		ThrowException(WSAGetLastError());
	}
	return {};
}

void NetworkBuilder::DisConnect() noexcept
{
	if (CONNECTION_SOCKET != INVALID_SOCKET)
	{
		shutdown(CONNECTION_SOCKET, SD_BOTH);
		closesocket(CONNECTION_SOCKET);
		HasConnection = false;
		CONNECTION_SOCKET = INVALID_SOCKET;
	}
}

NetworkBuilder::Exception::Exception(int line, const char* file, const int ErrorCode) : line(line) , file(file) , Message(1000,0)
{
	wchar_t* msg = nullptr;
	const auto MSgLEn = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,ErrorCode,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPWSTR)&msg, 0, NULL
	              );
	const auto Size = WideCharToMultiByte(CP_UTF8, 0, msg, (int)MSgLEn, &Message.at(0), (int)Message.length(), nullptr, nullptr);
	Message.resize(Size);
	LocalFree(msg);
}

const char* NetworkBuilder::Exception::what() const noexcept
{
	return Message.c_str();
}

int NetworkBuilder::Exception::GetLine() const noexcept
{
	return line;
}

const std::string& NetworkBuilder::Exception::GetFile() const noexcept
{
	return file;
}
