#include "main.h"

fd_set readfds;
fd_set writefds;

struct pipes *pipe_ = 0;
struct connection_info *conn_info = 0;
struct LastRequest *Req = 0;

char buffer_[2049];

int multiplexing(int master_socket, int max_clients, int *client_socket, struct sockaddr_in address, int addrlen)
{
    int activity, valread, sd, max_sd, i, j, new_socket, pipe_fd, max_pipe_fd = MAXIMUM_CONNECTIONS * 2, new_i;

    pipe_ = new struct pipes[MAXIMUM_CONNECTIONS];
    conn_info = new struct connection_info[MAXIMUM_CONNECTIONS];
    Req = new struct LastRequest[MAXIMUM_CONNECTIONS];
    
    memset(&buffer_, sizeof(buffer_), 0);

	while(TRUE) 
    {
        FD_ZERO(&readfds);
 
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
		
        for ( i = 0 ; i < max_clients ; i++) 
        {
			sd = client_socket[i];
            
			if(sd > 0)
				FD_SET( sd , &readfds);
            
            if(sd > max_sd)
				max_sd = sd;
        }

        for(int h = 0; h < MAXIMUM_CONNECTIONS; ++h)
            FD_SET(pipe_[h].fds[0], &readfds);
        

        for(int h = 0; h < MAXIMUM_CONNECTIONS; ++h)
            if(conn_info[h].connection_socket != -1 && Req[h].status == "wffc")
                FD_SET(conn_info[h].connection_socket, &writefds);

        max_sd += max_pipe_fd;

        activity = select( max_sd + 1 , &readfds , &writefds, NULL , NULL);
   
        if ((activity < 0) && (errno!=EINTR)) 
            errors_log("SELECT FAILED!");
        
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                errors_log("ACCEPT FAILED!");
                exit(EXIT_FAILURE);
            }

            for (i = 0; i < max_clients; i++) 
            {
				if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
					connections_log(new_socket, address, i);

                    conn_info[i].connection_id = i;

                    new_i = i;

                    conn_info[i].connection_socket = new_socket;
                    conn_info[i].connection_id_of_req_struct = i;

					break;
                }
            }
        }
        
        else
        {
            for(int k = 0; k < MAXIMUM_CONNECTIONS; ++k)
                if(FD_ISSET(pipe_[k].fds[0], &readfds))
                    send_from_pipe(k);
            
            for(int k = 0; k < MAXIMUM_CONNECTIONS; ++k)
            {
                if(Req[k].status == "wffc")
                    if(FD_ISSET(conn_info[k].connection_socket, &writefds))
                        std::cout<< "call form mult: " << std::endl, content_to_buf(k);
            }
        }
 
        for (int j = 0; j < max_clients; j++) 
        {
            sd = client_socket[j];

            if (FD_ISSET( sd , &readfds)) 
            {
                if ((valread = read( sd , buffer_, 1024)) == 0)
                {
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    disconnections_log(address);

                    close( sd );
                    client_socket[j] = 0;
                }
                 
                else   
                    parse_startline(new_i);
            }
        }
    }
     

    return 0;
}


void send_from_pipe(int id)
{
    char buf_fds[2048];

    memset(&buf_fds, 0, sizeof(buf_fds));

    int cord = read(pipe_[id].fds[0], buf_fds, 1000);
    send(conn_info[id].connection_socket, buf_fds, cord, 0);

    //create_fstream("favicon.ico", id);
    //create_fstream("backgr.png", id);
}


/*
void send_from_pipe(int id)
{
    char buf_fds[2048];
    char req_body[2048];
    int i = 0, h_size = 0;

    std::cout << "hey" << std::endl;

    memset(&buf_fds, sizeof(buf_fds), 0);
    memset(&req_body, sizeof(req_body), 0);

    int cord = read(pipe_[id].fds[0], buf_fds, sizeof(buf_fds));

    while(1)
    {
        if(buf_fds[i] == '\r' && buf_fds[i + 1] == '\n' && buf_fds[i + 2] == '\n')
            {h_size = i + 3; break;}
        i++;
    }

    memmove(req_body + 0, buf_fds + (h_size - 1), (cord - h_size));

    Req[id].File_Adr = "backgr.png";

    std::cout << (cord - h_size) << std::endl;

    std::cout << "hoo" << std::endl;

    send(conn_info[id].connection_socket, buf_fds, h_size, 0);
    send(conn_info[id].connection_socket, req_body, cord - h_size, 0);
}*/





