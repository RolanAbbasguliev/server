#include "main.h"

char static_content_buf[2500];

void pick_method(int id)
{
    if(Req[id].Method == "GET")
        GET_method(id);

    else
        send_error(id, "Method");
}

void GET_method(int id)
{
    if(Req[id].File_Adr != "")
        create_fstream(Req[id].File_Adr, id);
    
    else
        start_python(id);
    
}

void start_python(int id)
{   
    pid_t pid, cp;
    extern char** environ;               
    
    pipe(pipe_[id].fds);

    char* argv[] = {"p.py", "lul", NULL};

    if((pid = fork()) < 0)
        errors_log("FORK error");

    else if(pid == 0)
    {   
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
        close(pipe_[id].fds[1]);
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

    Req[id].count_of_r++;

    if(Req[id].bytes_for_send > sizeof(static_content_buf))
        Req[id].fs.read(static_content_buf, sizeof(static_content_buf));
    else
        Req[id].fs.read(static_content_buf, Req[id].bytes_for_send), Req[id].status = "wfr";

    if(Req[id].status == "wfr")
        Req[id].fs.close();

    std::cout << "Bytes for read:  " << Req[id].bytes_for_send << " || " << "Status: " << Req[id].status << std::endl;

    send_content(id); 
}

void send_content(int id)
{
    if(Req[id].status != "wffc" && Req[id].count_of_r < 2)
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
    }

    if(Req[id].status == "wff")
    {   
        if(Req[id].bytes_for_send <= sizeof(static_content_buf))
            send(conn_info[id].connection_socket, static_content_buf, Req[id].bytes_for_send, 0);

        else
        {
            send(conn_info[id].connection_socket, static_content_buf, sizeof(static_content_buf), 0);
            Req[id].bytes_for_send -= sizeof(static_content_buf);
            Req[id].status = "wffc";
        }
    }
    else if(Req[id].status == "wffc")
    {
        send(conn_info[id].connection_socket, static_content_buf, sizeof(static_content_buf), 0);
        Req[id].bytes_for_send -= sizeof(static_content_buf);
    }
    else if(Req[id].status == "wfr")
    {
        send(conn_info[id].connection_socket, static_content_buf, Req[id].bytes_for_send, 0);
        Req[id].count_of_r = 0;
    }
}

std::string map_to_str(int id)
{
    std::string all_headers_s = "";

    for (const auto& x : Req[id].Headers) 
        all_headers_s += x.first + ": " + x.second + "\n\r";
    
    return all_headers_s;
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

