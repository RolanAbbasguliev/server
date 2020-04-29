#include "main.h"
//#include "HTTP_req.h"
#include "HTTP.h"

//LastRequest Req[MAXIMUM_CONNECTIONS]

char response[2048];
//char content_response[260000];

char static_content_buf[2500];

std::string convert(std::vector<char> v)
{
    std::string str(v.begin(), v.end());
    return str;
}

void parse_startline(int i)
{   

    int id = conn_info[i].connection_id_of_req_struct, flag = 0, sl_flag = 0;

    std::vector<char> Meth, HTTP_v, adr, ftype;

    for(int u = 0; u < sizeof(buffer_); ++u)
        std::cout << buffer_[u];

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
        if((buffer_[i] == 0 || buffer_[i] == '/'))
        {
            if(sl_flag == 1)
                adr.push_back(buffer_[i]);
            else if(buffer_[i] == 0)
                ;
            else
                sl_flag = 1;
        }
        else
        {
            if(buffer_[i] == ' ')
            {
                buffer_[i] = 0, flag = 0;
                break;
            }
            else
            {
                if(buffer_[i] == '.')
                {
                    if(buffer_[i + 1] == '.' && buffer_[i + 2] == '/')
                        adr.push_back(buffer_[i]), adr.push_back(buffer_[i]), adr.push_back(buffer_[i + 2]);
                    else
                        flag = 1;
                }
                if(flag == 1)
                {
                    if(buffer_[i] != '.')
                        ftype.push_back(buffer_[i]);
                }

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
    sl_flag = 0;

    Req[id].Method = convert(Meth);
    Req[id].File_Adr = convert(adr);
    Req[id].File_type = convert(ftype);

    std::cout << "\n\n----------------------\n" << Req[id].File_Adr << " || " << Req[id].File_type << "\n---------------------\n" << std::endl;

    Req[id].HTTP_version = convert(HTTP_v);
    
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
    if(Req[id].Method == "GET")
    {
        GET_method(id);
    }
    else
    {
        send_error(id, "Method");
    }
}

void GET_method(int id)
{
    if(Req[id].File_Adr != "")
    {
        //Req[id].status = "wff";
        create_fstream(Req[id].File_Adr, id);
    }
    else
    {
        start_python(id);
    }
    
}

void parse_filetype(int id)
{
    ;
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

void create_fstream(std::string filename, int id)
{
    Req[id].fs.open (filename, std::ifstream::binary);

    struct stat stat_buf;
    stat(filename.c_str(), &stat_buf);

    Req[id].bytes_for_send = stat_buf.st_size;

    Req[id].status = "wff";

    content_to_buf(id);
}

void content_to_buf(int id)
{
    memset(&static_content_buf, sizeof(static_content_buf), 0);

    if(Req[id].bytes_for_send > sizeof(static_content_buf))
        Req[id].fs.read(static_content_buf, sizeof(static_content_buf));
    else
        Req[id].fs.read(static_content_buf, Req[id].bytes_for_send), Req[id].status = "wfr";

    if(Req[id].status == "wfr")
    {
        Req[id].fs.close();
    }

    std::cout << "Bytes for read:  " << Req[id].bytes_for_send << " || " << "Status: " << Req[id].status << std::endl;

    send_content(id); 
}

void send_content(int id)
{
    if(Req[id].status == "wff")
    {
        std::string start = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: ";
        std::string end = "\r\nContent-Transfer-Encoding: binary\r\nContent-Length:";
        int i = 0, j = 0, k = 0;
        
        start += define_content_type(id);
        start += "/";
        start += Req[id].File_type;
        start += end;
        start += " ";
        start += std::to_string(Req[id].bytes_for_send);
        start += "\r\n\r\n";

        send(conn_info[id].connection_socket, start.c_str(), strlen(start.c_str()), 0);
        
        if(Req[id].bytes_for_send <= sizeof(static_content_buf))
        {
            send(conn_info[id].connection_socket, static_content_buf, Req[id].bytes_for_send, 0);
        }
        else
        {
            send(conn_info[id].connection_socket, static_content_buf, sizeof(static_content_buf), 0);
            Req[id].bytes_for_send -= sizeof(static_content_buf);
            Req[id].status = "wffc";
            //content_to_buf(id);
        }
    }
    else if(Req[id].status == "wffc")
    {
        send(conn_info[id].connection_socket, static_content_buf, sizeof(static_content_buf), 0);
        Req[id].bytes_for_send -= sizeof(static_content_buf);
        //content_to_buf(id);
    }
    else if(Req[id].status == "wfr")
    {
        send(conn_info[id].connection_socket, static_content_buf, Req[id].bytes_for_send, 0);
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

std::string define_content_type (int id)
{
    std::string file_type = Req[id].File_type;
    std::string type;

    if(file_type == "jpeg" || file_type == "jpg" || file_type == "png" || file_type == "ico")
        type = "image";

    if(file_type == "mp3")
        type = "music", Req[id].File_type = "mpeg3";
    
    return type;
}

