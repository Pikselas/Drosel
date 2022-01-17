#include"Handler.h"
Handler::Handler( Request request, NetworkBuilder& conn) : request(request)
{
	conn.MoveConnection(connection);
	connection.Send("HTTP/1.1 200 OK\ncontent - type: text / html; charset = utf - 8");
	connection.Send("\r\n\r\n");
	connection.Send("<h1>HEllo world</h1>");
}