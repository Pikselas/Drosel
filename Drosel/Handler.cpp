#include"Handler.h"
Handler::Handler( Request request, const NetworkServer& server) : request(request)
{
	server.MoveConnection(connection);
}