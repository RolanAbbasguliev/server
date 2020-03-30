#include <string>
#include <map>

struct LastRequest
{
    std::string Method;
    std::string HTTP_version;
    std::string File_Adr;
    std::map<std::string, std::string> Headers;
};