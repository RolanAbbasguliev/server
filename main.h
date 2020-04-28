#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/stat.h>
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
#define MAXIMUM_CONNECTIONS 10

struct connection_info
{
    int connection_id;
    int connection_socket = -1;
    int connection_id_of_req_struct;
};

struct pipes
{
    int fds[2];
    int id_of_connection_info_struct;
};

struct LastRequest
{
    std::string Method;
    std::string HTTP_version;
    std::string File_Adr;
    std::string File_type;
    std::map<std::string, std::string> Headers;
    std::string status;
    std::ifstream fs;
    int bytes_for_send = 0;
    //int last_read_byte = 0;
};

extern LastRequest Req[MAXIMUM_CONNECTIONS];
extern fd_set readfds;
extern char buffer_[2049];
//extern int count_of_connections;
extern int count_of_child_proc;
extern connection_info conn_info[MAXIMUM_CONNECTIONS];
extern int pipe_fds[MAXIMUM_CONNECTIONS * 2];
//extern std::map<int, int*> pipe_fds_list;
extern pipes pipe_[MAXIMUM_CONNECTIONS];

int multiplexing(int master_socket, int max_clients, int *client_socket, sockaddr_in address, int addrlen);
void main_log();
void connections_log(int new_socket, struct sockaddr_in address, int i);
void disconnections_log(struct sockaddr_in address);
void errors_log(std::string error);
void actions_log(std::string action);
void parse_startline(int i);
void parse_headers(int id);
void pick_method(int id);
void GET_method(int id);
void start_python(int id);
std::string map_to_str(int id);
int send_to_main_buff(char* buf_fds);
void send_response(int new_socket, int responce_lenth);
void send_error(int id, std::string error);
void static_content_to_buf(std::string filename, int id);
void send_static_content(int id, int cont_lenth);
std::string define_content_type (int id);
//////////////////////////////////
void create_fstream(std::string filename, int id);
void content_to_buf(int id);
void send_content(int id);
void send_from_pipe(int id);

