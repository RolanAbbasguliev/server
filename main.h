#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <map>



#define TRUE   1
#define FALSE  0
#define PORT 1234
#define MAXIMUM_CONNECTIONS 5

extern char buffer_[2049];
extern int count_of_connections;
//extern char* all_headers;


struct sock_to_reqStruct
{
    int sock;
    int reqStruct;
};

int multiplexing(int master_socket, int max_clients, fd_set readfds, int *client_socket, sockaddr_in address, int addrlen);
void main_log();
void connections_log(int new_socket, struct sockaddr_in address, int i);
void disconnections_log(struct sockaddr_in address);
void errors_log(std::string error);
void actions_log(std::string action);
void parse_startline(int new_socket);
void parse_headers(int new_socket);
void pick_method(int new_socket);
void GET_method(int new_socket);
void start_python(int new_socket);
std::string map_to_str();
void send_to_main_buff(char* buf_fds);
void send_response(int new_socket);

