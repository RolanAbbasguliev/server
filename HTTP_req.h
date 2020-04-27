#include <string>
#include <map>

struct LastRequest
{
    std::string Method;
    std::string HTTP_version;
    std::string File_Adr;
    std::map<std::string, std::string> Headers;
    std::string status;
    int bytes_for_send;
};

///////////////////////////////////////////////////
//
//  status:
//  wff - waiting for file
//  wffc - waiting for file continue
//  wfr - all files sent, waiting for new request
//
//
//////////////////////////////////////////////////