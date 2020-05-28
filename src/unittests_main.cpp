//#include "unittests_main.h"
#include "unittests_main.h"

struct LastRequest *Req = 0;
char buffer_[4096];
char config_buff[3000];
std::map<std::string, std::string> config; 

int main( void )
{
    tests test;
    Req = new struct LastRequest[2];

    if(test.parser())
        std::cout << "OK - PARSER TEST" << std::endl;
    if(test.cfg())
        std::cout << "OK - CFG TEST" << std::endl;

}