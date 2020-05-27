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
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <list>
#include <boost/algorithm/string.hpp>

#define TRUE   1
#define FALSE  0

extern int MAXIMUM_CONNECTIONS;
extern int _PORT_;
extern std::string FILES_PATH;

/**
 * @brief Struct with info about connection
 */

struct connection_info
{
    connection_info()
    {
        int connection_socket = -1;
    }
    int connection_id;
    int connection_socket = -1;
    int connection_id_of_req_struct;
};

/**
 * @brief This struct containes file deckriptors for pipes
 */
struct pipes
{
    pipes()
    {
        id_of_connection_info_struct = 0;
        fds[0] = 0;
        fds[0] = 1;
    }
    int fds[2];
    int id_of_connection_info_struct;
};

/**
 * @brief This stuct containes request info
 */
struct LastRequest
{
    std::string Method;
    std::string HTTP_version;
    std::string File_Adr;
    std::string File_type;
    std::map<std::string, std::string> Headers;
    std::string Body;
    std::string status;
    std::ifstream fs;
    int bytes_for_send = 0;
    int count_of_r = 0;
};

extern std::map<std::string, std::string> config;
extern LastRequest *Req;
extern char buffer_[2049];
extern connection_info *conn_info;
extern pipes *pipe_;

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
void start_interp(int id, std::string script_name);
std::string map_to_str(int id);
int send_to_main_buff(char* buf_fds);
void send_response(int new_socket, int responce_lenth);
void send_error(int id, std::string error);
void static_content_to_buf(std::string filename, int id);
void send_static_content(int id, int cont_lenth);
std::string define_content_type (int id);
void create_fstream(std::string filename, int id);
void content_to_buf(int id);
void send_content(int id);
void send_from_pipe(int id);
std::string convert(std::vector<char> v);
void load_cfg_file();
void parse_cfg_file(int cfg_size);
void exec_cfg();
int str_to_int(std::string string);
void POST_method(int id);
std::vector<char> useless_func(int i);
std::string generate_filename(int id);
