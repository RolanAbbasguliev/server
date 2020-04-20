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

void parse_startline(int i)
{   
    int id = conn_info[i].connection_id_of_req_struct;

    //std::cout << "\n\n" << id << std::endl;   //    <---- tut chto-to ne tak -_-

    std::vector<char> Meth, HTTP_v, adr;

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
    //actions_log("parseeeeeeeeeer badumtsss");
    //std::cout << "\n\n" << id << std::endl;
    Req[id].Method = convert(Meth);
    Req[id].File_Adr = convert(adr);                    
    Req[id].HTTP_version = convert(HTTP_v);
    //actions_log("SUPERPARSER");
    parse_headers(id);
}

void parse_headers(int id)
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

                    Req[id].Headers.insert(std::pair<std::string, std::string>(convert(last_h) ,convert(last_h_body)));
                }
                else
                {
                    buf_local.push_back(buffer_[i]);
                }
            } 
        }
    }
    //actions_log("ok - parsed");
    pick_method(id);    
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




void pick_method(int id)
{
    GET_method(id);
}

void GET_method(int id)
{
    start_python(id);
}

void start_python(int id)
{   
    pid_t pid, cp;
    extern char** environ;                
    
    pipe(pipe_[id].fds);

    char* argv[] = {"p.py", "lul", NULL};

    if((pid = fork()) < 0)
    {
        errors_log("FORK error");
    }
    else if(pid == 0)
    {
        //child
        
        close(pipe_[id].fds[0]);
        dup2(pipe_[id].fds[1], STDOUT_FILENO);
        
        setenv("HEADERS_LIST", map_to_str(id).c_str(), 1);
        if(conn_info[id].connection_id == 0 || conn_info[id].connection_id == 2)
            setenv("TEST_M_CON", "0", 1);
        else
            setenv("TEST_M_CON", "1", 1);
        

        cp = execve(argv[0], argv, environ);
        if(cp == -1)
            actions_log(strerror(errno));
        else 
            actions_log("hm, vrode okey");

        
    }
    else
    {
        //parent

        close(pipe_[id].fds[1]);

        //actions_log("ku ka re ku");
        
        
        //int cofb = send_to_main_buff(buf_fds);
        //send_response(id, cofb);
    }
}

std::string map_to_str(int id)
{
    std::string all_headers_s = "";

    for (const auto& x : Req[id].Headers) 
        all_headers_s += x.first + ": " + x.second + "\n\r";
    
    return all_headers_s;
}

int send_to_main_buff(char* buf_fds)
{
    int cofb = 0;
    memset(&response, 0, sizeof(response));
    
    for(int i = 0; buf_fds[i] != 0; ++i)
        response[i] = buf_fds[i], ++cofb;
    
    return cofb;
}

void send_response(int new_socket, int responce_lenth)
{
    if(send(new_socket, response, responce_lenth, 0) != responce_lenth)    
        errors_log("SEND error"); actions_log("Welcome message send sent!");

    memset(&buffer_, 0, sizeof(buffer_));
}


