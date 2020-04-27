#include "main.h"

char error_method[] = "HTTP/1.1 405 Method Not Allowed\r\nContent-type: text/html\r\nAllow: GET\r\n\n<h1>405 Wrong Method</h1>";

void send_error(int id, std::string error)
{
    if(error == "Method")
        send(conn_info[id].connection_socket, error_method, strlen(error_method), 0);
}