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

extern char buffer_[4096];
extern char config_buff[3000];
extern std::map<std::string, std::string> config;

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
    std::string WebKitFormBoundary;
    int bytes_for_send = 0;
    int count_of_r = 0;
    int bound_flag = 0;
};

extern LastRequest *Req;

void load_cfg_file();
void parse_cfg_file(int cfg_size);
void parse_startline(int i);
void parse_headers(int id);
bool WebKitFromBoundary_check(int id);
std::vector<char> useless_func(int i);
std::string convert(std::vector<char> v);
int str_to_int(std::string string);

class tests
{
    public:
        
        bool parser()
        {
            memset(&buffer_, 0, sizeof(buffer_));
            for(int u = 0; u < sizeof(parser_testline); u++)
                buffer_[u] = parser_testline[u];

            parse_startline(1);

            if(Req[1].Method != "GET")
                std::cout << "PARSER TEST FAILED - METHOD" << std::endl;
            else if(Req[1].File_Adr != "/")
                std::cout << "PARSER TEST FAILED - FILE ADRESS" << std::endl;
            else if(Req[1].HTTP_version != "HTTP/1.1")
                std::cout << "PARSER TEST FAILED - HTTP VERSION" << std::endl;
            else if(Req[1].Body != "This is request body")
                std::cout << "PARSER TEST FAILED - REQUEST'S BODY" << std::endl;
            else 
                return true;
            return false;
        }

        bool cfg()
        {
            memset(&buffer_, 0, sizeof(buffer_));
            memset(&config_buff, 0, sizeof(config_buff));
            int error_flag = 0;
           
            load_cfg_file(); 
            
            auto it = config.cbegin();
            
            if(it->first != "FILES_PATH" || it->second != " /Users/viktor/Desktop/server_/files")
                error_flag++;
            ++it;
            if(it->first != "MAXIMUM_CONNECTIONS" || str_to_int(it->second) != 10)
                error_flag++;
            ++it;
            if(it->first != "PORT" || str_to_int(it->second) != 1234)
                error_flag++;
            
            if(error_flag > 0){
                std::cout << "CFG TEST FAILED : " << error_flag << std::endl; return false;}
            return true;

        }

    private:

        char parser_testline[109] = "GET / HTTP/1.1\r\nConnection: keep-alive\r\nContent-type: image/png\r\nContent-Length: 786\r\n\r\nThis is request body";
};
