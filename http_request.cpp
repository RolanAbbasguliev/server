#include "main.h"
#include "HTTP_req.h"
#include "HTTP.h"

LastRequest Req[MAXIMUM_CONNECTIONS];

std::string convert(std::vector<char> v)
{
    std::string str(v.begin(), v.end());
    return str;
}

void parse_startline()
{   
    std::vector<char> Meth, HTTP_v, adr;

    //memset(&Method, 0, sizeof(Method));
    //memset(&HTTP_version, 0, sizeof(HTTP_version));

    for(int i = 0; i != -1; ++i)
    {
        if(buffer_[i] == ' ')
        {
            buffer_[i] = 0;
            break;
        }
        else
        {
            Meth.push_back(buffer_[i]);
            buffer_[i] = 0;
        }
    }

    for(int i = 0; i != -1; ++i)
    {
        if(buffer_[i] == 0)
        {
            ;
        }
        else
        {
            if(buffer_[i] == ' ')
            {
                buffer_[i] = 0;
                break;
            }
            else
            {
                adr.push_back(buffer_[i]);
                buffer_[i] = 0;
            }
        }
    }
        
    for(int i = 0; i != -1; ++i)
    {
        if(buffer_[i] == 0)
        {
            ;
        }
        else
        {
            if(buffer_[i] == '\n')
                break;
            else
            {
                HTTP_v.push_back(buffer_[i]);
                buffer_[i] = 0;
            }
        }
    }

    Req[count_of_connections].Method = convert(Meth);
    Req[count_of_connections].File_Adr = convert(adr);
    Req[count_of_connections].HTTP_version = convert(HTTP_v);

    //for(int i = 0; i < 20; ++i)
    //{
        //std::cout << HTTP_version[i];
    //}
    //std::cout << std::endl;
    //std::cout << count_of_connections << std::endl;
    //std::cout << Req[count_of_connections].Method << "    " << Req[count_of_connections].HTTP_version << std::endl;
    parse_headers();
}

void parse_headers()
{
    std::vector<char> last_h, last_h_body, buf_local;
    int flag = 0;
    
    for(int i = 0; i < 2049; ++i)
    {
        if(buffer_[i] == 0)
        {
            ;
        }
        else
        {
            if(flag != 1)
            {
                buf_local.push_back(buffer_[i]);
                if(buffer_[i] == ':')
                {
                    buf_local.pop_back();
                    last_h = buf_local;
                    buf_local.clear();
                    flag = 1;
                }
            }
            else
            {
                if(buffer_[i] == '\n')
                {
                    flag = 0;
                    buf_local.erase(buf_local.begin());
                    last_h_body = buf_local;
                    buf_local.clear();

                    Req[count_of_connections].Headers.insert(std::pair<std::string, std::string>(convert(last_h) ,convert(last_h_body)));
                }
                else
                {
                    buf_local.push_back(buffer_[i]);
                }
            } 
        }
    }

    //std::cout << "3" << std::endl;

    //for (const auto& x : Req[count_of_connections].Headers) {
        //std::cout << x.first << ": " << x.second << "\n";
    //}

    pick_method();
}


//*******************************************************************************************************************
//
//
//
//
//
//
//   RESPONSE PART THERE
//
//
//
//
//
//
//*******************************************************************************************************************




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
    pid_t pid;

    setenv("HEADERS_LIST", map_to_str().c_str(), 1);

    extern char** environ;

    char* argv[] = {"ph.php", "lul", NULL};

    if((pid = fork()) < 0)
    {
        errors_log("FORK error");
    }
    else if(pid == 0)
    {
        //child

        //actions_log("FORK ok");
        int cp = execve(argv[0], argv, environ);
        if(cp == -1)
        {
            //std::cout << strerror(errno);
            actions_log(strerror(errno));
        }
        else 
            actions_log("hm, vrode okey");
    }
    else
    {
        //parent
        ;
    }
}

std::string map_to_str()
{
    std::string all_headers_s = "";

    for (const auto& x : Req[count_of_connections].Headers) 
    {
        all_headers_s += x.first + ": " + x.second + "\n\r";
        //std::cout << x.first << ": " << x.second << "\n";
    }
    //std::cout << std::string(all_headers_s) << std::endl;
    return all_headers_s;
}


