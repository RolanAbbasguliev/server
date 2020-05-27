#include "main.h"

char static_content_buf[2500]; ///< Buffer for static content like images etc.

/**
 * @brief Picking method for request
 * 
 * @param id ID of client which sent this request
 * 
 * @return Nothing
 */
void pick_method(int id)
{
    if(Req[id].Method == "GET")
        GET_method(id);
    else if(Req[id].Method == "POST")
        POST_method(id);
    else
        send_error(id, "Method");
}

/**
 * @brief Function for HTTP GET method
 * 
 * @param id ID of client which sent this request
 * 
 * @return Nothing
 */
void GET_method(int id)
{
    std::string c_type, script = "files/p.py"; 

    c_type = define_content_type(id);

    std::cout << "|| c_type : " << c_type << " || \n" << std::endl;
        
    if(c_type == "script")
        if(Req[id].File_Adr == "/")
            start_interp(id, script); 
    
        else
        {
            script = Req[id].File_Adr;
            start_interp(id, script);
        }
    else
        create_fstream(Req[id].File_Adr, id);
}

/**
 * @brief Function for HTTP POST method
 * 
 * @param id ID of client which sent this request
 * 
 * @return Nothing
 * 
 * @todo This function
 */
void POST_method(int id)
{
    FILE *F;
    std::string filename = Req[id].File_Adr; 

    if((F = fopen(filename.c_str(), "rb")) == NULL)
    {
        //close(F);

        std::string outfile_name = generate_filename(id);
        std::ofstream outfile (outfile_name);

        outfile << Req[id].Body;

        //next step: send all this shit to .tmp file and fix parser (405 error in big POSTs)
    }
}

/**
 * @brief Generating filenames for .tmp files
 * 
 * @param id ID of the client
 * 
 * @return std::string Filename
 */
std::string generate_filename(int id)
{
    std::string result;

    //need to add filetype in name generator

    return result = "client_" +  std::to_string(id) + ".tmp";
}

/**
 * @brief Starting interpreter for scrip-generated pages
 * 
 * Starting new process via execve() and setting env vars for it
 * 
 * @param id ID of client which sent this request
 * 
 * @param script_name Filepath of script which must be started
 * 
 * @return Nothing
 */ 
void start_interp(int id, std::string script_name)
{   
    pid_t pid, cp;
    pid_t status;
    extern char** environ;               
    
    pipe(pipe_[id].fds);

    char* argv[] = {(char *)script_name.c_str(), "lul", NULL};

    std::cout << "----" << script_name << "----" << std::endl;

    if((pid = fork()) < 0)
        errors_log("FORK error");

    else if(pid == 0)
    {
        if((pid = fork()) < 0)
            errors_log("FORK error");
        else if(pid > 0)
            exit(0);
        
        waitpid(pid, &status, 0);
        close(pipe_[id].fds[0]);
        dup2(pipe_[id].fds[1], STDOUT_FILENO);
        
        setenv("HEADERS_LIST", map_to_str(id).c_str(), 1);
        setenv("F_PATH", FILES_PATH.c_str(), 1);

        cp = execve(argv[0], argv, environ);
        if(cp == -1)
            actions_log(strerror(errno));
        else 
            actions_log("hm, vrode okey");
    }
    else
        close(pipe_[id].fds[1]);
}

/**
 * @brief Creating std::ifstream for static content
 * 
 * @param filename Name of file which must be sent to client
 * 
 * @param id ID of client which sent this request
 * 
 * @return Nothing
 */
void create_fstream(std::string filename, int id)
{
    FILE *F;

    if((F = fopen(filename.c_str(), "rb")) == NULL)
        send_error(id, "fnf");

    else
    {
        fclose(F);
        Req[id].fs.open (filename, std::ifstream::binary);

        struct stat stat_buf;
        stat(filename.c_str(), &stat_buf);

        Req[id].bytes_for_send = stat_buf.st_size;

        Req[id].status = "wff";

        content_to_buf(id);
    }
}

/**
 * @brief Filling static_content_buf 
 * 
 * @param id ID of client which sent this request
 * 
 * @return Nothing
 */
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

    send_content(id); 
}

/**
 * @brief Sending static_content_buf to client
 * 
 * @param id of client which sent this request
 * 
 * @return Nothing
 */
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

        std::cout << "************\n" << start << "**************\n" << std::endl;

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

/**
 * @brief Converting std::map to std::string
 * 
 *  Convering map of request headers to string to set them as env var for script
 * 
 * @param id ID of client which sent this request
 * 
 * @return std::string made from map
 */
std::string map_to_str(int id)
{
    std::string all_headers_s = "";

    for (const auto& x : Req[id].Headers) 
        all_headers_s += x.first + ": " + x.second + "\n\r";
    
    return all_headers_s;
}

/**
 * @brief Defines type of requested content
 * 
 * @param id ID of client which sent this request
 * 
 * @return std::strint content type
 */
std::string define_content_type (int id)
{
    std::string file_type = Req[id].File_type;
    std::string type;

    if(file_type == "jpeg" || file_type == "jpg" || file_type == "png" || file_type == "ico")
        if(Req[id].File_Adr == "/favicon.ico")
            Req[id].File_Adr = FILES_PATH + Req[id].File_Adr;
        type = "image";

    if(file_type == "mp3")
        type = "music", Req[id].File_type = "mpeg3";

    if(file_type == "py" || Req[id].File_Adr == "/")
        type = "script";


    return type;
}

