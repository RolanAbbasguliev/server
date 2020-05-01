#include "main.h"

char error_method[] = "HTTP/1.1 405 Method Not Allowed\r\nContent-type: text/html\r\nAllow: GET\r\n\n<h1>405 Wrong Method</h1>";
char error_file[] = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\n\n<h1>404 Requested object not found</h1>";

void send_error(int id, std::string error)
{
    if(error == "Method")
        send(conn_info[id].connection_socket, error_method, strlen(error_method), 0);
    if(error == "fnf")
        send(conn_info[id].connection_socket, error_file, strlen(error_file), 0);
}
