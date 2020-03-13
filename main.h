#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>

#define TRUE   1
#define FALSE  0
#define PORT 8888

int multiplexing(int master_socket, int max_clients, fd_set readfds, int *client_socket, sockaddr_in address, int addrlen, const char* message, char* buffer);
