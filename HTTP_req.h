#include <string>
#include <map>
#include <fstream>
#include <iostream>

struct LastRequest
{
    std::string Method;
    std::string HTTP_version;
    std::string File_Adr;
    std::string File_type;
    std::map<std::string, std::string> Headers;
    std::string status;
    std::ifstream fs;
    int bytes_for_send;
    int last_read_byte = 0;
};

///////////////////////////////////////////////////
//
//  status:
//  wff - waiting for file
//  wffc - waiting for file continue
//  wfr - all files sent, waiting for new request
//
//
///////////////////////////////////////////////////