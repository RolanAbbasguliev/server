#include "main.h"

fd_set readfds;
//fd_set writefds;

//int pipe_fds[MAXIMUM_CONNECTIONS * 2];

//std::map<int, int*> pipe_fds_list;

pipes pipe_[MAXIMUM_CONNECTIONS];

connection_info conn_info[MAXIMUM_CONNECTIONS];

char buffer_[2049];
int count_of_connections = -1;
//int count_of_child_proc = 0;  // ++ when we doing fork()

int multiplexing(int master_socket, int max_clients, int *client_socket, struct sockaddr_in address, int addrlen)
{
    int activity, valread, sd, max_sd, i, j, new_socket, pipe_fd, max_pipe_fd = MAXIMUM_CONNECTIONS * 2, new_i;
    //sock_to_reqStruct StRs[MAXIMUM_CONNECTIONS];
    
    memset(&buffer_, 0, sizeof(buffer_));

	while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
 
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
		
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
			sd = client_socket[i];
            
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
            
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
				max_sd = sd;
        }
        //actions_log("1");
        
        for(int h = 0; h < MAXIMUM_CONNECTIONS; ++h)
        {
            FD_SET(pipe_[h].fds[0], &readfds);
        }

        /*for(int h = 0; h < MAXIMUM_CONNECTIONS; ++h)
        {
            if(conn_info[h].connection_socket != -1)
                FD_SET(conn_info[h].connection_socket, &writefds);
        }*/

        //actions_log("2");
        max_sd += max_pipe_fd;

        //child_fds = count_of_child_proc * 2 + 1;

        //wait for an activity on one of the sockets , timeout is NULL , so wait...

        activity = select( max_sd + 1 , &readfds , /*&writefds*/NULL , NULL , NULL);
   
        if ((activity < 0) && (errno!=EINTR)) 
        {
            errors_log("SELECT FAILED!");
        }
         
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                errors_log("ACCEPT FAILED!");
                exit(EXIT_FAILURE);
            }

            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
				if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
					connections_log(new_socket, address, i);

                    //std::cout << "\n\n" << i << std::endl;

                    conn_info[i].connection_id = i;

                    //std::cout << "\n\n" << conn_info[i].connection_id << std::endl;

                    new_i = i;

                    conn_info[i].connection_socket = new_socket;
                    conn_info[i].connection_id_of_req_struct = i;
                    //conn_info[i].first_pipe_fd_id = i + 2; 

					break;
                }
            }
        }
        
        else
        {
            //std::cout << "WTFFFFFFFF" << std::endl;
            for(int k = 0; k < MAXIMUM_CONNECTIONS; ++k)
            {
                if(FD_ISSET(pipe_[k].fds[0], &readfds))
                {
                    char buf_fds[2048];

                    memset(&buf_fds, 0, sizeof(buf_fds));

                    int cord = read(pipe_[k].fds[0], buf_fds, 1000);
                    send(conn_info[k].connection_socket, buf_fds, cord, 0);

                }
            }
            /*
            for(int k = 0; k < MAXIMUM_CONNECTIONS; ++k)
            {
                if(conn_info[k].connection_socket == -1)
                {
                    ;
                }
                else
                {
                    if(FD_ISSET(conn_info[k].connection_socket, &writefds))
                    {
                        ;
                    }
                }
            }*/
        }
        //actions_log("ASS");
        //
        //its some IO operation 
        for (int j = 0; j < max_clients; j++) 
        {
            sd = client_socket[j];
             
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and read the incoming message
                if ((valread = read( sd , buffer_, 1024)) == 0)
                {
                    //Somebody disconnected , print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    disconnections_log(address);

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[j] = 0;
                }
                 
                else
                {   
                    //std::cout << "\n\n i: " << i << std::endl;

                    parse_startline(new_i);
                }
            }
        }
    }
     
    return 0;
}

//убрать сенд из мультипл в отдельное место + сделать селект дескрипторов сокета на рид 




/*actions_log("blblblblblblbl");
            for( j = 0; j < max_pipe_fd; ++j)
            {
                actions_log("BIG JOPA");

                //for (const auto& x : pipe_fds_list) {
                    //std::cout << x.first << ": " << x.second << "\n";

                if(FD_ISSET(, &readfds))
                {
                    //take id of ready process and start catch his output
                    actions_log("JOPA JOPA JOPA");
                    for(int k = 0; k < MAXIMUM_CONNECTIONS; ++k)
                    {
                        if(conn_info[k].first_pipe_fd_id == j)
                        {
                            char buf_fds[1024];

                            //std::cout << "la-la-la-la" << std::endl;
                            actions_log("la-la-la-la");

                            memset(&buf_fds, 0, sizeof(buf_fds));

                            //dup2(STDOUT_FILENO, pipe_fds[conn_info[k].first_pipe_fd_id]);

                            int cord = read(conn_info[k].first_pipe_fd_id, buf_fds, 1000);

                            //std::cout << "WTF???" << std::endl;
                            actions_log("WTF???");

                            //send(conn_info[k].connection_socket, buf_fds, cord, 0);
                            send(conn_info[k].connection_socket, "zzzzzzzzzzz", cord, 0);
                        }
                    }
                }
            }*/