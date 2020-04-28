#include <string>
#include <vector>

struct HTTP
{
    std::vector<std::string> Methods = {"GET"};
    std::string HTTP_version = "HTTP/1.1";
    std::vector<std::string> Headers = {"Host", "Connection", "Content-Type", "Upgrade-Insecure-Requests", "Sec-Fetch-Dest"}; 
};

//