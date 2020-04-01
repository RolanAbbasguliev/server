#include "main.h"
#include "HTTP_req.h"
#include "HTTP.h"

LastRequest Req[MAXIMUM_CONNECTIONS];
char response[2048];

std::string convert(std::vector<char> v)
{
    std::string str(v.begin(), v.end());
    return str;
}

void parse_startline(int new_socket)
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
    parse_headers(new_socket);
}

void parse_headers(int new_socket)
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

    pick_method(new_socket);
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




void pick_method(int new_socket)
{
    GET_method(new_socket);
}

void GET_method(int new_socket)
{
    start_python(new_socket);
}

void start_python(int new_socket)
{   
    pid_t pid, cp;
    int fds[2];  
                    
    pipe(fds);

    //setenv("HEADERS_LIST", map_to_str().c_str(), 1);

    extern char** environ;

    char* argv[] = {"p.py", "lul", NULL};

    if((pid = fork()) < 0)
    {
        errors_log("FORK error");
    }
    else if(pid == 0)
    {
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        
        setenv("HEADERS_LIST", map_to_str().c_str(), 1);

        cp = execve(argv[0], argv, environ);
        if(cp == -1)
            actions_log(strerror(errno));
        else 
            actions_log("hm, vrode okey");

        
    }
    else
    {
        //parent

        //std::cout << "here is parent's output: \n\n" << std::endl; 

        close(fds[1]);

        char buf_fds[1024];

        //dup2(fds[0], STDIN_FILENO);

        int cord = read(fds[0], buf_fds, 1000);

        //std::cout << "paaaaarent\n---------------------\n"<< "SIZE: " << cord << std::endl;
        /*
        int i = 0;
        while(1)
        {
            char ch = buf_fds[i];
            if(ch == 0)
                break;
            else
            {
                std::cout << ch;
                ++i;
            }
        }*/
        close(fds[0]);

        int status;
	    waitpid(cp, &status, 0);
        
        //actions_log("hello from parent");
        send_to_main_buff(buf_fds);
        send_response(new_socket);
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

void send_to_main_buff(char* buf_fds)
{
    memset(&response, 0, sizeof(response));
    
    for(int i = 0; buf_fds[i] != 0; ++i)
        response[i] = buf_fds[i];
}

void send_response(int new_socket)
{
    if(send(new_socket, response, strlen(response), 0) != strlen(response))
    {    
        errors_log("SEND error"); actions_log("Welcome message send sent!");
    }
}


