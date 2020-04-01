#include "main.h"
#include "HTTP_req.h"
#include "HTTP.h"


void pick_method()
{
    GET_method();
}

void GET_method()
{
    start_python();
}

void start_python()
{
    int env_var = setenv("HEADERS_LIST", map_to_str(), 1);
    if(env_var < 0)
        errors_log("HEADERS_LIST error");

    
    
    pid_t pid;

    char* argv[] = {"p.py", "DICK", NULL};

    if((pid = fork()) < 0)
    {
        errors_log("FORK error");
    }
    else if(pid == 0)
    {
        //child

        //actions_log("FORK ok");
        int cp = execve(argv[0], argv, NULL);
        if(cp == -1)
        {
            //std::cout << strerror(errno);
            actions_log(strerror(errno));
        }
        else 
            actions_log("NOT DICK");
    }
    else
    {
        //parent
        ;
    }
}

const char* map_to_str()
{
    std::string all_headers_s = "";

    for (const auto& x : Req[count_of_connections].Headers) 
    {
        all_headers_s += x.first + ": " + x.second + "\n\r";

        actions_log("added");

        std::cout << all_headers_s;
        //std::cout << x.first << ": " << x.second << "\n";
    }

    const char* all_headers = all_headers_s.c_str();

    actions_log("printing all_headers.......");

    std::cout << std::string(all_headers_s) << std::endl;

    return all_headers;
}

