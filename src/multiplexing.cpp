#include "main.h"

fd_set readfds; ///< Select() are waiting this fds for reading
fd_set writefds; ///< Select() are waiting this fds for writing

struct pipes *pipe_ = 0;
struct connection_info *conn_info = 0;
struct LastRequest *Req = 0;

char buffer_[4096]; ///< Buffer for incoming requests

/**
 * @brief Main fuction of the project
 * 
 *  @param master_socket Master socket
 *  @param max_clients Deprecated
 *  @param client_socket Pointer to array of client's sockets fds
 *  @param adress TCP information
 *  @param addrlen Lenth of adress
 * 
 *  @return While(1) function. Returning 0 if error
 */
int multiplexing(int master_socket, int max_clients, int *client_socket, struct sockaddr_in address, int addrlen)
{
    int activity, valread, sd, max_sd, i, j, new_socket, pipe_fd, max_pipe_fd = MAXIMUM_CONNECTIONS * 2, new_i;

    pipe_ = new struct pipes[MAXIMUM_CONNECTIONS]; 
    conn_info = new struct connection_info[MAXIMUM_CONNECTIONS];
    Req = new struct LastRequest[MAXIMUM_CONNECTIONS];
    
    memset(&buffer_, 0, sizeof(buffer_));

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
   
        //if ((activity < 0) && (errno!=EINTR)) 
            //errors_log("SELECT FAILED!");
        
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
                        content_to_buf(k);
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

/**
 * @brief Sending to client data from pipe
 * 
 * @param id ID of client, which must receive this data
 * 
 * @return Nothing
 */
void send_from_pipe(int id)
{
    char buf_fds[2048];

    memset(&buf_fds, 0, sizeof(buf_fds));

    int cord = read(pipe_[id].fds[0], buf_fds, 1000);

    send(conn_info[id].connection_socket, buf_fds, cord, 0);
}







